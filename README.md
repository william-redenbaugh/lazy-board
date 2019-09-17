
<h4> If you are looking for information on how it works, I made a detailed post on hackster.io. </h4> 

https://texasinstruments.hackster.io/jwlow/multipurpose-keypad-682653

<h1> Build Guide </h1>
If you want to build one of these yourself, follow this guide.

<h2> Parts List </h2>

  Here is a list of parts you will need.
  * __Pro Micro__ - _An arduino programmable board. _
  * __TI TLC5940__ - _LED Driver used to control the 16 LEDs._
  * __Schottky diode__(16) - _Used to prevent unwanted key presses (keyboard ghosting)._
  * __Kailh Choc switches__(16) - _The type of mechanical switch used for each key. There are many types avaliable which feel and sound different (clicky, tactile, linear)_
  * __Kailh Choc PCB sockets__(16) - _Sockets for the Choc Switches soldered on to the PCB. Sockets allow for the mechanical switches to be taken out easily._
  * __Kailh low profile keycaps__(16) - _Keycaps for the mechanical switches._
  * __3528 SMD LEDs__(16) - _The PCB has pads for SMD LEDs, I made the pads large so different kinds of sizes would work._
  * __10K Resistor__ - _For the LED driver._
  * __Solder Wick__ - _Needed for hand soldering the LED driver._
  
  <h4> Obtaining PCBs </h4>
  
* PCBs can be obtained by ordering them through sites like JLCPCB or PCBway. Ive ordered PCBs from both of them, the prices are good. One of the draw backs is that shipping to the US can talke 3-4 weeks.
 
<h2> Build Time! </h2>

* I will use the terms front and back. Back will refer to the side with the Snorlax.

<h4> Soldering LEDs </h4>

* If you look at the led pad in the top left, it labels the positive and negative. The rest of the LEDs will follow this orientation. When soldering the LEDs on, start just by soldering it on to one of the pads, this will make it easier to adjust. Then check if it is positioned correctly by trying to place the switch in. Adjust if needed. Once each LED is aligned properly solder in all the other ends.
 
<h4> LED driver </h4>

* First align the driver with the solder pads, it helps to solder in one of the ends to keep it in place. Next place a bunch of solder on all the pins so everything is connecting. Then take some wick and place it in a group of 4-8 pins to remove the solder. This should remove the solder between each pin, however the solder connecting the pad and the pin will stay. Repeat this with the rest.
(PIC)
* Make sure you solder the 10K resistor (R3). Make sure it is installed on the back side.

<h4> Diodes </h4>

* When putting in the diode, the direction matters. Follow the labeling on the front side. Also make sure to install the diodes on the back side of the pcb, this will allow for the housing to fit properly. Cut the excess leads.

<h4> Sockets and Pro Micro </h4>
* First install all the sockets. Sockets at K3 and K4 will need to have the bottom portions cut with an x-acto to allow for the pro micro to fit in. Once the pro micro is installed, cut the excess leads. Try to make sure the cuts are as close to the board as possible, as a switch will be installed on top of them.
(PIC)

<h2> Housing </h2>
I included the dxf files for the housing. It uses #2 screws.

<h2> Programming </h2>
Key presses can be modified by changing the key array in line 14. If you want to use keyboard modifiers, refer here https://www.arduino.cc/en/Reference/KeyboardModifiers.
There are 3 preset lighting modes.....



