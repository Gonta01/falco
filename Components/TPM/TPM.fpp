module Components {
    @ Component to interact with Trusted Platform Module
    active component TPM {

        @ Receiving calls from the rate group
        async input port run: Svc.Sched

        @ I2C port to write data
        output port write: Drv.I2C.Write

        @ I2C port to read data
        output port read: Drv.I2C.Read
        
        @ Communication error during data transmission
        event CommunicationError \
            severity warning high \
            format "Communication error"
            
        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

    }
}