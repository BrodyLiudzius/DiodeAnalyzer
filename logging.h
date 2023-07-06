#ifndef LOGGING_H_INCLUDED
#define LOGGING_H_INCLUDED

#include <Arduino.h>

#ifdef LOGGING_ENABLED
    int numIndents = 0;
#endif

#define LogNewScope() (LogNewScopeName(__FUNCTION__))

// "Look at me. I'm the debugger now"

void printIndent() {
    #ifdef LOGGING_ENABLED
        for (int i = 0; i < numIndents; i++) {
            Serial.print("\t");
        }
    #endif
}

void LogNewScopeName(const char* _functionName) {
    #ifdef LOGGING_ENABLED
        printIndent();
        Serial.print("in ");
        Serial.println("_functionName:");
        numIndents++;
    #endif
}

void Log(char* _valueName, double _value) {
    #ifdef LOGGING_ENABLED
        printIndent();
        Serial.print(_valueName);
        Serial.print(": ");
        Serial.println(_value);
    #endif
}

void LogEndScope() {
    #ifdef LOGGING_ENABLED
        Serial.print("\n");
        numIndents--;
    #endif
}

#endif // ifndef LOGGING_H_INCLUDED