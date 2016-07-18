# Celestial
#### Autonomous Flight Controller - Alpha Release

Celestial is an open-source flight controller designed for autonomous drone projects.
Compared to other flight controllers, it's built from the ground up to address several issues:
 - Cross-platform compatibility.  
 - Ease of programming and testing.
 - Safety-critical code.  
 
#### Features 
Celestial is currently in early developement and does not yet boast many features.  Currently, it can be run similar to other flight controllers. (** = in development, likely not commited yet)
 - Acro and self-stablizing flight modes.
 - ** Automatic takeoff and landing.
 - ** Altitude hold, using ultrasonics and barometer.
 - ** Acoustic localization for position estimation.
 - ** GPS waypoint navigation.

#### Cross-platform compatibility. 
Celestial is written as a basic C library that can be compiled to any device that can compile C99.  Devices can include STM32F4, XMOS, Arduino, Raspberry Pi, and others.  In order to use processor specific functionality (ie. I2C, SPI), a hardware abstraction layer needs to be written to interface between Celestial and the hardware.

Currently, the following devices are planned to be supported:
 - XMOS (https://github.com/CelestialFlight/Celestial_XMOS.git)
 - STM32F4 (coming soon)
 - Raspi (coming soon)
 - Arduino (coming soon)
 
#### Safety Critical code.
Celestial was written with safety in mind from it's conception.  This code follows what is called the "Power of Ten" and goes over several rules.
In short, they are:
 - No goto, setjmp/longjmp or indirect recursion
 - All loops should have an upper bound.
 - No dynamic memory allocation after initialization.
 - Functions should be kept short (less then N lines).
 - Assertions should average 2 per function.
 - Declare objects at smallest possible scope.
 - The return value of non-void functions should always be checked.
 - Limit the preprocessor to header files and simple macros.
 - * The use of pointers should be restricted.  
 - All code must be compiled with all compiler warnings on.  

###### ( * There is one use of function pointers that may conflict with rule #9.  The SerialBuffer struct is used often when trying to save data to an unknown peripheral (ie, UART, SD Card, SPI, etc.). The callback function is used when data is saved to the buffer.  In these cases, the callback functions should be short, simple, and easy to test that they function as expected.)
 
All code in should also be unit-tested and built before pushing.  

