module Components {
    @ Component to test File functionality
    active component FileTester {

        @ Receiving calls from the rate group
        async input port run: Svc.Sched

        @ File system test failed
        event TestFail() \
            severity warning high \
            format "File System test failed."

        @ File system test failed
        event NotInit() \
            severity warning high \
            format "File System failed to init."

        @ File system test failed
        event InitSuccess() \
            severity activity high \
            format "File System inited."

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