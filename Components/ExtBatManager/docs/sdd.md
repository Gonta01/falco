# Components::ExtBatManager

External battery manager

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
|enablePin|Port to enable external battery subsystem for measurements, and disable subsystem after measurements done|
|getVoltageValue|Port to retrieve ADC voltage|
|getVoltage|Port to provide all voltage data: instantaneous voltage, average voltage, validity flag, overvoltage flag, undervoltage flag|
|run|Port for receiving calls from the rate group|

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
|NotFullInit| During initialization not all measurements were successful|
|MeasurementError| Error during ADC measurement|
|VoltageInvalid| Voltage data invalid because of ADC measurement error or undervoltage or overvoltage|
|OvervoltageAlarm| Measured instantaneous voltage is over set maximum value|
|UndervoltageAlarm| Measured instantaneous voltage is under set maximum value|

## Telemetry
| Name | Description |
|---|---|
|InstantaneousVoltage|Telemetry channel that represents instantaneous voltage|
|AverageVoltage|Telemetry channel that represents average voltage|
|IsValid|Telemetry channel to indicate whether the voltage is valid|
|UndervoltageAlarm|Telemetry channel to indicate undervoltage|
|OvervoltageAlarm|Telemetry channel to indicate overvoltage|

## Unit Tests
Add unit test descriptions in the chart below
| Name | Description | Output | Coverage |
|---|---|---|---|
|ExtBatManagerUT1|It is checked that component is able to successfully handle data retrieved from mock ADC driver. Checked the ability to retrieve properly handled data and it can be added to the summary diagnostics by external component (which is out of scope of unit testing). Checked that measured value is added to telemetry channels. |Passed|---|
|ExtBatManagerUT2|Requirement states that the component should measure the external battery voltage at least every 200 ms. This requirement depends on the Rate Group component configuration that is connected to run port of ExtBatManager, that is out of range of unit testing and seems to be more related to integration testing. Formally this test is created, but time difference between timePoint1 and timePoint2 is always 0. |Unit test is invalid. Not Passed|---|
|ExtBatManagerUT3|It checked that the component has the ability to configure the maximum and minimum allowed measurement voltage.|Passed|---|
|ExtBatManagerUT4_1|The undervoltage detection is checked|Passed|---|
|ExtBatManagerUT4_2|The overvoltage detection is checked|Passed|---|
|ExtBatManagerUT5_1|Checked that voltage is invalid in case of overvoltage|Passed|---|
|ExtBatManagerUT5_2|Checked that voltage is invalid in case of undervoltage|Passed|---|
|ExtBatManagerUT5_3|Checked that voltage is invalid in case of ADC measurement error|Passed|---|
|ExtBatManagerUT6_1|Checked that component provides instantaneous and average values of the measured voltage. Checked that average value is calculated as a moving average of the last 10 measurements.|Passed|---|
|ExtBatManagerUT6_2|Checked that component provides instantaneous and average values of the measured voltage. Checked that average value is calculated as a moving average of the last 10 measurements. This test is performed with randomization.|Passed|---|

## Requirements
| Name | Description | Validation |
|---|---|---|
|ExtBatManagerReq1|The component must monitor the external battery voltage measurement (separate ADC input of the main microcontroller) by interacting with the ADC0Driver and add the measured voltage to the summary diagnostics and telemetry frame.|ExtBatManagerUT1|
|ExtBatManagerReq2|The component should measure the external battery voltage at least every 200 ms.|ExtBatManagerUT2|
|ExtBatManagerReq3|The component shall provide the ability to configure the maximum and minimum allowed measurement voltage.|ExtBatManagerUT3|
|ExtBatManagerReq4|The component must detect the output of the battery voltage beyond the maximum and minimum allowed for measurement. In case of reaching the limits of the maximum and minimum allowed battery voltage, it should generate the appropriate alarm status flag - Overvoltage Alarm, Undervoltage Alarm.|ExtBatManagerUT4_1, ExtBatManagerUT4_2|
|ExtBatManagerReq5|The component should reset the quality of the measured voltage to “bad” in the following cases: voltage out of the permitted limits, hardware errors of the ADC during voltage measurement.|ExtBatManagerUT5_1, ExtBatManagerUT5_2, ExtBatManagerUT5_3|
|ExtBatManagerReq6|The component shall provide instantaneous and average values of the measured voltage. The average value shall be calculated as a moving average of the last 10 measurements.|ExtBatManagerUT6_1, ExtBatManagerUT6_2|

## Change Log
| Date | Description |
| --- | --- |
|24/03/2025|Initial implementation|
