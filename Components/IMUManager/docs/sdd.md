# Components::IMUManager

Component to manage interaction with IMUs

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
|testGetDataSuccessICM20948|Verifies that valid sensor data is successfully retrieved from the ICM20948 sensor under normal operating conditions.|Passed|
|testGetDataFailedICM20948|Tests behavior when the ICM20948 sensor fails to provide data, simulating hardware error or communication failure.|Passed|
|testGetRawDataSuccessBNO055|Confirms that raw sensor data from the BNO055 is correctly returned when communication and sensor state are normal.|Passed|
|testGetRawDataFailedBNO055|Simulates failure in retrieving raw data from the BNO055, ensuring graceful error handling|Passed|
|testGetQuatDataSuccessBNO055|Validates successful retrieval and parsing of quaternion data from the BNO055 sensor.|Passed|
|testGetQuatDataFailedBNO055|Ensures appropriate behavior when quaternion data retrieval fails from BNO055 due to error |Passed|

## Requirements
3.1.3.16 Requirements for the driver of nine-axis MEMS absolute orientation sensors (accelerometer, gyroscope, magnetometer) ICM-20948 and BNO055 – IMUManager
| Name | Description | Validation |
|---|---|---|
|F_ICM20948_BNO055-1.|The driver must provide initialization of the ICM-20948 sensor (IMU#1) via the I2C interface at its address, including checking communication with the sensor after power-on| Described in IMU1Manager and IMU2Manager documentation|
|F_ICM20948_BNO055-2.|The driver must provide initialization of the BNO055 sensor (IMU#2) via the UART interface, including checking communication with the sensor after power-on.|Described in IMU1Manager and IMU2Manager documentation|
|F_ICM20948_BNO055-3.|The driver must configure the accelerometer, gyroscope, and magnetometer operating modes: measurement range and sampling rate for each of the IMUs separately.|Described in IMU1Manager and IMU2Manager documentation|
|F_ICM20948_BNO055-4.|The driver must support reading data from the accelerometer, gyroscope, and magnetometer through the specified interface, by interacting with low- and middle-level drivers.|Described in IMU1Manager and IMU2Manager documentation|
|F_ICM20948_BNO055-5.|The driver must support setting the sensor operating mode, in particular, operating modes with raw data (accelerometer, gyroscope, magnetometer) or full orientation mode (Fuson Mode).|Described in IMU1Manager and IMU2Manager documentation|
|F_ICM20948_BNO055-6.|The driver must convert the read values ​​into physical units: acceleration (m/s²), angular velocity (degrees/s), and magnetic field (μT).|Described in IMU1Manager and IMU2Manager documentation|
|F_ICM20948_BNO055-7.|The driver must provide raw data processing, including zero offset compensation and value scaling.|Described in IMU1Manager and IMU2Manager documentation|
|F_ICM20948_BNO055-8.|The driver must support different sensor power modes: normal mode, low power modes.|Described in IMU1Manager and IMU2Manager documentation|
|F_ICM20948_BNO055-9.|The driver must allow changing the data sampling rate for each of the sensors (accelerometer, gyroscope, magnetometer) during initial initialization.|Described in IMU1Manager and IMU2Manager documentation|
|F_ICM20948_BNO055-10| The driver must verify the validity of the read data by reading transport layer driver errors and detect anomalies, such as loss of sensor communication or invalid values.|Described in IMU1Manager and IMU2Manager documentation|
|F_ICM20948_BNO055-11| The driver must support accelerometer, gyroscope, and magnetometer calibration mechanisms to ensure measurement accuracy.|Described in IMU1Manager and IMU2Manager documentation|
|F_ICM20948_BNO055-12| The driver must store and use calibration factors to compensate for zero offsets and scale factors.|Described in IMU1Manager and IMU2Manager documentation|
|F_ICM20948_BNO055-13| The driver must provide APIs to obtain current values ​​of acceleration, angular velocity, magnetic field, and orientation.|Described in IMU1Manager and IMU2Manager documentation|

## Change Log
| Date | Description |
|---|---|
|---| Initial Draft |