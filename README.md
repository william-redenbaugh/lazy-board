# UNDER CONSTRUCTION
# arrowPad
1. A 4 x 4 keypad that uses a custom pcb and mechanical keyboard switches.


1. Parts
- [ ] Order Pcb (I used JLCPcb.com)
- [ ] Diodes
- [ ] Kailh Choc sockets
- [ ] Kailh Choc switches
- [ ] Kailh Choc keycaps
- [ ] Arduino Pro Micro
- [ ] M3 Screw holes

1. Extra Parts
- [ ] Slide switches (Used for enabling or disabling features)
- [ ] Smd led(3528 SMD LED)
- [ ] TI TLC5940 (TSSOP package)
- [ ] Trrs jack 
- [ ] Trrs cable 
- [ ] Mini NRF24L01 (not fully implemented)
- [ ] 6v - 16v battery (not fully implemented)

1. Design

The pcb has slots for 

1. LED lighting

The keyboard uses the Texas Instruments TLC5940. The driver allows PWM control over 16 LEDS while only taking up 3 pins on the pro micro. 

1. Swappable sockets

Sockets make swapping easy; allowing you to try different flavors of choc switches.

1. Wirless(not fully implemented)

The NRF24l01 can be added onto the pcb to give the keypad wirless capabilities. The NRF24l01 is powered throught the VCC pin on the Pro Micro. There is a slide switch that controls if 3.3v is being sent to the NRF24l01.

1. Trrs jack

Allow connection of 2 keypads. Data transfer through i2c. The cable makes 4 connects, (VCC -> RAW), (GND -> GND), (SCL -> SCL), and (SDA ->SDA) 
