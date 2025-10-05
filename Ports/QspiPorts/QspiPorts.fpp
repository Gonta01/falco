module Drv {
  module QSPI {
    enum QSPIError : U8 {
      NoError,
      IOError,
      Busy
    }

    enum QSPILaneWidth : U8 {
      SINGLE_BIT_SPI  = 0,
      DUAL_OUTPUT     = 1,
      QUAD_OUTPUT     = 2,
      DUAL_IO         = 3,
      QUAD_IO         = 4,
      DUAL_CMD        = 5,
      QUAD_CMD        = 6
    }

    enum QSPIAddressLength : U16 {
      ADDRL_24_BIT = 0,    # (0U << 10U),
      ADDRL_32_BIT = 1024  # (1U << 10U)
    }

    enum QSPIOptionLength : U16 {
      OPTL_1_BIT = 0,   # (0U << 8U),
      OPTL_2_BIT = 256, # (1U << 8U),
      OPTL_4_BIT = 512, # (2U << 8U),
      OPTL_8_BIT = 768  # (3U << 8U)
    }

    struct Ports {
      Channel1: bool
      Channel2: bool
      Channel3: bool
      Channel4: bool

      Channel5: bool
      Channel6: bool
      Channel7: bool
      Channel8: bool
    }

    struct QPSICommandConfig {
      Instruction: U8
      Width: Drv.QSPI.QSPILaneWidth
      Addr_en: bool
      Addr_len: Drv.QSPI.QSPIAddressLength
    }

    struct QSPIRegisterConfig {
      Instruction: U8
      Width: Drv.QSPI.QSPILaneWidth
      Dummy_cycles: U8
    }

    struct QSPIMemoryConfig {
      Instruction: U8
      Option: U8
      Width: Drv.QSPI.QSPILaneWidth
      Addr_len: Drv.QSPI.QSPIAddressLength
      Option_en: bool
      Option_len: Drv.QSPI.QSPIOptionLength
      Continuous_read_en: bool
      Dummy_cycles: U8
    }

    # Write command through QSPI
    # @param config - configuration structure
    # @param address - start address
    # @return NoError in case of success
    port CommandWrite(ref config: Drv.QSPI.QPSICommandConfig, address: U32) -> Drv.QSPI.QSPIError

    # Read registers' values
    # @param config - configuration structure
    # @param buff - buffer where read values will be stored
    # @return NoError in case of success
    port RegisterRead(ref config: Drv.QSPI.QSPIRegisterConfig, ref buff: Fw.Buffer) -> Drv.QSPI.QSPIError

    # Write values to registers
    # @param config - configuration structure
    # @param buff - buffer of values that will be written to registers
    # @return NoError in case of success
    port RegisterWrite(ref config: Drv.QSPI.QSPIRegisterConfig, ref buff: Fw.Buffer) -> Drv.QSPI.QSPIError

    # Read memory region starting with address
    # @param config - configuration structure
    # @param address - start address
    # @param buff - buffer where read values will be stored
    # @return NoError in case of success
    port MemoryRead(ref config: Drv.QSPI.QSPIMemoryConfig, address: U32, ref buff: Fw.Buffer) -> Drv.QSPI.QSPIError

     # Write memory region starting with address
     # @param config - configuration structure
     # @param address - start address
     # @param buff - buffer values of which will written to memory
     # @return NoError in case of success
    port  MemoryWrite(ref config: Drv.QSPI.QSPIMemoryConfig, address: U32, ref buff: Fw.Buffer) -> Drv.QSPI.QSPIError

  }
}
