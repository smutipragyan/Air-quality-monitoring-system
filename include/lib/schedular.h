#pragma once

#ifndef SYSTEM_SCHEDULER_H
#define SYSTEM_SCHEDULER_H

#include <Arduino.h>

// Sensor modules
#include "include/sensor/pms7003_s.h"
#include "include/sensor/scd41_s.h"
#include "include/sensor/sgp30_s.h"

// System modules
#include "include/lib/mqtt_client.h"
#include "include/lib/display.h"
#include "include/lib/wifi_manager.h"

// Timing configuration
constexpr unsigned long DISPLAY_SLEEP_TIME = 300000;      // 5 minutes
constexpr unsigned long WIFI_TIMEOUT_MS    = 15000;       // 15 seconds
constexpr unsigned long MQTT_RETRY_DELAY   = 5000;        // retry after 5s
constexpr unsigned long MQTT_LONG_DELAY    = 900000;      // retry after 15min

constexpr int MAX_MQTT_RETRIES = 3;

// Hardware configuration
constexpr int USER_BUTTON_PIN = 0;   // ESP32 BOOT button (GPIO0)

// Interrupt handler
void IRAM_ATTR handleButtonPress();

class Scheduler {
private:

    // Latest sensor values
    static float tempF;
    static float relHumidity;
    static float vocLevel;
    static float hydrogen;
    static float ethanolGas;

    static int co2ppm;
    static int pm1;
    static int pm25;
    static int pm10;
    static int airQualityIndex;

    // Timing trackers
    static unsigned long lastDisplayUpdate;
    static unsigned long lastSerialPrint;
    static unsigned long lastMQTTSend;
    static unsigned long displayTimer;

    // System states
    static bool displayActive;
    static volatile bool displayToggleRequested;

    static bool mqttActive;
    static bool wifiConnected;

    static unsigned long lastConnectionTry;
    static int retryCounter;

    // Internal helpers
    static int computeAQI(int pm25, int pm10);
    static void readSensors();
    static void manageConnections();

public:

    // Initialize system components
    static void init();

    // Main scheduler loop
    static void run();

    // Display control
    static void requestDisplayToggle();
    static bool displayEnabled();
};

// Allow other modules (like OLED driver) to check display state
inline bool isOledOn() {
    return Scheduler::displayEnabled();
}

#endif
