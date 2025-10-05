module Components {
    @ Temperature sensor working through I2C
    passive component STS21 {

        @ Sensor port to retrieve last measured temperature
        sync input port getTemperature: Drv.Sensor.GetTemperature

        @ Communication error during data transmission
        event CommunicationError \
            severity warning high \
            format "Communication error"

        @ Sensor was wrongly configured
        event WrongResolutionConfiguration \
            severity warning high \
            format "Wrong resolution configuration"

        @ Device wasn't initialized
        event NotInited \
            severity warning high \
            format "Device wasn't initialized"

        @ Unknown error
        event UnknownError \
            severity warning high \
            format "Unknown error occurred"

        @ Communication error during data transmission
        event InvalidMeasureActivationArgument(badArgument: Fw.On) \
            severity warning low \
            format "Invalid measurement activation argument: {}"

        @ I2C port to write data
        output port write: Drv.I2C.Write

        @ I2C port to read data
        output port read: Drv.I2C.Read

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