module Drv {
  module ADC {
    enum Status {
      SUCCESS = 0 @< Operation success
      FAIL = 1    @< Operation failure
    }

    port GetVoltageValue(
                ref voltage: F32
                ) -> Drv.ADC.Status

    port GetVoltageReference() -> F32

    port GetADCCode(
                ref adc: U32
                ) -> Drv.ADC.Status

    port GetADCMax() -> U32
  
  }
}
