#ifndef PMS7003_S_H
#define PMS7003_S_H

#include <Arduino.h>
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

    // Decode the 32-byte frame from the sensor
    static void decodeFrame(uint8_t* frameBuffer);

public:
    // Initialize UART communication
    static void initialize();

    // Read data from sensor
    static bool update();

    // Check if new data is available
    static bool available();

    // Getter functions
    static int getPM1();
    static int getPM25();
    static int getPM10();
};

#endif
