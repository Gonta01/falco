module Components {
    @ Component to display data on terminal
    active component Terminal {
        @ Receiving calls from the rate group
        async input port run: Svc.Sched

        @ Write data over UART
        output port write: Drv.UART.Write
        
        @ Port to send sensor data
        output port getSensorData: Drv.SensorManager.GetSensorData

        @ Port to send sensor data
        output port getSingleADCCode: Drv.ADC.GetADCCode

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

    }
}