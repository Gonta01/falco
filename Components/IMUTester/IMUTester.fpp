module Components {
    @ Component for IMUTesting
    active component IMUTester {

        @ Receiving calls from the rate group
        async input port run: Svc.Sched

        output port getICM20948Data:       Drv.IMUManager.GetData
        output port getICM20948Quaternion: Drv.IMUManager.GetQuaternion

        output port getBNO055Data:         Drv.IMUManager.GetData
        output port getBNO055Quaternion:   Drv.IMUManager.GetQuaternion

        telemetry ICM20948_Accel_X: F32
        telemetry ICM20948_Accel_Y: F32
        telemetry ICM20948_Accel_Z: F32
        
        telemetry ICM20948_Gyro_X: F32
        telemetry ICM20948_Gyro_Y: F32
        telemetry ICM20948_Gyro_Z: F32

        telemetry ICM20948_Magnet_X: F32
        telemetry ICM20948_Magnet_Y: F32
        telemetry ICM20948_Magnet_Z: F32

        telemetry ICM20948_Q0: F64
        telemetry ICM20948_Q1: F64
        telemetry ICM20948_Q2: F64
        telemetry ICM20948_Q3: F64

        @ Euler angles: Heading/Yaw, Roll, Pitch
        telemetry BNO055_Heading: F32
        telemetry BNO055_Roll: F32
        telemetry BNO055_Pitch: F32
        
        @ Quaternions: w, x, y, z
        telemetry BNO055_W: F32
        telemetry BNO055_X: F32
        telemetry BNO055_Y: F32
        telemetry BNO055_Z: F32

        @ Acceleration
        telemetry BNO055_Accel_X: F32
        telemetry BNO055_Accel_Y: F32
        telemetry BNO055_Accel_Z: F32

        @ Angle rate
        telemetry BNO055_Gyro_X: F32
        telemetry BNO055_Gyro_Y: F32
        telemetry BNO055_Gyro_Z: F32

        @ Magnetic field
        telemetry BNO055_Magn_X: F32
        telemetry BNO055_Magn_Y: F32
        telemetry BNO055_Magn_Z: F32

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

    }
}