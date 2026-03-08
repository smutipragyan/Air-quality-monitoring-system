#include "lib/mqtt_client.h"

// Static object initialization
WiFiClient MQTTClient::espClient;
PubSubClient MQTTClient::client(espClient);
bool MQTTClient::initialized = false;

// Initialize MQTT connection
bool MQTTClient::init() {

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("MQTT init aborted: WiFi not connected");
        return false;
    }

    if (!initialized) {
        client.setServer(MQTT_SERVER, MQTT_PORT);
        initialized = true;
    }

    if (client.connected()) {
        return true;
    }

    Serial.print("Attempting MQTT connection... ");

    bool success = client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASS);

    if (!success) {
        Serial.print("failed (state=");
        Serial.print(client.state());
        Serial.println(")");
        return false;
    }

    Serial.println("connected");

    // Home Assistant sensor discovery topics
    client.publish("homeassistant/sensor/esp32_temp/config",
        "{\"name\":\"ESP32 Temperature\",\"state_topic\":\"homeassistant/sensor/esp32_temperature/state\",\"unit_of_measurement\":\"°F\",\"device_class\":\"temperature\"}");

    client.publish("homeassistant/sensor/esp32_humidity/config",
        "{\"name\":\"ESP32 Humidity\",\"state_topic\":\"homeassistant/sensor/esp32_humidity/state\",\"unit_of_measurement\":\"%\",\"device_class\":\"humidity\"}");

    client.publish("homeassistant/sensor/esp32_co2/config",
        "{\"name\":\"ESP32 CO2\",\"state_topic\":\"homeassistant/sensor/esp32_co2/state\",\"unit_of_measurement\":\"ppm\",\"device_class\":\"carbon_dioxide\"}");

    client.publish("homeassistant/sensor/esp32_pm1/config",
        "{\"name\":\"ESP32 PM1.0\",\"state_topic\":\"homeassistant/sensor/esp32_pm1_0/state\",\"unit_of_measurement\":\"µg/m³\"}");

    client.publish("homeassistant/sensor/esp32_pm25/config",
        "{\"name\":\"ESP32 PM2.5\",\"state_topic\":\"homeassistant/sensor/esp32_pm2_5/state\",\"unit_of_measurement\":\"µg/m³\"}");

    client.publish("homeassistant/sensor/esp32_pm10/config",
        "{\"name\":\"ESP32 PM10\",\"state_topic\":\"homeassistant/sensor/esp32_pm10/state\",\"unit_of_measurement\":\"µg/m³\"}");

    client.publish("homeassistant/sensor/esp32_aqi/config",
        "{\"name\":\"ESP32 AQI\",\"state_topic\":\"homeassistant/sensor/esp32_aqi/state\",\"unit_of_measurement\":\"AQI\"}");

    client.publish("homeassistant/sensor/esp32_tvoc/config",
        "{\"name\":\"ESP32 TVOC\",\"state_topic\":\"homeassistant/sensor/esp32_tvoc/state\",\"unit_of_measurement\":\"ppb\"}");

    client.publish("homeassistant/sensor/esp32_h2/config",
        "{\"name\":\"ESP32 Hydrogen\",\"state_topic\":\"homeassistant/sensor/esp32_h2/state\"}");

    client.publish("homeassistant/sensor/esp32_ethanol/config",
        "{\"name\":\"ESP32 Ethanol\",\"state_topic\":\"homeassistant/sensor/esp32_ethanol/state\"}");

    return true;
}

// Check MQTT status
bool MQTTClient::isConnected() {
    return client.connected();
}

// Publish message
bool MQTTClient::publish(const char* topic, const String& payload) {

    if (!client.connected()) {
        return false;
    }

    return client.publish(topic, payload.c_str());
}

// Disconnect from broker
void MQTTClient::disconnect() {
    if (client.connected()) {
        client.disconnect();
    }
}

// Maintain MQTT connection
void MQTTClient::loop() {
    if (client.connected()) {
        client.loop();
    }
}
