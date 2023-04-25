#ifndef TRIAL_H_INCLUDED
#define TRIAL_H_INCLUDED

#include "diode.h"
#include "solve.h"

#include <Arduino.h>

// All variables are in base units; amps, volts, kelvin, etc.

const double ADCSampleTime = 0.000104; // This is for ATmega based boards (UNO, Nano, Mini, Mega)
const double PWMPeriod = 1.0 / 980.0; // for pins 5 & 6 of UNO, Nano, Mini or pins 3 & 11 of Mega
const double roomTemperature = 293.0; // 20 centigrade

void SetPinModes(DiodeTestPins _pins) {
    pinMode(_pins.ballast, INPUT);
    pinMode(_pins.diode, INPUT);
    pinMode(_pins.temperature, INPUT);
    pinMode(_pins.PWM, OUTPUT);
}

double GetTemperature(DiodeTestPins _pins) {
    SetPinModes(_pins);

    // If you add a temperature sensor,
    // Add logic for reading temperature sensor here
    // Return a value in Kelvin

    return roomTemperature;
}

int VoltageToDutyCycle(double _voltage, CircuitParameters _circuitParams) {
    return 1023 * (_voltage / _circuitParams.supplyVoltage);
}

// Takes the given number of ADC samples
double GetVoltage(DiodeTestPins _pins, CircuitParameters _circuitParams, int _numSamples) {
    double sum = 0;
    for (int i = 0; i < _numSamples; i++) {
        sum += analogRead(_pins.ballast) / _circuitParams.voltageDividerRatio;
    }
    return sum / _numSamples;
}

// Takes ADC samples for approximately the duration specified
// Useful for reading voltages produced by a PWM signal so that the ADC samples
// take a whole number of PWM periods
double GetVoltage(DiodeTestPins _pins, CircuitParameters _circuitParams, double _duration) {
    int numSamples = (int)(_duration / ADCSampleTime);
    return GetVoltage(_pins, _circuitParams, numSamples);
}

bool SetVoltage(double _voltage, DiodeTestPins _pins, CircuitParameters _circuitParams, double _sampleDuration = 10, StopCriteria _stopCriteria = {3, 0.0001}) {
    SetPinModes(_pins);

    double relativeTrueError = INFINITY;
    int iterations = 0;

    analogWrite(_pins.PWM, VoltageToDutyCycle(_voltage, _circuitParams));
    delay(_circuitParams.settleTime * 1000.0); //must convert to milliseconds
    double voltageActual = GetVoltage(_pins, _circuitParams, _sampleDuration);
    relativeTrueError = (voltageActual - _voltage) / _voltage;

    while (iterations < _stopCriteria.maxIterations && relativeTrueError > _stopCriteria.maxRelativeError) {
        iterations++;

        analogWrite(_pins.PWM, VoltageToDutyCycle(voltageActual/relativeTrueError, _circuitParams));
        delay(_circuitParams.settleTime * 1000.0); //must convert to milliseconds
        voltageActual = GetVoltage(_pins, _circuitParams, _sampleDuration);
        relativeTrueError = (voltageActual - _voltage) / _voltage;
    }
}

bool SetCurrent(double _current, Diode _diode, DiodeTestPins _pins, CircuitParameters _circuitParams, StopCriteria _stopCriteria = {3, 0.0001}) {
    SetPinModes(_pins);

    double temperature = GetTemperature(_pins);
    CalculateVoltageRequiredForCurrent(_current, _circuitParams.ballast, _diode, temperature);

    // set voltage
    // calculate current (can use _diode or measurements. Measurements probably induce less error)
    // calculate percent change in voltage
    // repeat
}

DiodeTestData RunDiodeTest(DiodeTestPins _pins, CircuitParameters _circuitParams, StopCriteria _stopCriteria) {
    SetPinModes(_pins);

    // set circuit voltage
    // wait for settle time
    // measure values

    // set circuit voltage
    // wait for settle time
    // measure values

    // generate test data
}

#endif