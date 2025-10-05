# Components::STS21

Temperature sensor working through I2C

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
|testSTS21Temperature|Tests if the STS21 sensor correctly reads and returns a valid temperature value under normal operating conditions|Passed|---|
|testSTS21ColdTemperature|Tests the sensor's ability to read and report low temperature values accurately|Passed|---|
|testSTS21FailureCommunication|Simulates I2C communication failure and verifies that the system handles this failure gracefully(In that example wrong CRC)|Passed|---|
|testSTS21FailureNotInited|Checks behavior when the sensor is accessed before proper initialization, ensuring the system correctly detects and handles uninitialized sensor use|Passed|---|

## Requirements
Add requirements in the chart below
| Name | Description | Validation |
|---|---|---|
|---|---|---|

## Change Log
| Date | Description |
|---|---|
|---| Initial Draft |