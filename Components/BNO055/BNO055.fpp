module Components {
    @ Component to work with IMU sensor BNO055
    passive component BNO055 {
        sync input port getAccelData: Drv.IMU.GetAccelData
        sync input port getGyroData:  Drv.IMU.GetGyroData
        sync input port getMagnData:  Drv.IMU.GetMagnData
        sync input port getRawData:   Drv.IMU.GetRawData

        sync input port getQuatData:  Drv.IMU.GetQuatData

        @ Write data over UART
        output port write: Drv.UART.Write

        @ Read data over UART
        output port read: Drv.UART.Read

        @ Get number of bytes available for read
        output port getNumOfBytes: Drv.UART.GetNumOfBytes

        @ Event reports about read fail error
        event ReadFailError \
            severity warning high \
            format "Read fail error occurred"

        @ Event reports about write fail error
        event WriteFailError \
            severity warning high \
            format "Write fail error occurred"

        @ Event reports about regmap invalid address error
        event RegmapInvalidAddressError \
            severity warning high \
            format "Regmap invalid address error occurred"    

        @ Event reports about regmap write disabled error
        event RegmapWriteDisabledError \
            severity warning high \
            format "Regmap write disabled error occurred"    
        
        @ Event reports about wrong start byte error
        event WrongStartByteError \
            severity warning high \
            format "Wrong start byte error occurred"    

        @ Event reports about bus overrun error
        event BusOverrunError(cnt: U32) \
            severity warning high \
            format "Bus overrun error occurred: {}"

        @ Event reports about max length error
        event MaxLengthError \
            severity warning high \
            format "Max length error occurred"

        @ Event reports about min length error
        event MinLengthError \
            severity warning high \
            format "Min length error occurred"

        @ Event reports about min length error
        event ReceiveCharacterTimeoutError \
            severity warning high \
            format "Receive character timeout error occurred"

        @ Event reports about response header mismatch error
        event ResponseHeaderMismatchError \
            severity warning high \
            format "Response header mismatch error occurred"
        
        @ Event reports about response size mismatch error
        event ResponseSizeMismatchError \
            severity warning high \
            format "Response size mismatch error occurred"

        @ Event reports about receive fail error
        event ReceiveFailError \
            severity warning high \
            format "Receive fail error occurred"

        @ Event reports about send fail error
        event SendFailError \
            severity warning high \
            format "Send fail error occurred" 

        @ Event reports about buffer clear fail error
        event BufferClearFailError \
            severity warning high \
            format "Buffer clear fail error occurred"

        @ Event reports about reset fail error
        event ResetFailError \
            severity warning high \
            format "Reset fail error occurred"

        @ Event reports that device wasn't initialized
        event NotInited \
            severity warning high \
            format "Device wasn't initialized" 

        @ Event reports about reset fail error
        event SelfTestError \
            severity warning high \
            format "Self test error occurred"

        @ Event reports that calibration finished
        event CalibrationUnfinished \
            severity warning low \
            format "Calibration is unfinished"

        @ Event reports that calibration finished
        event CalibrationFinished \
            severity activity high \
            format "Calibration is finished"

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