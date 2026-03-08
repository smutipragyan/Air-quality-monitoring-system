#pragma once

#ifndef PARTICLE_SENSOR_H
#define PARTICLE_SENSOR_H

#include <HardwareSerial.h>

// UART pin configuration for PMS7003 dust sensor
constexpr int PARTICLE_SENSOR_RX = 14;   // ESP32 receives data
constexpr int PARTICLE_SENSOR_TX = 27;   // ESP32 sends data

class ParticleSensor {
private:
    static HardwareSerial serialPort;

    // Latest particulate readings
    static int pm1;
    static int pm25;
    static int pm10;

    static bool dataReady;

    // Internal function to decode sensor frame
    static void decodeFrame(uint8_t* frameBuffer);

public:
    // Initialize UART communication with the sensor
    static void initialize();

    // Read incoming data from sensor
    static bool update();

    // Check if new measurement is available
    static bool available();

    // Getter functions
    static int getPM1();
    static int getPM25();
    static int getPM10();
};

#endif
