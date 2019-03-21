# arrowPad
1. A 3 x 3 keypad that uses a custom pcb and mechanical keyboard switches.


1. Parts
- [ ] Pcb
- [ ] Diodes
- [ ] Kailh Choc sockets
- [ ] Kailh Choc switches
- [ ] Choc keycaps
- [ ] Pro Micro
- 
Extra 
- [ ] 6v - 16v battery
- [ ] Trrs jack
- [ ] Trrs cable
- [ ] Mini NRF24L01

1. Design
The pcb has slots for 

1. Swappable sockets

Sockets make swapping easy; allowing you to try different flavors of choc switches.

1. Wirless

The NRF24l01 can be added onto the pcb to give the keypad wirless capabilities. The NRF24l01 is powered throught the VCC pin on the Pro Micro. However, the VCC pin provides 5v while the NRF24l01 can only take in 1.9-3.6v. 
In order to fix this I (soon) will add a voltage divider to the pcb.

1. Trrs jack

Allow connection of 2 keypads through the trrs jack and cable. The cable makes 4 connects, (VCC -> RAW), (GND -> GND), (TX -> RX), and (RX -> TX)
