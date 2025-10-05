module Drv {
  module I2C {
    enum Status {
      SUCCESS = 0 @< Transaction okay
      WRITE_ERR = 1 @< I2C write failed
      READ_ERR = 2 @< I2C read failed
      WRITE_READ_ERR = 3 @< I2C write-read failed
    }

    port Write(
                addr: U32 @< I2C slave device address
                ref serBuffer: Fw.Buffer @< Buffer with data to read to
                timeout: U32 @< Max time of waititing for operation ending
                ) -> Drv.I2C.Status

    port Read(
              addr: U32 @< I2C slave device address
              ref serBuffer: Fw.Buffer @< Buffer with data to write from
              timeout: U32 @< Max time of waititing for operation ending
              ) -> Drv.I2C.Status

    @ Write a set of bytes then read a set of bytes using the repeated start option
    port WriteRead(
                     addr: U32 @< I2C slave device address
                     ref writeBuffer: Fw.Buffer @< Buffer to write data to the i2c device
                     ref readBuffer: Fw.Buffer @< Buffer to read back data from the i2c device, must set size when passing in read buffer
                     timeout: U32 @< Max time of waititing for operation ending
                   ) -> Drv.I2C.Status

  }
}
