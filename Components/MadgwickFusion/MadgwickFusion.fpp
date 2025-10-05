module Components {
    @ Component to receive orientation based on raw data from IMU
    active component MadgwickFusion {

        @ Receiving calls from the rate group
        async input port run:        Svc.Sched

        @ Receiving IMU data
        output port getIMUData: Drv.IMUManager.GetData

        @ Write data over UART
        output port write: Drv.UART.Write

        @ Read data over UART
        output port read: Drv.UART.Read
        
        @ Obtained data if invalid
        event DataInvalid() \
            severity warning high \
            format "Data invalid."

        @ Data send timeout
        event DataSendTimeout() \
            severity warning high \
            format "Data send timeout."

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