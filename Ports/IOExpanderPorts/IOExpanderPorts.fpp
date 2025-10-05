module Drv {
  module IOExpander {
    # Channels can be independently configured as either analog inputs, digital inputs,
    # digital outputs open-drain or push-pull (outputs low logic level by default)
    enum IOType {
        AnalogIn,
        DigitalIn,
        DigitalOutOD,
        DigitalOutPP
    };
      
    # Channels of the multiplexer (analog inputs or GPIOs)
    enum Channel {
        Channel_1     = 0
        Channel_2     = 1
        Channel_3     = 2
        Channel_4     = 3
        Channel_5     = 4
        Channel_6     = 5
        Channel_7     = 6
        Channel_8     = 7
        ChannelAmount = 8
    };

    struct Ports {
      Channel1: bool
      Channel2: bool
      Channel3: bool
      Channel4: bool

      Channel5: bool
      Channel6: bool
      Channel7: bool
      Channel8: bool
    }

    struct PortsStates {
      Channel1: Fw.Logic
      Channel2: Fw.Logic
      Channel3: Fw.Logic
      Channel4: Fw.Logic
      
      Channel5: Fw.Logic
      Channel6: Fw.Logic
      Channel7: Fw.Logic
      Channel8: Fw.Logic
    }

    struct ThresholdsHitCounter {
      Min: I32
      Max: I32
    }

    struct DigitalSingleOutputData {
      Channel: Drv.IOExpander.Channel
      State: Fw.Logic
    }

    struct DigitalMultipleOutputsData {
      Ports: Drv.IOExpander.Ports @< Ports to work with
      States: PortsStates         @< Ports states to be set
    }

    struct DigitalSingleInputData {
      State: Fw.Logic
      IsValid: bool
    }

    struct DigitalMultipleInputsData {
      States: PortsStates         @< Ports states
      IsValid: bool
    }

    struct ReadAnaloglInputData {
      AdcCode: U16
      VoltageValue: F32
      IsValid: bool
    }

    struct NumOfThresholdHitData {
      ThresholdsHitCounter: ThresholdsHitCounter
      IsValid: bool
    }

    port WriteDigitalSingleOutput(
                                  data: Drv.IOExpander.DigitalSingleOutputData
                                  ref IsValid: bool
                                  )

    port WriteDigitalMultipleOutputs(
                                    data: Drv.IOExpander.DigitalMultipleOutputsData
                                    ref IsValid: bool
                                    )

    port ReadDigitalSingleInput(
                                channel: Drv.IOExpander.Channel
                                ref data: DigitalSingleInputData
                                )

    port ReadDigitalMultipleInputs(
                                  ports: Drv.IOExpander.Ports @< Ports to work with
                                  ref data: DigitalMultipleInputsData
                                  )

    port ReadAnaloglInput(
                          channel: Drv.IOExpander.Channel  
                          ref data: ReadAnaloglInputData
                          )

    port GetNumOfThresholdHit(
                              channel: Drv.IOExpander.Channel  
                              ref data: NumOfThresholdHitData
                              )

    port CalibrateADCOffset()                          
  }
}
