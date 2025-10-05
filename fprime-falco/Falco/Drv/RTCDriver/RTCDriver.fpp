module Falco {
    @ RTC driver
    passive component RTCDriver {

        @ Port to set time
        sync input port setTime: Drv.Calendar.Set
        
        @ Port to set time
        sync input port getTime: Drv.Calendar.Get

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

    }
}