module Components {
    @ Component for IMU calibration
    active component Calibration {

        @ Receiving calls from the rate group
        async input port run:        Svc.Sched

        @ Receiving IMU data
        output port getData: Drv.IMUManager.GetData

        @ Write data over UART
        output port write: Drv.UART.Write

        @ Read data over UART
        output port read: Drv.UART.Read
        
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