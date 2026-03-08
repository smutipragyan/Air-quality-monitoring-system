#ifndef SCD41_S_H
#define SCD41_S_H

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_SCD4x_Arduino_Library.h>

// Class for handling SCD41 environmental sensor
// Measures CO2 concentration, temperature, and humidity

class EnvironmentSensor {
private:
    static SCD4x sensor;

    // Latest sensor readings
    static int co2ppm;
    static float temperatureCelsius;
    static float relativeHumidity;

    // Utility function
    static float convertCtoF(float tempC);

public:
    // Initialize sensor and start measurement
    static void initialize();

    // Read new sensor data
    static bool update();

    // Getter functions
    static int getCO2();
    static float getTemperatureF();
    static float getHumidity();
};

#endif
