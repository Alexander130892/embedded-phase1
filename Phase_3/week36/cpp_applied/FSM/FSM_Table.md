STATE       -   EVENT           -        TRANSITION
IDLE        -   StartAdvertising-       ADVERTISING
ADVERTISING -   ConnectionReq   -       CONNECTING
CONNECTING  -   ConnectionACK   -       CONNECTED  
CONNECTING  -   TimeOut         -       DISCONNECTING  
CONNECTED   -   LinkLost        -       DISCONNECTING
DISCONNECTING -     Disconnect  -       IDLE