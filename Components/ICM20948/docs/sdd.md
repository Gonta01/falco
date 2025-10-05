# Components::ICM20948
ICM20948 component files includes some modified code of [SparkFun_ICM-20948_ArduinoLibrary](https://github.com/sparkfun/SparkFun_ICM-20948_ArduinoLibrary)

## Usage Examples
Add usage examples here

### Diagrams
Add diagrams here

### Typical Usage
And the typical usage of the component here

## Class Diagram
Add a class diagram here

## Port Descriptions
| Name | Description |
|---|---|
|---|---|

## Component States
Add component states in the chart below
| Name | Description |
|---|---|
|---|---|

## Sequence Diagrams
Add sequence diagrams here

## Parameters
| Name | Description |
|---|---|
|---|---|

## Commands
| Name | Description |
|---|---|
|---|---|

## Events
| Name | Description |
|---|---|
|---|---|

## Telemetry
| Name | Description |
|---|---|
|---|---|

## Unit Tests
Add unit test descriptions in the chart below
| Name | Description | Output | Coverage |
|---|---|---|---|
testIsConnectedICM20948|Verifies that the sensor is properly connected by reading the WHO_AM_I register.|Passed|
testDeviceResetICM20948|Tests the full device reset functionality|Passed|
testConfigClockSourceICM20948|Tests if the correct clock source is configured.|Passed|
testEnableGyroAndAccelICM20948|Verifies that both the gyroscope and accelerometer can be enabled successfully.|Passed|
testEnableFIFOFalseICM20948|Tests disabling FIFO mode and ensures it’s correctly configured off.|Passed|
testDisableDMPICM20948|Ensures the Digital Motion Processor (DMP) can be disabled properly.|Passed|
testSetConfigICM20948|Verifies general configuration|Passed|
testConfigureFIFOICM20948|Tests configuration of FIFO settings|Passed|
testConfigureInterruptPinICM20948|Verifies the interrupt pin behavior|Passed|
testConfigureDataReadyInterruptICM20948|Ensures the data ready interrupt is properly configured|Passed|
testConfigureGyroSampleRateICM20948|Tests if the gyroscope sample rate is set correctly|Passed|
testConfigureAccelSampleRateICM20948|Tests if the accelerometer sample rate is set|Passed|
testInitMagnetometerICM20948|Validates that the magnetometer is properly initialized|Passed|
testCalibrateGyroOffsetsICM20948|Tests the calibration routine that calculates and applies gyroscope offsets.|Passed|
testEnableFIFOTrueICM20948|Tests enabling FIFO mode|Passed|
testResetFIFOICM20948|Verifies the FIFO buffer can be reset|Passed|
testPollRawDataICM20948|Simulates polling raw sensor data|Passed|
testPollQuatDataICM20948|Simulates polling quaternion data from DMP|Failed|
testFailureNotInitedICM20948|Tests error handling when trying to use the device before proper initialization.|Passed|
testFailureReadICM20948|Simulates a failed read operatio and verifies proper error handling.|Passed|
testFailureWriteICM20948|Simulates a failed write operation and checks error reporting|Passed|
testFailureReadNotEqualToWrittenICM20948|Tests a read-back value does not match the expected written value.|Passed|

## Requirements
Add requirements in the chart below
| Name | Description | Validation |
|---|---|---|
|---|---|---|

## Change Log
| Date | Description |
|---|---|
|---| Initial Draft |