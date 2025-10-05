module Components {
    @ Component to test time
    active component TimeTester {
        @ Port receiving calls from the rate group
        async input port run: Svc.Sched

        @ Port to set time
        output port setTime: Drv.Calendar.Set

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