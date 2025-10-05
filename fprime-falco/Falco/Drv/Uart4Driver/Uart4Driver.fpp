module Falco {

  passive component Uart4Driver {
    @ Take data to write out via UART
    sync input port transmit: Drv.UART.Write

    @ Read data via UART
    sync input port receive: Drv.UART.Read

    @ Get number of available bytes for reading
    sync input port getNumOfBytes: Drv.UART.GetNumOfBytes

    @ Report UART diagnostics
    sync input port report: Drv.UART.Report
  }
}
