module Components {
    @ IO Expander ADS7138IRTER
    active component IOExpander {
        @ Receiving calls from the rate group
        async input port run: Svc.Sched
        
        guarded input port writeDigitalSingleOutput: Drv.IOExpander.WriteDigitalSingleOutput

        guarded input port readDigitalSingleInput: Drv.IOExpander.ReadDigitalSingleInput

        guarded input port writeDigitalMultipleOutputs: Drv.IOExpander.WriteDigitalMultipleOutputs

        guarded input port readDigitalMultipleInputs: Drv.IOExpander.ReadDigitalMultipleInputs

        guarded input port readAnalogInput: Drv.IOExpander.ReadAnaloglInput

        guarded input port getNumOfThresholdHit: Drv.IOExpander.GetNumOfThresholdHit

        guarded input port calibrateADCOffset: Drv.IOExpander.CalibrateADCOffset

        output port write: Drv.I2C.Write

        output port read: Drv.I2C.Read

        output port writeRead: Drv.I2C.WriteRead

        @ Port sending calls to the GPIO driver
        output port alert: Drv.GpioRead

        @ Device wasn't initialized
        event NotInited \
            severity warning high \
            format "Device wasn't initialized"

        @ Sensor was wrongly configured
        event WrongChannelConfiguration \
            severity warning high \
            format "Wrong channel configuration"

        @ Communication error during data transmission
        event CommunicationError \
            severity warning high \
            format "Communication error"

        @ CRC error check failed
        event CRCError \
            severity warning high \
            format "CRC error"

        @ Calibration unfinished
        event CalibrationUnfinishedError \
            severity warning high \
            format "Calibration unfinished error"

        @ Unknown error
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