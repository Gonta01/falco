module Components {
    @ Component for SBC diagnostics
    active component EnvSensManager {
        
        async input port run: Svc.Sched
        
        @ Ports to get DPS310 data
        output port getTemperatureDPS310:   Drv.Sensor.GetTemperature
        output port getPressuretDPS310:      Drv.Sensor.GetPressure
        
        @ Port to get STS21 temperature
        output port getTemperatureSTS21:   Drv.Sensor.GetTemperature

        @ Port to get NTC temperature
        output port getTemperatureTableNTC:   Drv.Sensor.GetTemperature
        output port getTemperaturePolynomNTC:   Drv.Sensor.GetTemperature

        @ Port to send sensor data
        guarded input port getSensorData: Drv.SensorManager.GetSensorData

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

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

    }
}