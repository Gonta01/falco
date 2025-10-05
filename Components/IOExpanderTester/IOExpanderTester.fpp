module Components {
    @ IOExpander testing component
    active component IOExpanderTester {
        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################
        @ Port for receiving calls from the rate group
        async input port run: Svc.Sched

        output port writeDigitalSingleOutput: Drv.IOExpander.WriteDigitalSingleOutput

        output port readDigitalSingleInput: Drv.IOExpander.ReadDigitalSingleInput

        output port writeDigitalMultipleOutputs: Drv.IOExpander.WriteDigitalMultipleOutputs

        output port readDigitalMultipleInputs: Drv.IOExpander.ReadDigitalMultipleInputs

        output port readAnalogInput1: Drv.IOExpander.ReadAnaloglInput

        output port readAnalogInput2: Drv.IOExpander.ReadAnaloglInput

        output port getNumOfThresholdHit1: Drv.IOExpander.GetNumOfThresholdHit

        output port getNumOfThresholdHit2: Drv.IOExpander.GetNumOfThresholdHit

        output port calibrateADCOffset1: Drv.IOExpander.CalibrateADCOffset

        output port calibrateADCOffset2: Drv.IOExpander.CalibrateADCOffset

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

        @ Test error, set value is not equal to read value
        event TestError \
            severity warning high \
            format "Test error occurred"

        @ Unknown error
        event UnknownError \
            severity warning high \
            format "Unknown error occurred"

        @ Telemetry channels to report Digital Inputs state
        telemetry IOExpander1_DigitalInput1: Fw.Logic
        telemetry IOExpander1_DigitalInput2: Fw.Logic
        telemetry IOExpander1_DigitalInput3: Fw.Logic
        telemetry IOExpander1_DigitalInput4: Fw.Logic
        telemetry IOExpander1_DigitalInput5: Fw.Logic
        telemetry IOExpander1_DigitalInput6: Fw.Logic
        telemetry IOExpander1_DigitalInput7: Fw.Logic
        telemetry IOExpander1_DigitalInput8: Fw.Logic
        
        @ Telemetry channels to report Analog Inputs state
        telemetry IOExpander1_AICode1: U16
        telemetry IOExpander1_AICode2: U16
        telemetry IOExpander1_AICode3: U16
        telemetry IOExpander1_AICode4: U16
        telemetry IOExpander1_AICode5: U16
        telemetry IOExpander1_AICode6: U16
        telemetry IOExpander1_AICode7: U16
        telemetry IOExpander1_AICode8: U16

        telemetry IOExpander1_AIVoltage1: F32
        telemetry IOExpander1_AIVoltage2: F32
        telemetry IOExpander1_AIVoltage3: F32
        telemetry IOExpander1_AIVoltage4: F32
        telemetry IOExpander1_AIVoltage5: F32
        telemetry IOExpander1_AIVoltage6: F32
        telemetry IOExpander1_AIVoltage7: F32
        telemetry IOExpander1_AIVoltage8: F32

        @ Digital output parameters
        param IOEXPANDER1_DIGITALOUTPUT1: Fw.Logic
        param IOEXPANDER1_DIGITALOUTPUT2: Fw.Logic
        param IOEXPANDER1_DIGITALOUTPUT3: Fw.Logic
        param IOEXPANDER1_DIGITALOUTPUT4: Fw.Logic
        param IOEXPANDER1_DIGITALOUTPUT5: Fw.Logic
        param IOEXPANDER1_DIGITALOUTPUT6: Fw.Logic
        param IOEXPANDER1_DIGITALOUTPUT7: Fw.Logic
        param IOEXPANDER1_DIGITALOUTPUT8: Fw.Logic

        @ Telemetry channels to report Digital Inputs state
        telemetry IOExpander2_DigitalInput1: Fw.Logic
        telemetry IOExpander2_DigitalInput2: Fw.Logic
        telemetry IOExpander2_DigitalInput3: Fw.Logic
        telemetry IOExpander2_DigitalInput4: Fw.Logic
        telemetry IOExpander2_DigitalInput5: Fw.Logic
        telemetry IOExpander2_DigitalInput6: Fw.Logic
        telemetry IOExpander2_DigitalInput7: Fw.Logic
        telemetry IOExpander2_DigitalInput8: Fw.Logic
        
        @ Telemetry channels to report Analog Inputs state
        telemetry IOExpander2_AICode1: U16
        telemetry IOExpander2_AICode2: U16
        telemetry IOExpander2_AICode3: U16
        telemetry IOExpander2_AICode4: U16
        telemetry IOExpander2_AICode5: U16
        telemetry IOExpander2_AICode6: U16
        telemetry IOExpander2_AICode7: U16
        telemetry IOExpander2_AICode8: U16

        telemetry IOExpander2_AIVoltage1: F32
        telemetry IOExpander2_AIVoltage2: F32
        telemetry IOExpander2_AIVoltage3: F32
        telemetry IOExpander2_AIVoltage4: F32
        telemetry IOExpander2_AIVoltage5: F32
        telemetry IOExpander2_AIVoltage6: F32
        telemetry IOExpander2_AIVoltage7: F32
        telemetry IOExpander2_AIVoltage8: F32

        @ Digital output parameters
        param IOEXPANDER2_DIGITALOUTPUT1: Fw.Logic
        param IOEXPANDER2_DIGITALOUTPUT2: Fw.Logic
        param IOEXPANDER2_DIGITALOUTPUT3: Fw.Logic
        param IOEXPANDER2_DIGITALOUTPUT4: Fw.Logic
        param IOEXPANDER2_DIGITALOUTPUT5: Fw.Logic
        param IOEXPANDER2_DIGITALOUTPUT6: Fw.Logic
        param IOEXPANDER2_DIGITALOUTPUT7: Fw.Logic
        param IOEXPANDER2_DIGITALOUTPUT8: Fw.Logic
        
        telemetry IOExpander1_AI3_maxThshldCnt: U32
        telemetry IOExpander1_AI3_minThshldCnt: U32

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}