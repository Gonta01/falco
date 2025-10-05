module Components {
    @ IMU sensor
    passive component ICM20948 {
        sync input port getCalibrationData: Drv.IMU.GetCalibrationData

        sync input port getAccelData:     Drv.IMU.GetAccelData
        sync input port getGyroData:      Drv.IMU.GetGyroData
        sync input port getMagnData:      Drv.IMU.GetMagnData
        sync input port pollRawData:   Drv.IMU.PollData

        sync input port getQuatData:      Drv.IMU.GetQuatData
        sync input port pollQuatData:  Drv.IMU.PollData
        
        @ Port to track interrupts
        output port intPin: Drv.GpioRead

        @ I2C port to write data
        output port write: Drv.I2C.Write

        @ I2C port to read data
        output port read: Drv.I2C.Read

        @ Event reports that device wasn't initialized
        event NotInited \
            severity warning high \
            format "Device wasn't initialized"

        @ Event reports about failure during write operation to device
        event FailedWriteToIMURegister \
            severity warning high \
            format "Failed write to device registers"

        @ Event reports about failure during read operation from device
        event FailedReadIMURegister \
            severity warning high \
            format "Failed read from device registers"
        
        @ Event reports about failure during read operation from device
        event ReadNotEqualTOWrittenRegister \
            severity warning high \
            format "Read-write registers check failed"

        @ Event reports about unkonwn error occurrence
        event UnknownError \
            severity warning high \
            format "Unknown error occurred"

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