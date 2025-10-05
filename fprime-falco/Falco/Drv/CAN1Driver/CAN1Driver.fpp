module Falco {

  passive component CAN1Driver {
    @ Take data to write out via CAN
    sync input port transmit: Drv.CAN.StandardTransmit

    @ Read data via CAN
    sync input port receive: Drv.CAN.StandardReceive

  }
}
