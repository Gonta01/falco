# Components::BNO055

Component to work with IMU sensor BNO055

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
|---|---|---|---|
|testReadBNO055|Verifies that data can be correctly read from a BNO055 register.|Passed|
|testWriteBNO055|Validates that data can be written to a BNO055 register successfully.|Passed|
|testGetAccelBNO055|Tests retrieval of accel data from the BNO055.|Passed|
|testGetGyroBNO055|Tests retrieval of gyroscope data from the BNO055.|Passed|
|testGetMagnBNO055|Tests retrieval of magnetometer data from the BNO055.|Passed|
|testGetRawBNO055|Checks raw data output from the BNO055.|Passed|
|testGetQuatBNO055|Tests quaternion data reading from the BNO055.|Passed|
|testConfigureBNO055|Tests full configuration routine of the BNO055|Passed|
|testFailureWriteFailBNO055|Simulates a write failure and ensures it is handled|Passed|
|testFailureReadFailBNO055|Simulates a read failure and checks for proper error reporting|Passed|
|testFailureRegmapInvalidAddressBNO055|Tests response to accessing an invalid register|Passed|
|testFailureRegmapWriteDisabledBNO055|Attempts to write to a read-only register and checks behavior|Passed|
|testFailureWrongStartByteBNO055|Simulates an incorrect start byte in a communication sequence|Passed|
|testFailureMaxLengthErrorBNO055|Tests behavior when maximum allowed payload is exceeded|Passed|
|testFailureMinLengthErrorBNO055|Tests behavior with too-short payloads or headers|Passed|
|testFailureRecieveCharacterTimeoutBNO055|Simulates timeout in receiving a byte from the sensor|Passed|
|testFailureResponseHeaderMismatchBNO055|Simulates a mismatch between expected and actual response headers|Passed|
|testFailureResponseSizeMismatchBNO055|Tests for mismatched expected vs actual response size|Passed|
|testFailureReceiveBNO055| Simulates general receive failure|Passed|
|testFailureSendBNO055|Simulates send command failure|Passed|
|testFailureInitBNO055|Tests failure during sensor initialization(in that case currentOperationMode != amgMode)|Passed|
|testFailureSelfTestBNO055|Simulates failure of internal self-test on the BNO055(expected chipID != actual)|Passed|

## Requirements
Add requirements in the chart below
| Name | Description | Validation |
|---|---|---|
|---|---|---|

## Change Log
| Date | Description |
|---|---|
|---| Initial Draft |