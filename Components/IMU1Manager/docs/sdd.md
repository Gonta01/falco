# Components::IMU1Manager

Component responsible for IMU1 management

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
|testGetDataSuccessIMU1Manager|Tests that IMU1Manager successfully retrieves data when the data source is available and returns valid values.|Passed|
|testGetDataFailedIMU1Manager|Tests that IMU1Manager correctly handles failure scenarios, such as data source unavailability.|Passed|

## Requirements
3.1.3.16 Requirements for the driver of nine-axis MEMS absolute orientation sensors (accelerometer, gyroscope, magnetometer) ICM-20948 and BNO055 – IMUManager
| Name | Description | Validation |
|---|---|---|
|F_ICM20948_BNO055-1.| The driver must provide initialization of the ICM-20948 sensor (IMU#1) via the I2C interface at its address, including checking communication with the sensor after power-on.|testIsConnectedICM20948|
|F_ICM20948_BNO055-2.| The driver must provide initialization of the BNO055 sensor (IMU#2) via the UART interface, including checking communication with the sensor after power-on.|---|
|F_ICM20948_BNO055-3.| The driver must configure the accelerometer, gyroscope, and magnetometer operating modes: measurement range and sampling rate for each of the IMUs separately.|testConfigureGyroSampleRateICM20948, testConfigureAccelSampleRateICM20948, testInitMagnetometerICM20948|
|F_ICM20948_BNO055-4.| The driver must support reading data from the accelerometer, gyroscope, and magnetometer through the specified interface, by interacting with low- and middle-level drivers.|testGetDataSuccessIMU1Manager|
|F_ICM20948_BNO055-5.| The driver must support setting the sensor operating mode, in particular, operating modes with raw data (accelerometer, gyroscope, magnetometer) or full orientation mode (Fusion Mode).|Configure function in ICM20948 driver|
|F_ICM20948_BNO055-6.| The driver must convert the read values ​​into physical units: acceleration (m/s²), angular velocity (degrees/s), and magnetic field (μT).| Inside getAccel, getMagn, getGyro in ICM20948 driver|
|F_ICM20948_BNO055-7.| The driver must provide raw data processing, including zero offset compensation and value scaling.|testSetConfigICM20948, testInitMagnetometerICM20948|
|F_ICM20948_BNO055-8.| The driver must support different sensor power modes: normal mode, low power modes.|wakeup and sleep in ICM20948 driver|
|F_ICM20948_BNO055-9.| The driver must allow changing the data sampling rate for each of the sensors (accelerometer, gyroscope, magnetometer) during initial initialization.|testConfigureGyroSampleRateICM20948, testConfigureAccelSampleRateICM20948, testInitMagnetometerICM20948|
|F_ICM20948_BNO055-10.| The driver must verify the validity of the read data by reading transport layer driver errors and detect anomalies, such as loss of sensor communication or invalid values.|testGetDataFailedIMU1Manager, testFailureReadICM20948, testFailureReadNotEqualToWrittenICM20948, testFailureWriteICM20948|
|F_ICM20948_BNO055-11.| The driver must support accelerometer, gyroscope, and magnetometer calibration mechanisms to ensure measurement accuracy.|testCalibrateGyroOffsetsICM20948, for acclerometr driver use gravitationalAcceleration, and for magnetometer getSensivity()|
|F_ICM20948_BNO055-12.| The driver must store and use calibration factors to compensate for zero offsets and scale factors.|Inside magneticDataHandling, gyroDataHandling, accelsDataHandling|
|F_ICM20948_BNO055-13.| The driver must provide APIs to obtain current values ​​of acceleration, angular velocity, magnetic field, and orientation.| testGetDataSuccessIMU1Manager|

## Change Log
| Date | Description |
|---|---|
|---| Initial Draft |