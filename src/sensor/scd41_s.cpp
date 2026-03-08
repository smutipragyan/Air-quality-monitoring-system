#include "sensor/scd41_s.h"

// Static member initialization
SCD4x EnvironmentSensor::sensor;

int EnvironmentSensor::co2ppm = 0;
float EnvironmentSensor::temperatureCelsius = 0;
float EnvironmentSensor::relativeHumidity = 0;

// Convert Celsius to Fahrenheit
float EnvironmentSensor::convertCtoF(float tempC) {
    return (tempC * 9.0f / 5.0f) + 32.0f;
}

// Initialize the SCD41 sensor
void EnvironmentSensor::initialize() {

    Wire.begin();
    delay(100);

    if (!sensor.begin()) {
        Serial.println("SCD41 sensor not detected");
        return;
    }

    Serial.println("SCD41 initialized");

    sensor.startPeriodicMeasurement();
}

// Read new measurement data
bool EnvironmentSensor::update() {

    static unsigned long lastSample = 0;
    static bool measurementActive = false;
    static unsigned long lastRecovery = 0;

    // Ensure periodic measurement is running
    if (!measurementActive) {

        if (!sensor.startPeriodicMeasurement()) {
            Serial.println("Unable to start SCD41 measurements");
            return false;
        }

        measurementActive = true;
        lastSample = millis();
        return false;
    }

    // Sensor needs about 5 seconds between readings
    if (millis() - lastSample < 5000) {
        return false;
    }

    // Attempt to read sensor values
    if (sensor.readMeasurement()) {

        co2ppm = sensor.getCO2();
        temperatureCelsius = sensor.getTemperature();
        relativeHumidity = sensor.getHumidity();

        lastSample = millis();
        return true;
    }

    // Recovery attempt if sensor stops responding
    if (millis() - lastRecovery > 30000) {

        Serial.println("SCD41 communication issue, attempting reset...");
        lastRecovery = millis();

        sensor.stopPeriodicMeasurement();
        delay(100);

        Wire.end();
        delay(100);
        Wire.begin();
        delay(100);

        if (!sensor.begin()) {
            Serial.println("Sensor restart failed");
            return false;
        }

        if (!sensor.startPeriodicMeasurement()) {
            Serial.println("Measurement restart failed");
            return false;
        }

        measurementActive = true;
        lastSample = millis();
    }

    return false;
}

// Getter functions
int EnvironmentSensor::getCO2() {
    return co2ppm;
}

float EnvironmentSensor::getTemperatureF() {
    return convertCtoF(temperatureCelsius);
}

float EnvironmentSensor::getHumidity() {
    return relativeHumidity;
}
