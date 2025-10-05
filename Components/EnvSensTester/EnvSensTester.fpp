module Components {
    @ Component for EnvSensManager testing
    active component EnvSensTester {
        @ Polling for receiving data
        async input port run: Svc.Sched

        @ Port to get sensor data
        output port getSensorData: Drv.SensorManager.GetSensorData

        @ Telemetry channel to report DPS310 data
        telemetry TemperatureDPS310: F32
        telemetry PressureDPS310:    F32

        @ Telemetry channels to reports STS21 temperature
        telemetry TemperatureSTS21: F32

        @ Telemetry channel of NTC temperature measurements
        telemetry TemperatureNTC: F32

        @ Telemetry channel of average temperature
        telemetry TemperatureAverage: F32

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