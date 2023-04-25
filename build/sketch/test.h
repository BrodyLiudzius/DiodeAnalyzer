#line 1 "C:\\Users\\brody\\Documents\\Arduino\\LEDConstPower\\test.h"
#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

// struct for pins
// struct for circuit constants like settle time, amplifier gain, divider ratio, etc
// struct for error and max iterations

struct DiodeTestData {
    float current;
    float forwardVoltage;

    float ballastVoltage;
    float totalVoltage;

    float standardDeviation;
};


float CalculateRCSettleTime(float _resistance, float _capacitance, int _numStages) {
    // Ripple is lower for more stages and higher PWM frequency, so use whatever PWM
    // pins have the highest frequency for the given Arduino model
    return _resistance * _capacitance * _numStages * 10.0;
    // Explanation: Takes ~10 time constants for output voltage to fully settle
    // across input filters given a change in PWM duty cycle
    // If multiple RC filters, add up their time constants
}

void SetVoltage(int _pinPWM, int pinBallastSense, float _voltage, float _settleTime, float _amplifierGain) {
    // set duty cycle
    // wait for settle time
    // test voltage
    // repeat
}

DiodeTestData TestDiode(int _pinPWM, int _pinDiodeSense, int _pinBallastSense, float _ballast, float _settleTime, int _numTests) {
    
}



#endif