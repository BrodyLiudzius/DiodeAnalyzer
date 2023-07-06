#line 1 "C:\\Users\\brody\\Documents\\Arduino\\LEDConstPower\\README.md"
Diode Characterizer

This Arduino program contains functions that can be used to characterize a diode. Specifically, this program (in conjunction with a small breadboard circuit) will determine the saturation current and emission coefficient of whatever diode is placed into the circuit.

## Motivation

This program was written partially to aid in a school project, but also as a side project and to be a tool for future use. In the context of the project it was created for, a series of LEDs of different wavelengths (and therefore different semiconductors) needed to all be powered with approximately 50 milliwatts of power so that would all produce approximately the same amount of light.

The project involved making a line following robot using only analog circuitry and absolutely no digital components. To this end, a light sensor was needed to detect a black line on the ground as well as a light to shine at the ground. To find the best light for this purpose, a series of LEDs from infrared to blue to white were all tested with various sensors. To make for a fair test of their suitability in the project, the Arduino was used so that diodes could be hot-swapped and still produce 50 milliwatts (minus inefficiency) of light.

## Uses

This program can be used to:
* Characterize a diode (find its saturation current and emission coefficient to be used in the Shockley ideal diode equation)
* Determine the current required to produce a given forward voltage across the diode
* Determine the current required to produce a given power dissipation through the diode
* Determine the voltage required across the diode and its ballast resistor to produce a given current through them
* Determine the voltage required across the diode and its ballast resistor to produce a given power dissipation through the diode

## Supporting Circuit

The breadboard circuit has been kept as simple to construct as possible for convenience and mainly serves to smooth the ripple of the Arduino's PWM output as well as allow for the arduino to control a higher voltage supply which was necessary for the particular application this program was written for.

![A drawing of circuit diagram required](./circuitDiagramDrawing.jpg)

## Suggestions

Things that you might want to consider changing if you use this code in your own project are marked with the tag `#edit`. You can search the project files to find all lines that need to be edited. This includes things like adding a temperature sensor, as the program currently assumes that it is room temperature (20 Celsius).