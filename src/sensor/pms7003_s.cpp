#include "sensor/pms7003_s.h"

// Static member initialization
HardwareSerial ParticleSensor::serialPort(2);

int ParticleSensor::pm1 = 0;
int ParticleSensor::pm25 = 0;
int ParticleSensor::pm10 = 0;

bool ParticleSensor::dataReady = false;

// Initialize UART communication with PMS7003
void ParticleSensor::initialize() {

    serialPort.begin(9600, SERIAL_8N1, PARTICLE_SENSOR_RX, PARTICLE_SENSOR_TX);

    Serial.println("Particle sensor started");

    // Command to force sensor into active reporting mode
    uint8_t activateCmd[] = {0x42, 0x4D, 0xE1, 0x00, 0x01, 0x70};
    serialPort.write(activateCmd, sizeof(activateCmd));

    delay(1000);
}

// Read incoming frame from sensor
bool ParticleSensor::update() {

    static uint8_t frame[32];
    static int byteIndex = 0;

    dataReady = false;

    while (serialPort.available()) {

        uint8_t incoming = serialPort.read();

        if (byteIndex == 0 && incoming != 0x42)
            continue;

        if (byteIndex == 1 && incoming != 0x4D) {
            byteIndex = 0;
            continue;
        }

        frame[byteIndex++] = incoming;

        if (byteIndex >= 32) {

            decodeFrame(frame);

            byteIndex = 0;
            dataReady = true;

            return true;
        }
    }

    return false;
}

// Decode particulate data from the frame
void ParticleSensor::decodeFrame(uint8_t* frameBuffer) {

    if (frameBuffer[0] != 0x42 || frameBuffer[1] != 0x4D)
        return;

    pm1  = (frameBuffer[10] << 8) | frameBuffer[11];
    pm25 = (frameBuffer[12] << 8) | frameBuffer[13];
    pm10 = (frameBuffer[14] << 8) | frameBuffer[15];
}

// Check if fresh data is available
bool ParticleSensor::available() {
    return dataReady;
}

// Getter functions
int ParticleSensor::getPM1() {
    return pm1;
}

int ParticleSensor::getPM25() {
    return pm25;
}

int ParticleSensor::getPM10() {
    return pm10;
}
