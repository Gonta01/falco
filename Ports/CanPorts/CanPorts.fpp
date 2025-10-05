module Drv {
  module CAN {
    enum Status {
      NoError     = 0,
      NoDataToRead  = 1,
      WriteFailed = 2
    };

    enum FrameFormat {
      StandardFrame = 0,
      ExtendedFrame = 1,
    };

    enum RemoteFrame {
      Disable = 0,
      Enable = 1,
    };

    enum FrameFD {
      Disable = 0,
      Enable = 1,
    };

    enum BitRateSwitch {
      Disable = 0,
      Enable = 1
    };

    enum NominalBitRate {
      BitRate250kbps = 0,
      BitRate500kbps = 1,
      BitRate1000kbps = 2
    };

    enum DataBitRate {
      BitRate2Mbps = 0,
      BitRate4Mbps = 1
    };

    enum DataLengthCode {
      len8byte = 8,
      len12bytes = 9,
      len16bytes = 10,
      len20bytes = 11,
      len24bytes = 12,
      len32bytes = 13,
      len48bytes = 14,
      len64bytes = 15,
    };

    struct StandardFlags {
      ID  : U32
      ExtendedFrameFlag : Drv.CAN.FrameFormat
      RemoterFrameFlag : Drv.CAN.RemoteFrame
    } 

    struct FDFlags {
      ID  : U32
      ExtendedFrameFlag : Drv.CAN.FrameFormat
      FDFrameFlag : Drv.CAN.FrameFD
      BitRateSwitchFlag: Drv.CAN.BitRateSwitch
    } 

    port StandardTransmit(
            ref buffer: Fw.Buffer
            flags: Drv.CAN.StandardFlags 
            timeoutMs: U32
            ) -> Drv.CAN.Status

    port StandardReceive(
            ref buffer: Fw.Buffer
            ref flags: Drv.CAN.StandardFlags
            timeoutMs: U32
            ) -> Drv.CAN.Status

    port FDTransmit(
            ref buffer: Fw.Buffer
            flags: Drv.CAN.FDFlags 
            timeoutMs: U32
            ) -> Drv.CAN.Status

    port FDReceive(
            ref buffer: Fw.Buffer
            ref flags: Drv.CAN.FDFlags
            timeoutMs: U32 
            ) -> Drv.CAN.Status
    }
}
