module Falco {

  passive component RS485Driver {
    @ Write data over UART
    output port write: Drv.UART.Write

    @ Read data over UART
    output port read: Drv.UART.Read
    @ Receive data through RS485
    guarded input port $recv: Drv.ByteStreamPoll

    @ Send data through RS485
    guarded input port $send: Drv.ByteStreamSend
  }
}
