Z3
==

A Verilog implementation of the Infocom Z-Machine V3.

Back in the very early 80s Infocom, the purveyor of very fine text based video games, when faced with the problems of porting their games to the plethora of home computers available back in the day invented a virtual machine optimised for running interactive fiction. Then interpreters were written for all the popular computers of the day so they could all run the same code.

This virtual machine was never meant to be implemented in hardware although now thanks to cheap FPGAs it can be! The Z-Machine CPU is written in Verilog and included here. So using a Cyclone II EP2C5 FPGA (~$10), a TFT LCD Arduino Shield (~$3), 512Kb 8-bit flash (~$2), 128Kb 8-bit SRAM (~$2) and a ADC ($2) you too could have a functioning Z-Machine system.

Check out the following YouTube video to see it in action...

[![Hardware Z-Machine](http://img.youtube.com/vi/HuQZq6DQQDY/0.jpg)](http://www.youtube.com/watch?v=HuQZq6DQQDY)

The spec the Z3 follows is...
http://inform-fiction.org/zmachine/standards/z1point0/index.html. Specifically version 3, the so called "Standard" games. These were released between 1982 and 1987 and covers most Infocom games.

Z3 passes the "CZECH - Comprehensive Z-machine Emulation CHecker" by Amir Karger. And is known to run Zork I, Hitchhiker's Guide to the Galaxy, Planetfall and Curses (by Graham Nelson).

To provide some input/output a terminal is implemented in a BIOS (in Z-machine code) which exists in the ROM above 120Kb. The IO related opcodes (like @print) vector into the BIOS which provides the terminal. A small number of extra op codes were added so the BIOS could interface with an LCD touchscreen.

Save/Restore isn't currently supported (as my hardware hasn't any storage) and is the only notable omission.

To compare the performance of the Z-Machine to other machines from around the same time. I ported the Dhrystone benchmark (and others) to Inform (a language for writing Z-Machine code). Using this benchmark the Z3 at 10MHz is around 0.93 DMIPS which is comparable to the performance of an Atari ST running native code.

The BIOS contains two easter eggs. By typing "invade" a fairly complete Space Invaders clone will start. Also "mandelbrot" draws a mandelbrot fractal.

For (very brief) instructions to build, check instructions.txt.

Disclaimer: This is my first Verilog project so might not be written all that well
