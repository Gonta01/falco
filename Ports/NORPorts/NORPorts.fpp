module Drv {
  module NOR {
    
    enum ErrorCode : U8 {
      NoError,
      WrongMemoryId,
      InputBufferEmpty,
      AccessOutOfRange,
      UnalignedAccess,
      IncompleteErase,
      IOInterfaceError,
      FlashMemoryBusy,
    }

    port  GetSectorSize() -> U32
    port  GetMemorySize() -> U32

    port  Write(startAddress: U32, ref dataOutBuff: Fw.Buffer) -> Drv.NOR.ErrorCode
    port  Read(startAddress: U32, ref dataInBuff: Fw.Buffer) -> Drv.NOR.ErrorCode
    port  Erase(startAddress: U32, length: U32) -> Drv.NOR.ErrorCode
    port  FullErase() -> Drv.NOR.ErrorCode
  }

  module NORManager {
    enum Status : U8 {
      FAILED,
      SUCCESS,
      IN_PROCESS,
    }

    port RequestWrite(startAddress: U32, ref writeBuff: Fw.Buffer)
    port RequestRead(startAddress: U32, ref readBuff: Fw.Buffer)
    port RequestErase(startAddress: U32, length: U32)
    port Status(status: Drv.NORManager.Status)
  }
}
