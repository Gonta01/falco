# Components::IMU2Manager

Component responsible for IMU2 management

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
|testGetRawDataSuccessIMU2Manager|Tests that IMU2Manager successfully retrieves data when the data source is available and returns valid values.|Passed|
|testGetRawDataFailedIMU2Manager|Tests that IMU2Manager correctly handles failure scenarios, such as data source unavailability.|Passed|
|testGetQuatDataSuccessIMU2Manager|Tests that IMU2Manager successfully retrieves quat when the quat source is available and returns valid values.|Passed|
|testGetQuatDataFailedIMU2Manager|Tests that IMU2Manager correctly handles failure scenarios, such as quat source unavailability.|Passed|

## Requirements
3.1.3.16 Requirements for the driver of nine-axis MEMS absolute orientation sensors (accelerometer, gyroscope, magnetometer) ICM-20948 and BNO055 – IMUManager
| Name | Description | Validation |
|---|---|---|
|F_ICM20948_BNO055-1. |The driver must provide initialization of the ICM-20948 sensor (IMU#1) via the I2C interface at its address, including checking communication with the sensor after power-on.|---|
|F_ICM20948_BNO055-2. |The driver must provide initialization of the BNO055 sensor (IMU#2) via the UART interface, including checking communication with the sensor after power-on.|testConfigureBNO055|
|F_ICM20948_BNO055-3. |The driver must configure the accelerometer, gyroscope, and magnetometer operating modes: measurement range and sampling rate for each of the IMUs separately.|testConfigureBNO055|
|F_ICM20948_BNO055-4. |The driver must support reading data from the accelerometer, gyroscope, and magnetometer through the specified interface, by interacting with low- and middle-level drivers.|testGetRawDataSuccessIMU2Manager|
|F_ICM20948_BNO055-5. |The driver must support setting the sensor operating mode, in particular, operating modes with raw data (accelerometer, gyroscope, magnetometer) or full orientation mode (Fusion Mode).|testConfigureBNO055|
|F_ICM20948_BNO055-6. |The driver must convert the read values ​​into physical units: acceleration (m/s²), angular velocity (degrees/s), and magnetic field (μT).|testGetAccelBNO055, testGetGyroBNO055, testGetMagnBNO055|
|F_ICM20948_BNO055-7. |The driver must provide raw data processing, including zero offset compensation and value scaling.|testGetRawBNO055|
|F_ICM20948_BNO055-8. |The driver must support different sensor power modes: normal mode, low power modes.|testConfigureBNO055|
|F_ICM20948_BNO055-9. |The driver must allow changing the data sampling rate for each of the sensors (accelerometer, gyroscope, magnetometer) during initial initialization.|testConfigureBNO055|testGetRawDataFailedIMU2Manager|
|F_ICM20948_BNO055-10.| The driver must verify the validity of the read data by reading transport layer driver errors and detect anomalies, such as loss of sensor communication or invalid values.|testFailureReadFailBNO055, testFailureResponseHeaderMismatchBNO055, testFailureResponseSizeMismatchBNO055, testFailureReceiveBNO055, testFailureSendBNO055, testFailureRecieveCharacterTimeoutBNO055, testFailureMaxLengthErrorBNO055, testFailureMinLengthErrorBNO055, testFailureWrongStartByteBNO055, testFailureRegmapInvalidAddressBNO055, testFailureReadFailBNO055|
|F_ICM20948_BNO055-11.| The driver must support accelerometer, gyroscope, and magnetometer calibration mechanisms to ensure measurement accuracy.|testConfigureBNO055|
|F_ICM20948_BNO055-12.| The driver must store and use calibration factors to compensate for zero offsets and scale factors.|testGetAccelBNO055, testGetMagnBNO055, testGetGyroBNO055,|
|F_ICM20948_BNO055-13.| The driver must provide APIs to obtain current values ​​of acceleration, angular velocity, magnetic field, and orientation.|testGetRawDataSuccessIMU2Manager|
|
## Change Log
| Date | Description |
|---|---|
|---| Initial Draft |