module Falco {

  passive component Uart0Driver {
    @ Take data to write out via UART
    guarded input port transmit: Drv.UART.Write

    @ Read data via UART
    guarded input port receive: Drv.UART.Read

    @ Report UART diagnostics
    sync input port report: Drv.UART.Report
  }
}
