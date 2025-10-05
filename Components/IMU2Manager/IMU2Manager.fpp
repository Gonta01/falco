module Components {
    @ Component responsible for IMU2 management
    active component IMU2Manager {

        @ Receiving calls from the rate group
        async input port run: Svc.Sched drop
        
        guarded input port getData:         Drv.IMUManager.GetData
        guarded input port getQuaternion:   Drv.IMUManager.GetQuaternion

        output port getAccelData:        Drv.IMU.GetAccelData
        output port getGyroData:         Drv.IMU.GetGyroData
        output port getMagnData:         Drv.IMU.GetMagnData
        output port getRawData:          Drv.IMU.GetRawData
        output port getQuatData:         Drv.IMU.GetQuatData

        output port pollRawData:               Drv.IMU.PollData
        output port pollQuatData:              Drv.IMU.PollData
        
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