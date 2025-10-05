module Drv {
  module UART {
    enum WriteStatus {
      FAILED = 0            @< Transmission failure
      SUCCESS = 1            @< Transmission success
    }

    struct Diagnostics {
      Overrun: U32 @< Counter of overrun errors
      Parity:  U32 @< Counter of parity errors
      Framing: U32 @< Counter of framing errors
      ReceiveBufferFull: U32 @< Counter of how many time receive buffer was fully filled; It may be an indicator of potential data loss
      TransmitFailed: U32 @< Counter of failed transmissions
      TransmitBufferOversize: U32 @< Counter of how many attempts to send oversized buffer were conducted
    }

    port Write(
            ref buffer: Fw.Buffer   @< Buffer with data to write from
            timeoutMs: U32 
            ) -> WriteStatus

    port Read(
            ref buffer: Fw.Buffer   @< Buffer with data to read to
            timeoutMs: U32 
            ) -> U32                @< Number of read bytes is returned

    port GetNumOfBytes() -> U32     @< Number of bytes available for read

    port Report() -> Diagnostics
  }
}
