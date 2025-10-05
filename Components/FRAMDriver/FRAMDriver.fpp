module Components {
    @ Component that is responsible for FRAM operating
    passive component FRAMDriver {

        sync input port read: Drv.FRAM.Read

        sync input port write: Drv.FRAM.Write

        sync input port wakeup: Drv.FRAM.Wakeup

        sync input port sleep: Drv.FRAM.Sleep

        sync input port reinit: Drv.FRAM.Reinit

        output port driver_reinit: Drv.SPI.Reinit

        output port driver_write: Drv.SPI.Write

        output port driver_read: Drv.SPI.Read

        @ Port sending calls to the GPIO driver
        output port chipSelect: Drv.GpioWrite

        @ Event reports that hardware uninited
        event Uninited \
            severity warning high \
            format "Uninited"
            
        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller
        
        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

    }
}