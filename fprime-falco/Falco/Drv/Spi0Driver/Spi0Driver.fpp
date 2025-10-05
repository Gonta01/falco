module Falco {
    @ Component to interact with SPI peripheral of MCU
    passive component Spi0Driver {

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################
        @ Reinitialize SPI peripheral
        sync input port reinit: Drv.SPI.Reinit

        @ Send data through SPI
        sync input port write: Drv.SPI.Write

        @ Receive data through SPI
        sync input port read: Drv.SPI.Read

    }
}