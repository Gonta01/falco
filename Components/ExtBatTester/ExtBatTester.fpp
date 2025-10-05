module Components {
    @ External battery tester
    active component ExtBatTester {

        async input port run: Svc.Sched

        output port getVoltage: Drv.ExtBat.GetVoltage

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

    }
}