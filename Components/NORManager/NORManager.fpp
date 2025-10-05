module Components {
    @ Component NOR management
    active component NORManager {

        guarded input port getOutSectorSize : Drv.NOR.GetSectorSize
        guarded input port getOutMemorySize : Drv.NOR.GetMemorySize
        
        output port getInSectorSize : Drv.NOR.GetSectorSize
        output port getInMemorySize : Drv.NOR.GetMemorySize

        output port read :     Drv.NOR.Read
        output port write:     Drv.NOR.Write
        output port erase:     Drv.NOR.Erase

        async input port requestWrite: Drv.NORManager.RequestWrite
        output port writeStatus : Drv.NORManager.Status
        
        async input port requestRead : Drv.NORManager.RequestRead
        output port readStatus  : Drv.NORManager.Status
        
        async input port requestErase : Drv.NORManager.RequestErase
        output port eraseStatus  : Drv.NORManager.Status

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