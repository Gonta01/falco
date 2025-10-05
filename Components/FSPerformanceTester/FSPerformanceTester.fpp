module Components {
    @ Component to test FS performance
    active component FSPerformanceTester {
        
        @ Receiving calls from the rate group
        async input port run: Svc.Sched

        output port getTime: Drv.Calendar.Get
        
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