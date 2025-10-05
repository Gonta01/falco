# SAMV71Q21B::RS485Driver

RS485 driver. 

## Typical Usage

It was designed to be used in half-duplex RS485 communication.

## Usage Examples

## Implementation descriptions

### Data transmission

Driver enable pin is set high, data transmission conducted, driver enable pin is set low.

### Data reception

By default driver enable pin is set low, so component polls available data.


## Change Log
| Date | Description | Redmine task |
|---|---|---|
| 18.09.24 | Initial implementation | #61734 |