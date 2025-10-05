module Falco {
    
    passive component Adc0Driver {

        guarded input port getVoltageValue: Drv.ADC.GetVoltageValue

        guarded input port getVoltageReference: Drv.ADC.GetVoltageReference

        guarded input port getADCCode: Drv.ADC.GetADCCode
        guarded input port getADCMax: Drv.ADC.GetADCMax
    }
}
