// Inc/sensor_fsm.hpp — tick sequencer: reads both sensors, publishes one LogRecord per Tick
//
// Transition table (see README):
//   #  State       Event   Guard                     Action                                   Next
//   1  Init        start   imu_init == Ok            -                                        Logging
//   2  Init        start   imu_init != Ok            -                                        Recovering
//   3  Logging     Tick    imu_read != Timeout       read env + imu, publish (Nack -> invalid) Logging
//   4  Logging     Tick    imu_read == Timeout       publish with imu_valid = false            Recovering
//   5  Logging     LogOff  -                         -                                        Idle
//   6  Recovering  Tick    bus_reset + imu_init Ok   read env, publish env-only record         Logging
//   6b Recovering  Tick    recovery failed           read env, publish env-only record         Recovering
//   7  Recovering  LogOff  -                         -                                        Idle
//   8  Idle        LogOn   -                         -                                        Logging
//   9  Idle        Tick    -                         nothing                                  Idle
//   -  any         other   -                         ignored                                  same
#pragma once

#include <cstddef>
#include <cstdint>

#include "log_record.hpp"
#include "sensor_hw.hpp"
#include "subject.hpp"

enum class State : std::uint8_t { Init, Logging, Idle, Recovering };
enum class Event : std::uint8_t { Tick, LogOn, LogOff };

template <SensorHw Hw, std::size_t N>
class SensorFsm {
public:
    ~SensorFsm() = default;
    SensorFsm(Hw& hw, Subject<LogRecord, N>& out) : hw_{hw}, out_{out} {}

    SensorFsm(const SensorFsm&)            = delete;
    SensorFsm& operator=(const SensorFsm&) = delete;

    // Rows 1-2. Only acts in Init; calling it again later does nothing.
    void start()
    {
        // If not in Init, return
        if(state_ != State::Init){
            return;
        }
        if(hw_.imu_init() == BusStatus::Ok){
            state_ = State::Logging;
        }else{
            state_ = State::Recovering;
        }
    }

    // Dispatch on the current state; each on_* returns the next state.
    // No `default:` on purpose: -Wswitch warns if a State is added and forgotten here.
    void handle(Event e)
    {
        switch (state_) {
            case State::Init:       break;                     // only start() leaves Init
            case State::Logging:    state_ = on_logging(e);    break;
            case State::Idle:       state_ = on_idle(e);       break;
            case State::Recovering: state_ = on_recovering(e); break;
        }
    }

    [[nodiscard]] State state() const { return state_; }

private:
    // Rows 3, 4, 5
    State on_logging(Event e)
    {
        switch (e) {
            case Event::LogOff: return State::Idle;            // row 5
            case Event::LogOn:  return State::Logging;
            case Event::Tick:{
                LogRecord r{};
                r = begin_record();
                const BusStatus bus_s = hw_.imu_read(r.imu);
                // imu_valid
                r.imu_valid = (bus_s == BusStatus::Ok);
                //publish r
                out_.publish(r);
                //update state
                if(bus_s == BusStatus::Timeout){
                    return State::Recovering;
                }else{
                    return State::Logging;
                } 
            }
        }
        return State::Logging;
    }

    // Rows 8, 9
    State on_idle(Event e)
    {
        switch(e){
            case Event::LogOff:{
                return State::Idle;
            }
            case Event::LogOn:{
                return State::Logging;
            }
            case Event::Tick:{
                return State::Idle;
            }
        }
        return State::Idle;    // unreachable for valid events; keeps the function well-defined
    }

    // Rows 6, 6b, 7
    State on_recovering(Event e)
    {
        switch (e) {
            case Event::LogOff:{ // row 7
                return State::Idle; 
            }case Event::LogOn:{  // already "logging" --> Keep recovering
                return State::Recovering; 
            }case Event::Tick: {  
                LogRecord r = begin_record();
                r.imu_valid = false;
                bool ok = (hw_.bus_reset() == BusStatus::Ok && hw_.imu_init() == BusStatus::Ok);
                out_.publish(r);
                return (ok ? State::Logging : State::Recovering);
            }
        }
        return State::Recovering;
    }

    // Common start of every published record: timestamp + BMP280.
    LogRecord begin_record()
    {
        LogRecord r{};
        r.timestamp_ms = hw_.now_ms();
        r.env_valid = hw_.env_read(r.env);
        return r;
    }

    Hw&                    hw_;
    Subject<LogRecord, N>& out_;
    State                  state_{State::Init};
};