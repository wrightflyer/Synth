This is a VS2013 project to create an ILI9341 simulator. 

This is done by using the ILI9341_t3 driver code and as little
of the Arduino infrastructure required to make it compile but
then at the core the SPI routines to send command and data to
the display are replaced by routines to intercept the data
flow and feed it to writes in a COLORREF array whis is then
blitted to a window in WM_PAINT.

(or that's the plan anyway) 

This is so that display output can easily be visualized on
a PC without having to repeatedly flash a Teensy (or even need
Teensy/ILI9341 hardware at all) 

Update: hang on to your hat but this is the first ever picture
of the sim kind of working. This was supposed to be a blue/white
graduated fill but it turned out a bit more "rainbow" but that's
hopefully just a colour mapping issue....

![](ILI_sim/1st_ever.png)