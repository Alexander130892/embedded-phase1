#include <cstdio>
#include <cstdint>

enum class BleEvent : uint8_t {
    StartAdvertising, ConnectionReq, ConnectionAck, TimeOut, LinkLost, Disconnect
};

class BleState {
public:
    virtual BleState* on_event(BleEvent ev) = 0;
    virtual const char* name() const = 0;   
    virtual ~BleState() = default;
};

class Idle : public BleState {
public:
    BleState* on_event(BleEvent ev) override;
    const char* name() const override { 
        return "Idle"; 
    }
};
class Advertising : public BleState {
public:
    BleState* on_event(BleEvent ev) override;
    const char* name() const override { 
        return "Advertising"; 
    }
};
class Connecting : public BleState {
public:
    BleState* on_event(BleEvent ev) override;
    const char* name() const override { 
        return "Connecting"; 
    }
};
class Connected : public BleState {
public:
    BleState* on_event(BleEvent ev) override;
    const char* name() const override { 
        return "Connected"; 
    }
};
class Disconnecting : public BleState {
public:
    BleState* on_event(BleEvent ev) override;
    const char* name() const override { 
        return "Disconnecting"; 
    }
};

static Idle            idleState;
static Advertising     advertisingState;
static Connecting      connectingState;
static Connected       connectedState;
static Disconnecting   disconnectingState;

// Idle::on_event's body, and the other four — defined out-of-line since they need
// to reference other states' instances, which must all be declared first

BleState* Idle::on_event(BleEvent ev){
    if(ev == BleEvent::StartAdvertising){
        return &advertisingState;
    }
    return this;
}
BleState* Advertising::on_event(BleEvent ev){
    if(ev == BleEvent::ConnectionReq){
        return &connectingState;
    }
    return this;
}
BleState* Connecting::on_event(BleEvent ev){
    if(ev == BleEvent::ConnectionAck){
        return &connectedState;
    }else if(ev == BleEvent::TimeOut){
        return &disconnectingState;
    }
    return this;
}
BleState* Connected::on_event(BleEvent ev){
    if(ev == BleEvent::LinkLost){
        return &disconnectingState;
    }
    return this;
}
BleState* Disconnecting::on_event(BleEvent ev){
    if(ev == BleEvent::Disconnect){
        return &idleState;
    }
    return this;
}


class BleConnection {
    BleState* current_;
public:
    BleConnection() : current_(&idleState) {}   
    void handle_event(BleEvent ev) { 
        current_ = current_->on_event(ev); 
    }
    const char* current_name() const { 
        return current_->name(); 
    }
};

int main() {
    // TODO: same test sequence as fsm_switch, using current_name() instead of a cast int
    BleConnection conn;
    // TODO: drive it through a sequence of events, check conn.current() after each
    conn.handle_event(BleEvent::StartAdvertising);
    printf("%s\n", conn.current_name());
    conn.handle_event(BleEvent::ConnectionReq);
    printf("%s\n", conn.current_name());
    conn.handle_event(BleEvent::ConnectionAck);
    printf("%s\n", conn.current_name());
    conn.handle_event(BleEvent::LinkLost);
    printf("%s\n", conn.current_name());
    conn.handle_event(BleEvent::Disconnect);
    printf("%s\n", conn.current_name());
    //deliberately wrong event
    conn.handle_event(BleEvent::ConnectionAck);
    printf("%s\n", conn.current_name());

}