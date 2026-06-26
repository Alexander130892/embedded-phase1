#include <cstdint>

constexpr std::uint32_t c_max_baudrate = 115200;

class uart{
public:
    uart(std::uint32_t baud = 9600): baudrate_(baud){;}
    void init(){
        write_brr(calculate_uartdiv());
    }
    std::uint32_t get_baudrate() const{
        return baudrate_;
    }
    void set_baudrate(std::uint32_t baudrate){
        if(baudrate <= c_max_baudrate){
            baudrate_ = baudrate;
        }
        else{
            baudrate_ = c_max_baudrate;
        }
    }
private:
    std::uint32_t baudrate_;
    std::uint8_t calculate_uartdiv(){
        return baudrate_/32000;
    }
    void write_brr(std::uint8_t){}
};

//Alternatively
// class uart {
//     std::uint32_t baudrate_;
//     std::uint8_t calculate_uartdiv();
//     void write_brr(std::uint8_t);
// public:
//     uart(std::uint32_t baud = 9600);
//     void init();
// };

class sensor{
public:
    sensor(uart &u, std::uint32_t read_interval):
        uart_(u),
        read_interval_(read_interval){}
private:
    uart &uart_;
    const std::uint32_t read_interval_;
};

int main(){
    uart uart1(115200);
    sensor sensor1(uart1, 500);
    return 0;
}