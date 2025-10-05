module Falco {

  passive component Uart1Driver {
    @ Take data to write out via UART
    sync input port transmit: Drv.UART.Write

    @ Read data via UART
    sync input port receive: Drv.UART.Read

    @ Report UART diagnostics
    sync input port report: Drv.UART.Report
  }
}
