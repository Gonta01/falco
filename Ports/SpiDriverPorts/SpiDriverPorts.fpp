module Drv {
  module SPI {
    enum Status {
      NoError          = 0,
      IOError          = 1,
      BufferOutOfRange = 2
    };

    port Reinit()

    port Write(
                ref buffer: Fw.Buffer
                timeoutMs: U32
              ) -> Drv.SPI.Status

    port Read(  
                ref buffer: Fw.Buffer
                timeoutMs: U32
              ) -> Drv.SPI.Status
  }
}
