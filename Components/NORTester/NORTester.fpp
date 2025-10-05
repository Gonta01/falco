module Components {
    @ Component for NOR testing
    active component NORTester {
        async input port run    : Svc.Sched

        output port getSectorSize : Drv.NOR.GetSectorSize
        output port getMemorySize : Drv.NOR.GetMemorySize

        output port requestErase: Drv.NORManager.RequestErase
        async input port getEraseStatus: Drv.NORManager.Status

        output port requestWrite: Drv.NORManager.RequestWrite
        async input port getWriteStatus: Drv.NORManager.Status

        output port requestRead: Drv.NORManager.RequestRead
        async input port getReadStatus : Drv.NORManager.Status

        @ Event reports that test has been started
        event TestStarted \
            severity activity high \
            format "Test has been started!"

        @ Event reports that test has been finished
        event TestFinished \
            severity activity high \
            format "Test has been finished!"

        @ Event reports that erase error occurred
        event EraseError \
            severity warning high \
            format "Erase error occurred!"

        @ Event reports that write error occurred
        event WriteError \
            severity warning high \
            format "Write error occurred!"

        @ Event reports that read error occurred
        event ReadError \
            severity warning high \
            format "Read error occurred!"

        @ Event reports that write-read testing failed
        event BuffersNotEqual \
            severity warning high \
            format "Write-read testing failed - buffers not equal!"        

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