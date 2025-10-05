module Falco {
    
    passive component I2c2Driver {

        guarded input port write: Drv.I2C.Write

        guarded input port read: Drv.I2C.Read

        guarded input port writeRead: Drv.I2C.WriteRead
        
        @ Write error
        event WriteError() \
            severity warning high \
            format "Write error"
        
        @ Read error
        event ReadError() \
            severity warning high \
            format "Read error"

        @ WriteRead error
        event WriteReadError() \
            severity warning high \
            format "WriteRead error"

        @ Timeout error
        event TimeoutError() \
            severity warning high \
            format "Timeout error"
        
        @ NAK error
        event NAKError() \
            severity warning high \
            format "NAK error"

        @ Bus error
        event BusError() \
            severity warning high \
            format "Bus error" 

        @ Unknown error
        event UnknownError() \
            severity warning high \
            format "Unknown error" 

        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut
    }
}
