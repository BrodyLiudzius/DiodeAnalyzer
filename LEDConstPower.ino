// Copyright (C) 2023 Brody Liudzius

#define LOGGING_ENABLED

#include "diode.h"
#include "solve.h"
#include "trial.h"

#include <Arduino.h>

void setup() {
    Serial.begin(9600);
    DiodeTestPins pins = {5, 14, 15, 16};
    CircuitParameters circuitParams = {15.0, 220.0, CalculateRCSettleTime(1000.0, 0.000001, 3), 2.0, 0.5};
    SetVoltage(2.0, pins, circuitParams, 10*PWMPeriod, {3, 0.0001});
    //analogWrite(pins.PWM, 50);
    delay(1000);
}

void loop() {}
