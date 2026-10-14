// test/test_sensor_fsm.cpp — host tests for SensorFsm (one test per table row, plus scenarios)
#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>

#include "sensor_fsm.hpp"
#include "unity.h"

namespace {

// ---------------------------------------------------------------------------
// FakeHw: every result is scripted, every call is counted.
// A script is a short list of results consumed one per call; when it runs out,
// the last entry repeats. Default: everything succeeds.
// ---------------------------------------------------------------------------
template <class T>
struct Script {
    std::array<T, 8> results{};
    std::size_t      len = 0;
    std::size_t      pos = 0;

    void set(std::initializer_list<T> r)
    {
        len = 0;
        pos = 0;
        for (const T& v : r) {
            results[len++] = v;
        }
    }
    T next(T fallback)
    {
        if (len == 0) {
            return fallback;
        }
        const T v = results[pos < len ? pos : len - 1];
        ++pos;
        return v;
    }
};

struct FakeHw {
    Script<BusStatus> imu_init_script;
    Script<BusStatus> imu_read_script;
    Script<bool>      env_read_script;
    Script<BusStatus> bus_reset_script;

    int imu_init_calls  = 0;
    int imu_read_calls  = 0;
    int env_read_calls  = 0;
    int bus_reset_calls = 0;
    int now_calls       = 0;

    std::uint32_t clock_ms = 0;              // advances 1000 ms per now_ms() call

    BusStatus imu_init()
    {
        ++imu_init_calls;
        return imu_init_script.next(BusStatus::Ok);
    }
    BusStatus imu_read(ImuSample& s)
    {
        ++imu_read_calls;
        const BusStatus st = imu_read_script.next(BusStatus::Ok);
        if (st == BusStatus::Ok) {
            s = ImuSample{10, 20, 1000, -1, -2, -3};
        }
        return st;
    }
    bool env_read(EnvSample& e)
    {
        ++env_read_calls;
        const bool ok = env_read_script.next(true);
        if (ok) {
            e = EnvSample{1971, 101140};
        }
        return ok;
    }
    BusStatus bus_reset()
    {
        ++bus_reset_calls;
        return bus_reset_script.next(BusStatus::Ok);
    }
    std::uint32_t now_ms()
    {
        ++now_calls;
        clock_ms += 1000;
        return clock_ms;
    }
};

static_assert(SensorHw<FakeHw>, "FakeHw must satisfy the port");

// Observer that keeps every published record.
struct Sink {
    std::array<LogRecord, 16> records{};
    std::size_t               count = 0;

    void on_record(const LogRecord& r)
    {
        if (count < records.size()) {
            records[count] = r;
        }
        ++count;
    }
    const LogRecord& last() const { return records[count - 1]; }
};

using Out = Subject<LogRecord, 2>;
using Fsm = SensorFsm<FakeHw, 2>;

// Fresh per test (setUp resets them). The Subject is a local in each test:
// it has no unsubscribe, so a new one per test keeps subscriptions from piling up.
FakeHw hw;
Sink   sink;

void wire(Out& o, Sink& s)
{
    TEST_ASSERT_TRUE(o.subscribe(Out::Callback::create<Sink, &Sink::on_record>(s)));
}

#define TEST_ASSERT_STATE(expected, fsm) \
    TEST_ASSERT_EQUAL_INT(static_cast<int>(expected), static_cast<int>((fsm).state()))

} // namespace

void setUp(void)
{
    hw   = FakeHw{};
    sink = Sink{};
}
void tearDown(void) {}

// --- start(): rows 1-2 -----------------------------------------------------

static void test_initial_state_is_init_and_ignores_events(void)
{
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    TEST_ASSERT_STATE(State::Init, fsm);
    fsm.handle(Event::Tick);
    fsm.handle(Event::LogOn);
    TEST_ASSERT_STATE(State::Init, fsm);
    TEST_ASSERT_EQUAL_UINT(0, sink.count);
    TEST_ASSERT_EQUAL_INT(0, hw.imu_init_calls + hw.imu_read_calls + hw.env_read_calls);
}

static void test_row1_start_imu_ok_goes_logging(void)
{
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    TEST_ASSERT_STATE(State::Logging, fsm);
    TEST_ASSERT_EQUAL_INT(1, hw.imu_init_calls);
    TEST_ASSERT_EQUAL_UINT(0, sink.count);                  // start() does not publish
}

static void test_row2_start_imu_fails_goes_recovering(void)
{
    hw.imu_init_script.set({BusStatus::Nack});
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    TEST_ASSERT_STATE(State::Recovering, fsm);
}

static void test_start_twice_does_nothing_the_second_time(void)
{
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::LogOff);                              // now Idle
    fsm.start();
    TEST_ASSERT_STATE(State::Idle, fsm);
    TEST_ASSERT_EQUAL_INT(1, hw.imu_init_calls);
}

// --- Logging: rows 3, 4, 5 -------------------------------------------------

static void test_row3_tick_all_ok_publishes_full_record(void)
{
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::Tick);

    TEST_ASSERT_STATE(State::Logging, fsm);
    TEST_ASSERT_EQUAL_UINT(1, sink.count);
    const LogRecord& r = sink.last();
    TEST_ASSERT_EQUAL_UINT32(1000, r.timestamp_ms);
    TEST_ASSERT_TRUE(r.env_valid);
    TEST_ASSERT_TRUE(r.imu_valid);
    TEST_ASSERT_EQUAL_INT32(1971, r.env.temp_centi_c);
    TEST_ASSERT_EQUAL_INT32(101140, r.env.press_pa);
    TEST_ASSERT_EQUAL_INT32(1000, r.imu.accel_z_mg);
    TEST_ASSERT_EQUAL_INT32(-3, r.imu.gyro_z_mdps);
}

static void test_row3_nack_marks_imu_invalid_and_stays_logging(void)
{
    hw.imu_read_script.set({BusStatus::Nack});
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::Tick);

    TEST_ASSERT_STATE(State::Logging, fsm);                 // Nack: bus works, reset won't help
    TEST_ASSERT_EQUAL_UINT(1, sink.count);
    TEST_ASSERT_FALSE(sink.last().imu_valid);
    TEST_ASSERT_TRUE(sink.last().env_valid);
    TEST_ASSERT_EQUAL_INT(0, hw.bus_reset_calls);
}

static void test_row3_env_failure_marks_env_invalid_only(void)
{
    hw.env_read_script.set({false});
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::Tick);

    TEST_ASSERT_STATE(State::Logging, fsm);
    TEST_ASSERT_FALSE(sink.last().env_valid);
    TEST_ASSERT_TRUE(sink.last().imu_valid);
}

static void test_row4_timeout_publishes_then_goes_recovering(void)
{
    hw.imu_read_script.set({BusStatus::Timeout});
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::Tick);

    TEST_ASSERT_STATE(State::Recovering, fsm);
    TEST_ASSERT_EQUAL_UINT(1, sink.count);                  // the tick still produced a row
    TEST_ASSERT_FALSE(sink.last().imu_valid);
    TEST_ASSERT_TRUE(sink.last().env_valid);
    TEST_ASSERT_EQUAL_INT(0, hw.bus_reset_calls);           // recovery happens on the NEXT tick
}

static void test_row5_logoff_goes_idle_without_publishing(void)
{
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::LogOff);
    TEST_ASSERT_STATE(State::Idle, fsm);
    TEST_ASSERT_EQUAL_UINT(0, sink.count);
}

static void test_logon_while_logging_keeps_logging(void)
{
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::LogOn);
    TEST_ASSERT_STATE(State::Logging, fsm);
    TEST_ASSERT_EQUAL_UINT(0, sink.count);
}

// --- Idle: rows 8, 9 -------------------------------------------------------

static void test_row9_idle_tick_reads_nothing_publishes_nothing(void)
{
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::LogOff);
    const int reads_before = hw.imu_read_calls + hw.env_read_calls + hw.now_calls;
    fsm.handle(Event::Tick);
    fsm.handle(Event::Tick);
    TEST_ASSERT_STATE(State::Idle, fsm);
    TEST_ASSERT_EQUAL_UINT(0, sink.count);
    TEST_ASSERT_EQUAL_INT(reads_before, hw.imu_read_calls + hw.env_read_calls + hw.now_calls);
}

static void test_row8_idle_logon_resumes_logging(void)
{
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::LogOff);
    fsm.handle(Event::LogOn);
    TEST_ASSERT_STATE(State::Logging, fsm);
    fsm.handle(Event::Tick);
    TEST_ASSERT_EQUAL_UINT(1, sink.count);
}

static void test_idle_logoff_stays_idle(void)
{
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::LogOff);
    fsm.handle(Event::LogOff);
    TEST_ASSERT_STATE(State::Idle, fsm);
}

// --- Recovering: rows 6, 6b, 7 ---------------------------------------------

static void test_row6_recovery_success_goes_logging(void)
{
    hw.imu_read_script.set({BusStatus::Timeout, BusStatus::Ok});
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::Tick);                                // -> Recovering
    const int init_before = hw.imu_init_calls;

    fsm.handle(Event::Tick);                                // recovery tick
    TEST_ASSERT_STATE(State::Logging, fsm);
    TEST_ASSERT_EQUAL_INT(1, hw.bus_reset_calls);
    TEST_ASSERT_EQUAL_INT(init_before + 1, hw.imu_init_calls);
    TEST_ASSERT_EQUAL_UINT(2, sink.count);
    TEST_ASSERT_TRUE(sink.last().env_valid);                // BMP280 (SPI) still logged
    TEST_ASSERT_FALSE(sink.last().imu_valid);               // no IMU data on a recovery tick
}

static void test_recovering_never_reads_the_imu(void)
{
    hw.imu_init_script.set({BusStatus::Timeout});           // start -> Recovering, keeps failing
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::Tick);
    fsm.handle(Event::Tick);
    TEST_ASSERT_EQUAL_INT(0, hw.imu_read_calls);
}

static void test_row6b_reset_fails_skips_imu_init_and_stays(void)
{
    hw.imu_init_script.set({BusStatus::Timeout});           // start -> Recovering
    hw.bus_reset_script.set({BusStatus::Timeout});
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    const int init_before = hw.imu_init_calls;

    fsm.handle(Event::Tick);
    TEST_ASSERT_STATE(State::Recovering, fsm);
    TEST_ASSERT_EQUAL_INT(1, hw.bus_reset_calls);
    TEST_ASSERT_EQUAL_INT(init_before, hw.imu_init_calls);  // short-circuit: no init after failed reset
    TEST_ASSERT_EQUAL_UINT(1, sink.count);                  // env-only row still published
    TEST_ASSERT_FALSE(sink.last().imu_valid);
}

static void test_row6b_reset_ok_but_init_fails_stays(void)
{
    hw.imu_init_script.set({BusStatus::Timeout, BusStatus::Nack});
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();                                            // Timeout -> Recovering
    fsm.handle(Event::Tick);                                // reset Ok, init Nack
    TEST_ASSERT_STATE(State::Recovering, fsm);
    TEST_ASSERT_EQUAL_INT(1, hw.bus_reset_calls);
    TEST_ASSERT_EQUAL_INT(2, hw.imu_init_calls);
}

static void test_recovery_one_attempt_per_tick_until_success(void)
{
    hw.imu_init_script.set({BusStatus::Timeout});           // start fails
    hw.bus_reset_script.set({BusStatus::Timeout, BusStatus::Timeout, BusStatus::Ok});
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    hw.imu_init_script.set({BusStatus::Ok});                // from now on init works

    fsm.handle(Event::Tick);
    TEST_ASSERT_STATE(State::Recovering, fsm);
    fsm.handle(Event::Tick);
    TEST_ASSERT_STATE(State::Recovering, fsm);
    fsm.handle(Event::Tick);
    TEST_ASSERT_STATE(State::Logging, fsm);

    TEST_ASSERT_EQUAL_INT(3, hw.bus_reset_calls);           // exactly one per tick
    TEST_ASSERT_EQUAL_UINT(3, sink.count);                  // no gap in the CSV
}

static void test_row7_recovering_logoff_goes_idle(void)
{
    hw.imu_init_script.set({BusStatus::Timeout});
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::LogOff);
    TEST_ASSERT_STATE(State::Idle, fsm);
    TEST_ASSERT_EQUAL_INT(0, hw.bus_reset_calls);
}

static void test_logon_while_recovering_stays_recovering(void)
{
    // Jumping to Logging would skip the bus reset; the next read would time out again.
    hw.imu_init_script.set({BusStatus::Timeout});
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::LogOn);
    TEST_ASSERT_STATE(State::Recovering, fsm);
    TEST_ASSERT_EQUAL_INT(0, hw.bus_reset_calls);           // a command is not a recovery attempt
    TEST_ASSERT_EQUAL_UINT(0, sink.count);
}

// --- scenario --------------------------------------------------------------

static void test_scenario_timestamps_increase_across_states(void)
{
    hw.imu_read_script.set({BusStatus::Ok, BusStatus::Timeout, BusStatus::Ok});
    Out o; wire(o, sink);
    Fsm fsm{hw, o};
    fsm.start();
    fsm.handle(Event::Tick);   // Logging, ok         t=1000
    fsm.handle(Event::Tick);   // Logging, timeout    t=2000 -> Recovering
    fsm.handle(Event::Tick);   // Recovering, ok      t=3000 -> Logging
    fsm.handle(Event::Tick);   // Logging, ok         t=4000

    TEST_ASSERT_STATE(State::Logging, fsm);
    TEST_ASSERT_EQUAL_UINT(4, sink.count);
    for (std::size_t i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL_UINT32(1000u * (i + 1), sink.records[i].timestamp_ms);
    }
    TEST_ASSERT_TRUE(sink.records[0].imu_valid);
    TEST_ASSERT_FALSE(sink.records[1].imu_valid);
    TEST_ASSERT_FALSE(sink.records[2].imu_valid);
    TEST_ASSERT_TRUE(sink.records[3].imu_valid);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_initial_state_is_init_and_ignores_events);
    RUN_TEST(test_row1_start_imu_ok_goes_logging);
    RUN_TEST(test_row2_start_imu_fails_goes_recovering);
    RUN_TEST(test_start_twice_does_nothing_the_second_time);
    RUN_TEST(test_row3_tick_all_ok_publishes_full_record);
    RUN_TEST(test_row3_nack_marks_imu_invalid_and_stays_logging);
    RUN_TEST(test_row3_env_failure_marks_env_invalid_only);
    RUN_TEST(test_row4_timeout_publishes_then_goes_recovering);
    RUN_TEST(test_row5_logoff_goes_idle_without_publishing);
    RUN_TEST(test_logon_while_logging_keeps_logging);
    RUN_TEST(test_row9_idle_tick_reads_nothing_publishes_nothing);
    RUN_TEST(test_row8_idle_logon_resumes_logging);
    RUN_TEST(test_idle_logoff_stays_idle);
    RUN_TEST(test_row6_recovery_success_goes_logging);
    RUN_TEST(test_recovering_never_reads_the_imu);
    RUN_TEST(test_row6b_reset_fails_skips_imu_init_and_stays);
    RUN_TEST(test_row6b_reset_ok_but_init_fails_stays);
    RUN_TEST(test_recovery_one_attempt_per_tick_until_success);
    RUN_TEST(test_row7_recovering_logoff_goes_idle);
    RUN_TEST(test_logon_while_recovering_stays_recovering);
    RUN_TEST(test_scenario_timestamps_increase_across_states);
    return UNITY_END();
}