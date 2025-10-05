# Components::DPS310

Component for barometric pressure sensor DPS310

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
|testDPS310Temperature|Tests if the DPS310 correctly measures and returns a valid temperature reading under normal conditions|Passed|---|
|testDPS310ColdTemperature|Verifies that the sensor can accurately report low-temperature values|Passed|---|
|testDPS310Pressure|Tests if the DPS310 accurately measures and returns valid atmospheric pressure data|Passed|---|
|testDPS310FailureNotInited|Checks how the system handles attempts to use the DPS310 before it has been properly initialized (in that case reset() was unsucces so we get NotInited events and our mesurements was unseccesful)|Passed|---|
|testDPS310FailureWrite|Simulates a failure during an I2C write operation to the sensor and checks that the error is correctly handled|Passed|---|
|testDPS310FailureRead|Simulates a failure during an I2C read operation from the sensor and ensures proper error detection and handling|Passed|---|

## Requirements
Add requirements in the chart below
| Name | Description | Validation |
|---|---|---|
|---|---|---|

## Change Log
| Date | Description |
|---|---|
|---| Initial Draft |