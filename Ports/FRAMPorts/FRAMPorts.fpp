module Drv {
  module FRAM {
    enum ExtStatus {
      UNDEFINED       = 0 
      READ_SUCCESS    = 1 
      READ_FAILED     = 2  
      WRITE_SUCCESS   = 3 
      WRITE_FAILED    = 4 
      WAKEUP_SUCCESS  = 5
      WAKEUP_FAILED   = 6 
      SLEEP_SUCCESS   = 7
      SLEEP_FAILED    = 8 
    }

    enum InStatus {
      SUCCESS  = 0 
      FAILED   = 1
    }

    port Read(
          addr: U32
          ref buffer: Fw.Buffer
          ) -> Drv.FRAM.InStatus

    port Write(
            addr: U32
            ref buffer: Fw.Buffer
            ) -> Drv.FRAM.InStatus

    port Wakeup() -> Drv.FRAM.InStatus

    port Sleep() -> Drv.FRAM.InStatus

    port Reinit()

    port RequestRead(
          addr: U32
          ref buffer: Fw.Buffer
          )

    port RequestWrite(
            addr: U32
            ref buffer: Fw.Buffer
            )

    port RequestWakeup()

    port RequestSleep()

    port ExternalStatus(status: Drv.FRAM.ExtStatus)
  }
}
