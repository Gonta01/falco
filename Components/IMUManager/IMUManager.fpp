module Components {
    @ Component to manage interaction with IMUs
    active component IMUManager {

        @ Receiving calls from the rate group
        async input port run: Svc.Sched drop

        guarded input port getICM20948Data:       Drv.IMUManager.GetData
        guarded input port getICM20948Quaternion: Drv.IMUManager.GetQuaternion

        guarded input port getBNO055Data:         Drv.IMUManager.GetData
        guarded input port getBNO055Quaternion:   Drv.IMUManager.GetQuaternion

        output port getAccelDataICM20948:      Drv.IMU.GetAccelData
        output port getGyroDataICM20948:       Drv.IMU.GetGyroData
        output port getMagnDataICM20948:       Drv.IMU.GetMagnData
        output port pollRawData:               Drv.IMU.PollData

        output port getQuatDataICM20948:       Drv.IMU.GetQuatData
        output port pollQuatData:              Drv.IMU.PollData

        output port getAccelDataBNO055:        Drv.IMU.GetAccelData
        output port getGyroDataBNO055:         Drv.IMU.GetGyroData
        output port getMagnDataBNO055:         Drv.IMU.GetMagnData
        output port getQuatDataBNO055:         Drv.IMU.GetQuatData

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