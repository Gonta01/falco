module Components {
    @ External battery manager
    active component ExtBatManager {

        @ Port for receiving calls from the rate group
        async input port run: Svc.Sched

        @ Port to provide all voltage data: instantaneous voltage, average voltage, validity flag, overvoltage flag, undervoltage flag
        guarded input port getVoltage: Drv.ExtBat.GetVoltage

        @ Port to retrieve ADC voltage
        output port getVoltageValue: Drv.ADC.GetVoltageValue

        @ Port to enable subsystem for measurements, and disable subsystem after measurements done
        output port enablePin: Drv.GpioWrite

        @ Telemetry channel that represents instantaneous voltage
        telemetry InstantaneousVoltage: F32

        @ Telemetry channel that represents average voltage
        telemetry AverageVoltage: F32

        @ Telemetry channel to indicate whether the voltage is valid
        telemetry IsValid: bool

        @ Telemetry channel to indicate undervoltage
        telemetry UndervoltageAlarm: bool

        @ Telemetry channel to indicate overvoltage
        telemetry OvervoltageAlarm: bool

        @ During initialization not all measurements were successful
        event NotFullInit() \
            severity warning low \
            format "Not full initialization."

        @ Error during ADC measurement
        event MeasurementError() \
            severity warning low \
            format "Error during measurement."

        @ Voltage data invalid because of ADC measurement error or undervoltage or overvoltage
        event VoltageInvalid() \
            severity warning high \
            format "Voltage data invalid."

        @ Measured instantaneous voltage is over set maximum value
        event OvervoltageAlarm() \
            severity warning high \
            format "Overvoltage alarm."    

        @ Measured instantaneous voltage is under set maximum value
        event UndervoltageAlarm() \
            severity warning high \
            format "Undervoltage alarm." 

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