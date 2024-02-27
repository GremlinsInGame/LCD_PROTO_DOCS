# LCD_PROTO_DOCS

There are many RA8875 Libraries that are Arduino Compatible.
Here is one that can be used:
https://github.com/mjs513/RA8875/tree/RA8875_t4

To enable capacitive touchscreen functionality in this RA8875 Library, open the 
folder located here: *RA8875-RA8875_t4\src\_settings*
and uncomment this code(currently located on line 63):
```c
#define USE_FT5206_TOUCH//capacitive touch screen
```
Take a look at the PDF to use as a soldering guide if you are building the PCB from the Kit.
**LCD-PROTO PCB Components Guide.pdf**

Tips for Assembly and Soldering is in the Wiki.
**https://github.com/GremlinsInGame/LCD_PROTO_DOCS/wiki**

.INO file is the Demo program running in the Teensy 4.1
**LCD_Proto.ino**
