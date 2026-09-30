#include <cstdint>
#include <cstdio>

enum class BleState : uint8_t {
    Idle, 
    Advertising, 
    Connecting, 
    Connected, 
    Disconnecting
};

enum class BleEvent : uint8_t {
    StartAdvertising, 
    ConnectionReq, 
    ConnectionAck, 
    TimeOut, 
    LinkLost, 
    Disconnect
};

class BleConnection {
    BleState state_ = BleState::Idle;
public:
    BleState current() const { 
        return state_; 
    }
    void handle_event(BleEvent ev) {
        switch (state_) {
            case BleState::Idle:
                if( ev == BleEvent::StartAdvertising ){
                    this->state_ = BleState::Advertising;
                }
                break;
            case BleState::Advertising:
                if( ev == BleEvent::ConnectionReq ){
                    this->state_ = BleState::Connecting;
                }
                break;  
            case BleState::Connecting:
                if( ev == BleEvent::ConnectionAck ){
                    this->state_ = BleState::Connected;
                }else if( ev == BleEvent::TimeOut ){
                    this->state_ = BleState::Disconnecting;
                }
                break;
            case BleState::Connected:
                if( ev == BleEvent::LinkLost ){
                    this->state_ = BleState::Disconnecting;
                }
                break;
            case BleState::Disconnecting:
                if( ev == BleEvent::Disconnect ){
                    this->state_ = BleState::Idle;
                }
                break;
            default:
                break;
        }
    }
};

int main() {
    BleConnection conn;
    // TODO: drive it through a sequence of events, check conn.current() after each
    conn.handle_event(BleEvent::StartAdvertising);
    printf("%d\n", conn.current());
    conn.handle_event(BleEvent::ConnectionReq);
    printf("%d\n", conn.current());
    conn.handle_event(BleEvent::ConnectionAck);
    printf("%d\n", conn.current());
    conn.handle_event(BleEvent::LinkLost);
    printf("%d\n", conn.current());
    conn.handle_event(BleEvent::Disconnect);
    printf("%d\n", conn.current());
    //deliberately wrong event
    conn.handle_event(BleEvent::ConnectionAck);
    printf("%d\n", conn.current());
}