module Components {
    @ Component for RTC management
    active component RTCManager {

        @ Command to set time
        async command SET_TIME(
            timeValue: U64
        )

        @ Port to periodically perform internal logic
        async input port run: Svc.Sched

        @ Port to set time
        output port setTime: Drv.Calendar.Set
        
        @ Port to set time
        output port getTime: Drv.Calendar.Get
        
        @ Input ping port
        async input port PingIn: Svc.Ping

        @ Output ping port
        output port PingOut: Svc.Ping

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