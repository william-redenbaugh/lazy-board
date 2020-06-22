# ** NOTE**  MY LIBRARY WAS BASED OFF OF ADAFRUITS SSD1351 LIBRARY, CHECK THEIR STUFF OUT IF YOU WANNA KNOW WHERE THE CODE CAME FROM. I JUST REWROTE THE LIBRARY TO BE: 

## Simpler: There was just too much C++ inheritance and special functions that I felt it got in the way of understanding on the Matrix OLED really works

## Faster: Using DMA may be more memory intensive, but allows stuff be to wayy faster

## Less RAM: even with DMA, currently the project uses less RAM than the default Adafruit GFX library