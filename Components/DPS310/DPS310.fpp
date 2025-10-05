module Components {
    @ Component for barometric pressure sensor DPS310
    passive component DPS310 {
        @ Sensor port to retrieve last measured temperature
        guarded input port getTemperature: Drv.Sensor.GetTemperature

        @ Sensor port to retrieve last measured pressure
        guarded input port getPressure: Drv.Sensor.GetPressure

        @ Event reports that device wasn't initialized
        event NotInited \
            severity warning high \
            format "Device wasn't initialized"

        @ Event reports about measurement timeout
        event MeasurementTimeout \
            severity warning high \
            format "Measurement timeout"

        @ Event reports about failure during write operation to device
        event FailedWriteToRegsiter \
            severity warning high \
            format "Failed write to device registers"

        @ Event reports about failure during read operation from device
        event FailedReadFromRegsiter \
            severity warning high \
            format "Failed read from device registers"

        @ Event reports about unkonwn error occurrence
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

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}