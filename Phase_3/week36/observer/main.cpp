#include <cstdio>
#include <cstdint>
#include <array>

struct Bmp280Data {
    float temperature_c;
    float pressure_hpa;
};

class ISensorObserver {
public:
    virtual void on_data(const Bmp280Data& data) = 0;
    virtual ~ISensorObserver() = default;
};

class UartLogger : public ISensorObserver {
public:
    void on_data(const Bmp280Data& data) override{
        printf("[UART] Temp: %.1fC, Pressure: %.1fhPa\n", data.temperature_c, data.pressure_hpa);
    }   
};

class LedIndicator : public ISensorObserver {
public:
    void on_data(const Bmp280Data& data) override{
        if(data.temperature_c > 2){
            printf("[LED] LED ON: %.1fC\n", data.temperature_c); 
        }else{
            printf("[LED] LED OFF: %.1fC\n", data.temperature_c); 
        }
    }   
};

class Bmp280 {
    static constexpr size_t MaxObservers = 4;  // Arbitrary number as exercise
    std::array<ISensorObserver*, MaxObservers> observers_{};
    size_t count_ = 0;

public:
    bool attach(ISensorObserver* obs){
        if(this->count_ < MaxObservers){
            this->observers_[this->count_] = obs;
            this->count_++;
            return true;
        }else return false;
    }   
    // TODO — produces a Bmp280Data (stub it, e.g. static/incrementing fake values), notifies every attached observer
    void poll(){
        //  Fake/update data
        static Bmp280Data fake_data={0,0};
        fake_data.temperature_c++;
        fake_data.pressure_hpa++;
        //  Notify all Observers
        for (size_t i = 0; i < count_; ++i) {
            observers_[i]->on_data(fake_data);
        }
    }                   
};

int main() {
    // TODO: construct a Bmp280, attach a UartLogger and a LedIndicator, call poll() a few times
    Bmp280          sensor;
    UartLogger      uart;
    LedIndicator    led;

    sensor.attach(&uart);
    sensor.attach(&led);

    for(int i = 0; i < 5; i++){
        sensor.poll();
    }
}