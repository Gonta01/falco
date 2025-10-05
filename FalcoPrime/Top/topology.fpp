module FalcoPrime {

  # ----------------------------------------------------------------------
  # Symbolic constants for port numbers
  # ----------------------------------------------------------------------

    enum Ports_RateGroups {
      rateGroup1,
      rateGroup2,
      rateGroup3
    }

    enum Ports_StaticMemory0 {
      framer
      deframer
      deframing
    }

  topology FalcoPrime {

    # ----------------------------------------------------------------------
    # Instances used in the topology
    # ----------------------------------------------------------------------

    instance $health
    instance cmdDisp
    instance comModule
    instance deframer
    instance eventLogger
    instance fatalAdapter
    instance fatalHandler
    instance framer
    instance greenLed
    instance spiCS
    instance led
    instance rateDriver
    instance rateGroup1
    instance rateGroup2
    instance rateGroup3
    instance rateGroupDriver
    instance staticMemory0
    instance systemResources
    instance textLogger
    instance timeHandler
    instance tlmSend
    instance sts21
    instance dps310
    instance i2c0Driver
    instance i2c1Driver
    instance i2c2Driver
    instance Uart0Driver
    instance Uart1Driver
    instance Uart4Driver
    instance RS485Driver
    instance can0Tester
    instance can0Driver
    instance can1Driver
    instance ioExpander1
    instance ioExpander2
    instance spi0Driver
    instance framDriver
    instance FRAMManager
    instance FRAMTester
    instance icm20948
    instance bno055
    instance adc0Driver
    instance extBatManager
    instance extBatTester
    instance ntc
    instance adc1Driver
    instance EnvSensManager
    instance IMU1Manager
    instance IMU1IntPin
    instance IMU2Manager
    instance alert1
    instance alert2
    instance enableBatteryMeasurement
    instance TPM

    instance qspiDriver
    instance NORDriver
    instance NORManager
    instance NORTester
    instance RTCDriver
    instance rtcManager
    instance madgwickFusion
    instance imuCalibration
    # ----------------------------------------------------------------------
    # Pattern graph specifiers
    # ----------------------------------------------------------------------

    command connections instance cmdDisp

    event connections instance eventLogger

    telemetry connections instance tlmSend

    text event connections instance textLogger

    time connections instance timeHandler

    health connections instance $health

    # ----------------------------------------------------------------------
    # Direct graph specifiers
    # ----------------------------------------------------------------------

    connections RateGroups {
      # Block driver
      rateDriver.CycleBackward -> rateDriver.CycleIn
      rateDriver.CycleOut -> rateGroupDriver.CycleIn

      # Rate group 1
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup1] -> rateGroup1.CycleIn
      rateGroup1.RateGroupMemberOut[0] -> comModule.schedIn
      rateGroup1.RateGroupMemberOut[1] -> tlmSend.Run
      rateGroup1.RateGroupMemberOut[2] -> systemResources.run

      # Rate group 2
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup2] -> rateGroup2.CycleIn
      
      # Rate group 3
      rateGroupDriver.CycleOut[Ports_RateGroups.rateGroup3] -> rateGroup3.CycleIn

      rateGroup1.RateGroupMemberOut[3] -> $health.Run
    }

    connections FaultProtection {
      eventLogger.FatalAnnounce -> fatalHandler.FatalReceive
    }

    connections Downlink {

      tlmSend.PktSend -> framer.comIn
      eventLogger.PktSend -> framer.comIn

      framer.framedAllocate -> staticMemory0.bufferAllocate[Ports_StaticMemory0.framer]
      framer.framedOut -> comModule.$send

      comModule.deallocate -> staticMemory0.bufferDeallocate[Ports_StaticMemory0.framer]

    }
    
    connections Uplink {

      comModule.allocate -> staticMemory0.bufferAllocate[Ports_StaticMemory0.deframer]
      comModule.$recv -> deframer.framedIn
      deframer.framedDeallocate -> staticMemory0.bufferDeallocate[Ports_StaticMemory0.deframer]

      deframer.comOut -> cmdDisp.seqCmdBuff
      cmdDisp.seqCmdStatus -> deframer.cmdResponseIn

      deframer.bufferAllocate -> staticMemory0.bufferAllocate[Ports_StaticMemory0.deframing]
      deframer.bufferDeallocate -> staticMemory0.bufferDeallocate[Ports_StaticMemory0.deframing]
      
    }

    connections LedConnections {
      # Rate Group 1 (1Hz cycle) ouput is connected to led's run input
      rateGroup1.RateGroupMemberOut[4] -> led.run
      # led's gpioSet output is connected to gpioDriver's gpioWrite input
      led.gpioSet -> greenLed.gpioWrite
    }

    connections ComModuleConnections {
      comModule.write -> Uart1Driver.transmit
      comModule.read  -> Uart1Driver.receive
    }


    connections CANTestConnections {
      can0Tester.write -> can0Driver.transmit 
      can0Tester.read  -> can0Driver.receive
      rateGroup2.RateGroupMemberOut[0] -> can0Tester.run
    }

    connections ICM20948Connections {
      icm20948.write -> i2c0Driver.write
      icm20948.read  -> i2c0Driver.read
    }

    connections BNO055Connections {
      bno055.write -> Uart4Driver.transmit
      bno055.read -> Uart4Driver.receive
      bno055.getNumOfBytes -> Uart4Driver.getNumOfBytes
    }

    connections IMUConnections {
      IMU2Manager.getAccelData          -> bno055.getAccelData
      IMU2Manager.getGyroData           -> bno055.getGyroData
      IMU2Manager.getMagnData           -> bno055.getMagnData
      IMU2Manager.getRawData            -> bno055.getRawData

      IMU2Manager.getQuatData           -> bno055.getQuatData

      IMU1Manager.getAccelData          -> icm20948.getAccelData
      IMU1Manager.getGyroData           -> icm20948.getGyroData
      IMU1Manager.getMagnData           -> icm20948.getMagnData
      IMU1Manager.getCalibrationData    -> icm20948.getCalibrationData
      IMU1Manager.pollRawData           -> icm20948.pollRawData

      IMU1Manager.pollQuatData          -> icm20948.pollQuatData
      IMU1Manager.getQuatData           -> icm20948.getQuatData
      icm20948.intPin                   -> IMU1IntPin.gpioRead

      rateGroup3.RateGroupMemberOut[0]  -> IMU2Manager.run
      rateGroup3.RateGroupMemberOut[1]  -> IMU1Manager.run
    }

    connections EnvSensConnections {
      rateGroup2.RateGroupMemberOut[1]        -> EnvSensManager.run
      EnvSensManager.getTemperatureDPS310     -> dps310.getTemperature
      EnvSensManager.getPressuretDPS310       -> dps310.getPressure
      EnvSensManager.getTemperatureSTS21      -> sts21.getTemperature
      EnvSensManager.getTemperatureTableNTC   -> ntc.getTableTemperature
      EnvSensManager.getTemperaturePolynomNTC -> ntc.getPolynomTemperature

      dps310.write-> i2c0Driver.write
      dps310.read-> i2c0Driver.read

      sts21.write-> i2c0Driver.write
      sts21.read-> i2c0Driver.read

      ntc.getVoltageValue     -> adc1Driver.getVoltageValue
      ntc.getVoltageReference -> adc1Driver.getVoltageReference
      ntc.getADCCode          -> adc1Driver.getADCCode
      ntc.getADCMax           -> adc1Driver.getADCMax
    }

    connections NORConnection {
      NORDriver.commandWrite  -> qspiDriver.commandWrite
      NORDriver.memoryRead    -> qspiDriver.memoryRead
      NORDriver.memoryWrite   -> qspiDriver.memoryWrite
      NORDriver.registerRead  -> qspiDriver.registerRead
      NORDriver.registerWrite -> qspiDriver.registerWrite

      NORManager.erase        -> NORDriver.erase
      NORManager.write        -> NORDriver.write
      NORManager.read         -> NORDriver.read

      NORTester.requestErase   -> NORManager.requestErase
      NORTester.requestWrite   -> NORManager.requestWrite
      NORTester.requestRead    -> NORManager.requestRead
      NORManager.eraseStatus   -> NORTester.getEraseStatus
      NORManager.writeStatus   -> NORTester.getWriteStatus
      NORManager.readStatus    -> NORTester.getReadStatus

      NORTester.getMemorySize -> NORManager.getOutMemorySize
      NORTester.getSectorSize -> NORManager.getOutSectorSize

      NORManager.getInMemorySize -> NORDriver.getMemorySize
      NORManager.getInSectorSize -> NORDriver.getSectorSize

      rateGroup1.RateGroupMemberOut[8]  -> NORTester.run
    }

    connections RTCConnections {
      rateGroup2.RateGroupMemberOut[2]    -> rtcManager.run
      rtcManager.setTime                  -> RTCDriver.setTime
      rtcManager.getTime                  -> RTCDriver.getTime
    }

    connections FRAM {
      FRAMTester.requestRead -> FRAMManager.requestRead
      FRAMTester.requestWrite -> FRAMManager.requestWrite
      FRAMTester.requestSleep -> FRAMManager.requestSleep
      FRAMTester.requestWakeup -> FRAMManager.requestWakeup
      
      rateGroup1.RateGroupMemberOut[9] -> FRAMTester.run
      
      FRAMManager.status -> FRAMTester.status
      FRAMManager.read -> framDriver.read
      FRAMManager.write -> framDriver.write
      FRAMManager.sleep -> framDriver.sleep
      FRAMManager.wakeup -> framDriver.wakeup
      FRAMManager.reinit -> framDriver.reinit

      framDriver.driver_reinit -> spi0Driver.reinit
      framDriver.driver_write -> spi0Driver.write
      framDriver.driver_read -> spi0Driver.read
      framDriver.chipSelect -> spiCS.gpioWrite
    }

    connections TPMConnections {
      rateGroup2.RateGroupMemberOut[3]    -> TPM.run
      TPM.write                           -> i2c0Driver.write
      TPM.read                            -> i2c0Driver.read
    }

    # connections MadgwickFusion {
    #   madgwickFusion.getIMUData        -> IMU1Manager.getData
    #   madgwickFusion.write             -> Uart1Driver.transmit
    #   rateGroup3.RateGroupMemberOut[2] -> madgwickFusion.run
    # }

    # connections Calibration {
    #   imuCalibration.getData -> IMU1Manager.getData
    #   imuCalibration.write   -> Uart1Driver.transmit
    #   imuCalibration.read   -> Uart1Driver.receive
    #   rateGroup1.RateGroupMemberOut[0] -> imuCalibration.run
    # }
  }
}
