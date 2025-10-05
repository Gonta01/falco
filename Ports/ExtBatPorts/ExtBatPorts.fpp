module Drv {
  module ExtBat {
    struct VoltageData {
      AverageVoltage:       F32
      InstantaneousVoltage: F32
      IsValid             : bool
      UndervoltageAlarm   : bool
      OvervoltageAlarm    : bool
    }

    port GetVoltage(
                       ref voltageData: Drv.ExtBat.VoltageData
                       )
  }
}
