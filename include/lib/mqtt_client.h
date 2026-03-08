#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

// MQTT configuration
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "ESP32_AirQuality"

class MQTTClient {
private:
    static WiFiClient espClient;
    static PubSubClient client;
    static bool initialized;

public:
    // Initialize MQTT connection
    static bool init();

    // Check if MQTT is connected
    static bool isConnected();

    // Publish message to MQTT topic
    static bool publish(const char* topic, const String& payload);

    // Disconnect MQTT
    static void disconnect();

    // Maintain MQTT connection
    static void loop();
};

#endif
