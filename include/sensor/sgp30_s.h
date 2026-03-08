#pragma once

#ifndef GAS_SENSOR_H
#define GAS_SENSOR_H

#include <Wire.h>
#include <Adafruit_SGP30.h>

// Driver class for SGP30 gas sensor
// Measures TVOC, Hydrogen, and Ethanol levels

class GasSensor {
private:
    static Adafruit_SGP30 sensor;

    // Gas measurement values
    static float tvocLevel;
    static float hydrogenLevel;
    static float ethanolLevel;

    static bool sensorReady;

public:
    // Initialize the sensor
    static void initialize();

    // Read new measurements
    static void update();

    // Getter functions
    static float getTVOC();
    static float getHydrogen();
    static float getEthanol();
};

#endif
