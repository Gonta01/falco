module Components {
    @ Component to receive temperature measurements from NTC sensor
    passive component NTC {
        sync input port getTableTemperature: Drv.Sensor.GetTemperature
        sync input port getPolynomTemperature: Drv.Sensor.GetTemperature
        
        output port getVoltageValue: Drv.ADC.GetVoltageValue

        output port getVoltageReference: Drv.ADC.GetVoltageReference

        output port getADCCode: Drv.ADC.GetADCCode

        output port getADCMax: Drv.ADC.GetADCMax

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