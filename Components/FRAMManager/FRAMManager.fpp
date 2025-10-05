module Components {
    @ Component for FRAM management
    active component FRAMManager {

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################
        async input port requestRead: Drv.FRAM.RequestRead
        async input port requestWrite: Drv.FRAM.RequestWrite
        async input port requestWakeup: Drv.FRAM.RequestWakeup
        async input port requestSleep: Drv.FRAM.RequestSleep
        output port status: Drv.FRAM.ExternalStatus
        
        output port reinit: Drv.FRAM.Reinit
        output port read: Drv.FRAM.Read
        output port write: Drv.FRAM.Write
        output port wakeup: Drv.FRAM.Wakeup
        output port sleep: Drv.FRAM.Sleep
        
        @ Command to reinit FRAM driver and peripheral it relies on (SPI)
        sync command REINIT()
        
        @ Event reports about failure during write operation
        event WriteFailed \
            severity warning high \
            format "Write error"

        @ Event reports about failure during read operation
        event ReadFailed \
            severity warning high \
            format "Read error"

        @ Event reports about failure during sleep mode activation
        event SleepFailed \
            severity warning high \
            format "Failed to activate sleep mode"

        @ Event reports about failure during wakeup operation
        event WakeupFailed \
            severity warning high \
            format "Failed to wakeup from sleep mode"

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

    }
}