module Components {
    @ Testing component
    active component RS485Tester {
        @ Port for receiving calls from the rate group
        async input port run: Svc.Sched

        @ Transmit data
        output port transmit: Drv.ByteStreamSend

        @ Receive data
        output port receive: Drv.ByteStreamPoll

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

    }
}