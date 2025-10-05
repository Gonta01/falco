module Components {

  passive component ComModule {

    @ Polling for receiving data
    sync input port schedIn: Svc.Sched

    @ Indicates the driver has connected to the UART device
    output port ready: Drv.ByteStreamReady

    @Allocate new buffer
    output port allocate: Fw.BufferGet

    @return the allocated buffer
    output port deallocate: Fw.BufferSend

    @ Write data over UART
    output port write: Drv.UART.Write

    @ Read data over UART
    output port read: Drv.UART.Read

    @ Takes data to transmit out
    guarded input port $send: Drv.ByteStreamSend

    @ Outputs received data
    output port $recv: Drv.ByteStreamRecv
  }
}
