module Components {
    @ Component for CAN testing
    active component CANTester {

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################
        @ Receiving calls from the rate group
        async input port run: Svc.Sched

        @ Port for transmiting CAN data
        output port write: Drv.CAN.StandardTransmit

        @ Port for receiving CAN data
        output port read:  Drv.CAN.StandardReceive

        @ Event reports that write error occured
        event WriteError \
            severity warning high \
            format "Write error!"
        
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