module Falco {

  passive component CAN0FDDriver {
    @ Take data to write out via CAN
    sync input port transmit: Drv.CAN.FDTransmit

    @ Read data via CAN
    sync input port receive: Drv.CAN.FDReceive

  }
}
