module Components {
    @ Component for FRAM testing
    active component FRAMTester {

        output port requestRead: Drv.FRAM.RequestRead
        output port requestWrite: Drv.FRAM.RequestWrite

        output port requestWakeup: Drv.FRAM.RequestWakeup
        output port requestSleep: Drv.FRAM.RequestSleep
        async input port status: Drv.FRAM.ExternalStatus

        async input port run: Svc.Sched

         @ Event reports finish of FRAM test
        event FRAMTestFinished \
            severity activity high \
            format "Test finished. Full memory tested."

        @ Event reports write read test failed
        event BuffersNotEqual \
            severity warning high \
            format "Buffers not equal. Write-Read test failed."
        
        @ Event reports that SM is in Init state
        event InitSM \
            severity activity high \
            format "State Machine: init"
        
        @ Event reports that SM is in Safe state
        event SafeSM \
            severity activity high \
            format "State Machine: safe"

        @ Event reports that SM is in Sleep state
        event SleepSM \
            severity activity high \
            format "State Machine: sleep"    

        @ Event reports that SM is in Sleep state
        event TestedChunk(addr: U32, chunkSize: U32) \
            severity activity low \
            format "Tested chunk. Addr: {}. Size: {}"        

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

    }
}