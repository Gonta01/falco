module FalcoPrime {

  # ----------------------------------------------------------------------
  # Defaults
  # ----------------------------------------------------------------------

  module Default {
    constant QUEUE_SIZE = 10
    constant STACK_SIZE = 1024
  }

  # ----------------------------------------------------------------------
  # Active component instances
  # ----------------------------------------------------------------------
  instance rateDriver: Falco.HardwareRateDriver base id 0x0001 \
    queue size Default.QUEUE_SIZE\
    stack size Default.STACK_SIZE \
    priority 30

  instance rateGroup1: Svc.ActiveRateGroup base id 0x0010 \
    queue size Default.QUEUE_SIZE \
    stack size 2048 \
    priority 29

  instance rateGroup2: Svc.ActiveRateGroup base id 0x0020 \
    queue size Default.QUEUE_SIZE \
    stack size 2048 \
    priority 29

  instance rateGroup3: Svc.ActiveRateGroup base id 0x0030 \
    queue size Default.QUEUE_SIZE \
    stack size 2048 \
    priority 29
  
  instance textLogger: DebugLogger.DebugActiveTextLogger base id 0x0040 \
    queue size 50 \
    stack size 2048 \
    priority 28

  instance eventLogger: Svc.ActiveLogger base id 0x0050 \
    queue size 50 \
    stack size Default.STACK_SIZE \
    priority 27

  instance cmdDisp: Svc.CommandDispatcher base id 0x0060 \
    queue size Default.QUEUE_SIZE\
    stack size 2048 \
    priority 25

  instance tlmSend: Svc.TlmChan base id 0x0070 \
    queue size 20 \
    stack size Default.STACK_SIZE \
    priority 23

  instance rs485Tester: Components.RS485Tester base id 0x0300 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 20 
  
  instance can0Tester: Components.CANTester base id 0x0500 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 20 

  instance FRAMManager: Components.FRAMManager base id 0x0700 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 20  

  instance FRAMTester: Components.FRAMTester base id 0x0800 \
    queue size Default.QUEUE_SIZE \
    stack size 4096 \
    priority 20  

  instance EnvSensManager: Components.EnvSensManager base id 0x1200 \
    queue size Default.QUEUE_SIZE \
    stack size 2048 \
    priority 20

  instance ioExpander1: Components.IOExpander base id 0x1600 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 20

  instance ioExpander2: Components.IOExpander base id 0x1700 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 20

  instance extBatManager: Components.ExtBatManager base id 0x1800 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 20

  instance extBatTester: Components.ExtBatTester base id 0x1900 \
    queue size Default.QUEUE_SIZE \
    stack size Default.STACK_SIZE \
    priority 20

  instance NORTester: Components.NORTester base id 0x1B00 \
    queue size Default.QUEUE_SIZE \
    stack size 2048 \
    priority 20

  instance NORManager: Components.NORManager base id 0x1C00 \
    queue size Default.QUEUE_SIZE \
    stack size 2048 \
    priority 20
  
  instance rtcManager: Components.RTCManager base id 0x2000 \
    queue size Default.QUEUE_SIZE \
    stack size 2048 \
    priority 16 
  
  instance IMU1Manager: Components.IMU1Manager base id 0x2100 \
    queue size Default.QUEUE_SIZE \
    stack size 2048 \
    priority 20

  instance IMU2Manager: Components.IMU2Manager base id 0x2200 \
    queue size Default.QUEUE_SIZE \
    stack size 2048 \
    priority 20

  instance TPM: Components.TPM base id 0x2300 \
    queue size Default.QUEUE_SIZE \
    stack size 2048 \
    priority 20  

  instance madgwickFusion: Components.MadgwickFusion base id 0x2400 \
    queue size Default.QUEUE_SIZE \
    stack size 2048 \
    priority 20

  instance imuCalibration: Components.Calibration base id 0x2500 \
    queue size Default.QUEUE_SIZE \
    stack size 2048 \
    priority 20  

  # ----------------------------------------------------------------------
  # Queued component instances
  # ----------------------------------------------------------------------
    instance $health: Svc.Health base id 0x3900 \
    queue size 25

  # ----------------------------------------------------------------------
  # Passive component instances
  # ----------------------------------------------------------------------
  instance Uart0Driver: Falco.Uart0Driver base id 0x4000

  instance RS485Driver: Falco.RS485Driver base id 0x4100

  instance Uart1Driver: Falco.Uart1Driver base id 0x4200

  instance Uart4Driver: Falco.Uart4Driver base id 0x4300

  instance comModule: Components.ComModule base id 0x5000

  instance framer: Svc.Framer base id 0x5100

  instance fatalAdapter: Svc.AssertFatalAdapter base id 0x5200

  instance fatalHandler: Svc.FatalHandler base id 0x5300

  instance timeHandler: Falco.SystemTime base id 0x5400

  instance rateGroupDriver: Svc.RateGroupDriver base id 0x5500

  instance staticMemory0: Svc.StaticMemory base id 0x5600

  instance deframer: Svc.Deframer base id 0x5800

  instance systemResources: Svc.SystemResources base id 0x5900

  instance greenLed: Falco.GpioDriver base id 0x5C00

  instance spiCS: Falco.GpioDriver base id 0x5D00

  instance i2c0Driver: Falco.I2c0Driver base id 0x5E00

  instance i2c1Driver: Falco.I2c1Driver base id 0x5F00

  instance i2c2Driver: Falco.I2c2Driver base id 0x6000

  instance can0Driver: Falco.CAN0Driver base id 0x6100
  
  instance can1Driver: Falco.CAN1Driver base id 0x6200

  instance framDriver: Components.FRAMDriver base id 0x6500

  instance spi0Driver: Falco.Spi0Driver base id 0x6600

  instance adc0Driver: Falco.Adc0Driver base id 0x6800

  instance adc1Driver: Falco.Adc1Driver base id 0x6900

  instance ntc: Components.NTC base id 0x6A00

  instance sts21: Components.STS21 base id 0x6B00

  instance dps310: Components.DPS310 base id 0x6C00

  instance bno055: Components.BNO055 base id 0x6D00

  instance icm20948: Components.ICM20948 base id 0x6E00

  instance alert1: Falco.GpioDriver base id 0x6F00

  instance alert2: Falco.GpioDriver base id 0x7000

  instance enableBatteryMeasurement: Falco.GpioDriver base id 0x7100

  instance IMU1IntPin: Falco.GpioDriver base id 0x7200

  instance qspiDriver: Falco.QspiDriver base id 0x7300

  instance NORDriver: Components.NORDriver base id 0x7400

  instance RTCDriver: Falco.RTCDriver base id 0x7500

  instance led: Components.Led base id 0x10000
  
}
