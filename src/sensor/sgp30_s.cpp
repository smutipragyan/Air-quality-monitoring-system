#include "sensor/sgp30_s.h"

// Static member initialization
Adafruit_SGP30 GasSensor::sensor;

float GasSensor::tvocLevel = 0;
float GasSensor::hydrogenLevel = 0;
float GasSensor::ethanolLevel = 0;

bool GasSensor::sensorReady = false;

// Initialize the SGP30 sensor
void GasSensor::initialize() {

    if (!sensor.begin()) {
        Serial.println("SGP30 initialization failed");
        return;
    }

    Serial.println("SGP30 gas sensor ready");
    sensorReady = true;
}

// Read new gas measurements
void GasSensor::update() {

    if (!sensorReady)
        return;

    // Perform IAQ measurement
    if (sensor.IAQmeasure()) {

        tvocLevel = sensor.TVOC;

        // Retrieve raw gas values
        if (sensor.IAQmeasureRaw()) {
            hydrogenLevel = sensor.rawH2;
            ethanolLevel = sensor.rawEthanol;
        }
    }
    else {
        Serial.println("SGP30 measurement error");
    }
}

// Getter functions
float GasSensor::getTVOC() {
    return tvocLevel;
}

float GasSensor::getHydrogen() {
    return hydrogenLevel;
}

float GasSensor::getEthanol() {
    return ethanolLevel;
}
