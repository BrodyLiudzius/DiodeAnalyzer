#ifndef SOLVE_H_INCLUDED
#define SOLVE_H_INCLUDED

#include "diode.h"

#include <Arduino.h>

// All variables are in base units; amps, volts, kelvin, etc.

double CalculateRCSettleTime(double _resistance, double _capacitance, int _numStages) {
    // Ripple is lower for more stages and higher PWM frequency, so use whatever PWM
    // pins have the highest frequency for the given Arduino model
    return _resistance * _capacitance * _numStages * 10.0;
    // Explanation: Takes ~10 time constants for output voltage to fully settle across
    // input filters given a change in PWM duty cycle. If multiple RC filters, add up
    // their time constants
}

Diode CharacterizeDiode(DiodeTestData _testData) {
    const double elementaryCharge = 1.602176634E-19;
    const double boltzmannConstant = 1.380649E-23; // Is a more precise value known for boltmann's constant?

    Diode diode;

    // Is this related to seebeck/thomson/peltier effects or coefficients?
    double voltPerKelvin0 = _testData.forwardVoltage[0] / _testData.temperature[0];
    double voltPerKelvin1 = _testData.forwardVoltage[1] / _testData.temperature[1];
    double voltTemp = voltPerKelvin0 / voltPerKelvin1; // is this a ratio of them?

    diode.__lnB = (voltTemp * log(_testData.current[1] - log(_testData.current[0]))) / (voltTemp - 1);
    diode.saturationCurrent = exp(diode.__lnB);

    double A0 = _testData.forwardVoltage[0] / (_testData.temperature[0] * (log(_testData.current[0] - diode.__lnB)));
    double A1 = _testData.forwardVoltage[1] / (_testData.temperature[1] * (log(_testData.current[1] - diode.__lnB)));

    diode.__A = (A0 + A1) / 2.0; // Averaging both A's because it seems like a reasonable thing to do

    diode.emissionCoefficient = diode.__A * elementaryCharge / boltzmannConstant;

    return diode;
}

double CalculateCurrentRequiredForForwardVoltage(double _forwardVoltage, double _ballast, Diode _diode, double _temperature = roomTemperature) {
    return _diode.saturationCurrent * (exp(_forwardVoltage / _diode.__A / _temperature) - 1);
}

double CalculateCurrentRequiredForPower(double _powerDissipation, Diode _diode, double _temperature = roomTemperature, StopCriteria _stopCriteria = {1000, 1E-6}) {
    // A variation of secant method is used to numerically solve for the current
    
    double relativeTrueError = INFINITY;
    int iterations = 0;

    double I0 = 0.1; // We need to make a starting guess for what the current is
    double I1 = 0.1001;

    // Record whatever the most accurate calculation is so far in case the algorithm does not converge
    double lowestRelTrueErr = INFINITY;
    double mostAccurateI;

    while (iterations < _stopCriteria.maxIterations && abs(relativeTrueError) > _stopCriteria.maxRelativeError) {
        iterations++;

        double P0 = I0 * _diode.__A * _temperature * log(I0 / _diode.saturationCurrent);
        double P1 = I1 * _diode.__A * _temperature * log(I1 / _diode.saturationCurrent);

        double absoluteTrueError = P1 - _powerDissipation;
        relativeTrueError = absoluteTrueError / _powerDissipation;

        if (abs(relativeTrueError) < lowestRelTrueErr)
            mostAccurateI = I1;
        
        double slope = (P0 - P1) / (I0 - I1);

        I0 = I1;
        I1 += absoluteTrueError / slope;
    }
    
    return I1;
}

double CalculateVoltageRequiredForCurrent(double _current, double _ballast, Diode _diode, double _temperature = roomTemperature) {
    return _current * _ballast + _diode.__A * log(_current / _diode.saturationCurrent);
}

double CalculateVoltageRequiredForPower(double _powerDissipation, double _ballast, Diode _diode, double _temperature = roomTemperature, StopCriteria _stopCriteria = {1000, 1E-9}) {
    double current = CalculateCurrentRequiredForPower(_powerDissipation, _diode, _temperature, _stopCriteria);
    return CalculateVoltageRequiredForCurrent(current, _ballast, _diode, _temperature);
}

#endif
