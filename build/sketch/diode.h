#line 1 "C:\\Users\\brody\\Documents\\Arduino\\LEDConstPower\\diode.h"
#ifndef DIODE_H_INCLUDED
#define DIODE_H_INCLUDED

// #edit: Consider changing these constants for your particular board
const double roomTemperature = 293.0; // 20 centigrade

struct Diode {
    // A & lnB are redundant info, but are kept becuase they are convenient for calculations
    double __A; // Can be found by emission coefficient times boltzmann's constant divided by elementary charge
    double __lnB; // Can be found by taking natural log of saturation current

    double saturationCurrent;
    double emissionCoefficient;
};

struct DiodeTestPins {
    int PWM; // pin that will be used to generate test voltages
    int diode; // pin that measures forward voltage of diode
    int ballast; // pin that measures the voltage across the diode and ballast
    int temperature; // unused currently
};

struct CircuitParameters {
    double supplyVoltage;
    double ballast;
    double settleTime;
    double voltageAmplifierGain; // gain of input op amp
    double voltageDividerRatio;
};

// Defines stopping criteria for iterative processes like numerically solving
// values or adjusting experimental inputs based on outputs
struct StopCriteria {
    unsigned int maxIterations;  
    double maxRelativeError; // 100% is represented as 1.0
};

struct DiodeTestData {
    double current[2];
    double forwardVoltage[2];
    double ballastVoltage[2];
    double totalVoltage[2];
    double temperature[2]; // should be in Kelvin

    // #toDo: add some kind of measure of statistical uncertainty?
};

#endif
