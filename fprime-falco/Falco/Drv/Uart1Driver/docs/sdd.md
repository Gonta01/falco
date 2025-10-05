# SAMV71Q21B::UART1Driver

UART1 driver. 

## Typical Usage

It was designed to be used in full-duplex communication.

## Usage Examples

As part of ByteStreamDriverModel

## Implementation descriptions

### Data transmission

Buffer that is intended to be transmitted is copied into non-cached buffer (UART1Buffer) and then this buffer is provided to DMA which transfers data from buffer to transmit register. During data transmission receiver is disabled.

Buffer size that can be transmitted is limited by size of non-cached buffer.

Note: non cached buffer is used because of [Cache Coherency Issue](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/TechnicalReferenceManual/Handling-Cache-Coherency-Issues-at-Runtime-Using-Cache-Maintenance-Operations-on-PIC32MZ-MCUs-DS90003308.pdf)


### Data reception

Data reception implies polling of circular buffer (it is implemented inside Microchip's HAL).

### Error reports

Transmission and receptions errors are counted and can be retrieved through report port.

## Change Log
| Date | Description | Redmine task |
|---|---|---|
| --- | Initial implementation | #61179 |
| 18.09.24 | Implementation with DMA on transmit and HAL's circular buffer on reception | #61734 |