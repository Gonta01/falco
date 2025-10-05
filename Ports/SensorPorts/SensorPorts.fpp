module Drv {
  module Sensor {
    enum Status : U8 {
      SUCCESS = 0
      FAILED  = 1
    }

    port GetTemperature(ref temperature: F32) -> Status

    port GetPressure(ref pressure: F32) -> Status

  }

  module SensorManager {
    struct SensorData {
      TemperatureDPS310: F32
      TemperatureDPS310Valid: bool

      TemperatureSTS21: F32
      TemperatureSTS21Valid: bool

      TemperatureTableNTC: F32
      TemperaturePolyNTC : F32
      TemperatureNTCValid: bool

      PressureDPS310: F32
      PressureDPS310Valid: bool

      TemperatureAverage: F32
      TemperatureAverageValid: bool
    }

    port GetSensorData(ref sensorData: SensorData)

  }
}
