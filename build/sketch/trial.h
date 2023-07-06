#line 1 "C:\\Users\\brody\\Documents\\Arduino\\LEDConstPower\\trial.h"
#ifndef TRIAL_H_INCLUDED
#define TRIAL_H_INCLUDED

#include "logging.h"
#include "diode.h"
#include "solve.h"

#include <Arduino.h>

// All variables are in base units; amps, volts, kelvin, etc.

// #edit: Consider changing these constants for your particular board
const double ADCSampleTime = 0.000104; // This is for ATmega based boards (UNO, Nano, Mini, Mega)
const double PWMPeriod = 1.0 / 980.0; // for pins 5 & 6 of UNO, Nano, Mini or pins 3 & 11 of Mega

void SetPinModes(DiodeTestPins _pins) {
    pinMode(_pins.ballast, INPUT);
    pinMode(_pins.diode, INPUT);
    pinMode(_pins.temperature, INPUT);
    pinMode(_pins.PWM, OUTPUT);
}

double GetTemperature(DiodeTestPins _pins) {
    LogNewScope();

    SetPinModes(_pins);

    // #edit, #toDo: Implement this function
    // If you add a temperature sensor,
    // Add logic for reading temperature sensor here
    // Return a value in Kelvin

    LogEndScope();

    return roomTemperature;
}

int VoltageToDutyCycle(double _voltage, CircuitParameters _circuitParams) {
    LogNewScope();

    int dutyCycle = 255.0 * _voltage / _circuitParams.supplyVoltage;
    Log("Duty cycle", dutyCycle);

    LogEndScope();
    return dutyCycle;
}

// Takes the given number of ADC samples
double GetVoltage(int _pin, CircuitParameters _circuitParams, int _numSamples) {
    LogNewScope();

    double sum = 0;
    for (int i = 0; i < _numSamples; i++) {
        sum += analogRead(_pin) / _circuitParams.voltageDividerRatio * 5.0 / 1023.0;
    }
    double voltage = sum / _numSamples;
    Log("Voltage", voltage);

    LogEndScope();
    return voltage;
}

// Takes ADC samples for approximately the duration specified
// Useful for reading voltages produced by a PWM signal so that the ADC samples
// take a whole number of PWM periods
double GetVoltage(int _pin, CircuitParameters _circuitParams, double _duration) {
    LogNewScope();
    int numSamples = (int)(_duration / ADCSampleTime);
    Log("Samples required", numSamples);
    LogEndScope();
    return GetVoltage(_pin, _circuitParams, numSamples);
}

// Sets the voltage across the ballast resistor and diode
// Voltage is iteratively set to get it as accurate as possible. _stopCriteria sets the limits for when to stop iterating
void SetVoltage(double _voltage, DiodeTestPins _pins, CircuitParameters _circuitParams, double _sampleDuration = 10*PWMPeriod, StopCriteria _stopCriteria = {3, 0.0001}) {
    LogNewScope();
    
    SetPinModes(_pins);

    double relativeTrueError = INFINITY;
    int iterations = 0;

    double PWM = (double)VoltageToDutyCycle(_voltage, _circuitParams);
    analogWrite(_pins.PWM, PWM);
    delay((int)(_circuitParams.settleTime * 1000.0)); //must convert to milliseconds
    double voltageActual = GetVoltage(_pins.ballast, _circuitParams, _sampleDuration);
    relativeTrueError = (voltageActual - _voltage) / _voltage;

    Log("PWM", voltageActual);
    Log("Voltage", voltageActual);
    Log("Relative true error", relativeTrueError);

    while (iterations < _stopCriteria.maxIterations && abs(relativeTrueError) > _stopCriteria.maxRelativeError) {
        iterations++;

        PWM = PWM * _voltage / voltageActual;
        analogWrite(_pins.PWM, PWM);
        delay((int)(_circuitParams.settleTime * 1000.0)); //must convert to milliseconds
        voltageActual = GetVoltage(_pins.ballast, _circuitParams, _sampleDuration);
        relativeTrueError = (voltageActual - _voltage) / _voltage;

        Log("Iteration", iterations);
        Log("  Voltage", voltageActual);
        Log("  Relative true error", relativeTrueError);
    }
    LogEndScope();
}

void SetCurrent(double _current, Diode _diode, DiodeTestPins _pins, CircuitParameters _circuitParams, double _sampleDuration = 10, StopCriteria _stopCriteria = {5, 0.001}) {
    SetPinModes(_pins);

    double temperature = GetTemperature(_pins);
    double voltage = CalculateVoltageRequiredForCurrent(_current, _circuitParams.ballast, _diode, temperature);

    // #toDo: Replace with a loop that actually calculates current through the circuit for more accuracy
    SetVoltage(voltage, _pins, _circuitParams, _sampleDuration, _stopCriteria);
}

DiodeTestData RunDiodeTest(DiodeTestPins _pins, CircuitParameters _circuitParams,  double _sampleDuration = 10, StopCriteria _stopCriteria = {5, 0.001}) {
    SetPinModes(_pins);

    DiodeTestData testData;

    SetVoltage(4.0, _pins, _circuitParams);
    testData.temperature[0] = GetTemperature(_pins);
    testData.totalVoltage[0] = GetVoltage(_pins.ballast, _circuitParams, PWMPeriod * 10);
    testData.forwardVoltage[0] = GetVoltage(_pins.diode, _circuitParams, PWMPeriod * 10);
    testData.ballastVoltage[0] = testData.totalVoltage[0] - testData.forwardVoltage[0];
    testData.current[0] = testData.ballastVoltage[0] / _circuitParams.ballast;

    SetVoltage(6.0, _pins, _circuitParams);
    testData.temperature[1] = GetTemperature(_pins);
    testData.totalVoltage[1] = GetVoltage(_pins.ballast, _circuitParams, PWMPeriod * 10);
    testData.forwardVoltage[1] = GetVoltage(_pins.diode, _circuitParams, PWMPeriod * 10);
    testData.ballastVoltage[1] = testData.totalVoltage[1] - testData.forwardVoltage[1];
    testData.current[1] = testData.ballastVoltage[1] / _circuitParams.ballast;

    return testData;
}

#endif