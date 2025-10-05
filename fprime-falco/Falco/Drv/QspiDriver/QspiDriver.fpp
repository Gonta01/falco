module Falco {
    @ Component to interact with SPI peripheral of MCU
    passive component QspiDriver {

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        @ QSPI command write
        guarded input port commandWrite:  Drv.QSPI.CommandWrite

        @ QSPI register read
        guarded input port registerRead:  Drv.QSPI.RegisterRead
        
        @ QSPI register write
        guarded input port registerWrite: Drv.QSPI.RegisterWrite

        @ QSPI memory read
        guarded input port memoryRead:    Drv.QSPI.MemoryRead

        @ QSPI memory write
        guarded input port memoryWrite:   Drv.QSPI.MemoryWrite

    }
}