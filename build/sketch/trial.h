#line 1 "C:\\Users\\brody\\Documents\\Arduino\\LEDConstPower\\trial.h"
#ifndef TRIAL_H_INCLUDED
#define TRIAL_H_INCLUDED

#include "diode.h"
#include "solve.h"

#include <Arduino.h>

bool SetCircuitVoltage(double _voltage, DiodeTestPins _pins, CircuitParameters _circuitParams, StopCriteria _stopCriteria) {
    // set pin modes

    // set duty cycle
    // wait for settle time
    // test voltage
    // repeat
}

bool SetCircuitCurrent(double _current, Diode _diode, DiodeTestPins _pins, CircuitParameters _circuitParams, StopCriteria _stopCriteria) {
    // set pin modes

    // calculate required voltage

    // set voltage
    // calculate current (can use _diode or measurements. Measurements probably induce less error)
    // calculate percent change in voltage
    // repeat
}

DiodeTestData RunDiodeTest(DiodeTestPins _pins, CircuitParameters _circuitParams, StopCriteria _stopCriteria) {
    // set pin modes

    // set circuit voltage
    // wait for settle time
    // measure values

    // set circuit voltage
    // wait for settle time
    // measure values

    // generate test data
}

#endif