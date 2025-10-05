module Drv {
  module IMU {
    enum Status : U8 {
      SUCCESS = 0
      FAILED  = 1
    }
    
    struct CalibrationData {
      AccelX: I16
      AccelY: I16
      AccelZ: I16

      GyroX: I16
      GyroY: I16
      GyroZ: I16

      MagnX: I16
      MagnY: I16
      MagnZ: I16
    }

    struct AccelData {
      X: F32
      Y: F32
      Z: F32
    }

    struct GyroData {
      X: F32
      Y: F32
      Z: F32
    }

    struct MagnData {
      X: F32
      Y: F32
      Z: F32
    }

    struct RawData {
      accelData: Drv.IMU.AccelData
      gyroData: Drv.IMU.GyroData
      magnData: Drv.IMU.MagnData
    }

    struct QuatData {
      Q0: F32
      Q1: F32
      Q2: F32
      Q3: F32
    }

    struct Data {
      Accel : AccelData
      AccelValid: bool

      Gyro : GyroData
      GyroValid: bool

      Magn : MagnData
      MagnValid: bool

      Calibration: CalibrationData
      CalibrationValid: bool
    }
    
    struct Quaternion {
      Quat: QuatData
      QuatValid: bool
    }

    port GetAccelData(ref accelData: AccelData)  -> Drv.IMU.Status
    port GetGyroData(ref gyroData:   GyroData)   -> Drv.IMU.Status
    port GetMagnData(ref magnData:   MagnData)   -> Drv.IMU.Status
    port GetRawData(ref rawData:   RawData)   -> Drv.IMU.Status
    port GetCalibrationData(ref calibData: CalibrationData) -> Drv.IMU.Status
    port PollData() -> Drv.IMU.Status

    port GetQuatData(ref quatData: QuatData) -> Drv.IMU.Status
  }

  module IMUManager {
    port GetData(ref data: Drv.IMU.Data)
    port GetQuaternion(ref quat: Drv.IMU.Quaternion)
  }
}
