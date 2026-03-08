#include "lib/wifi_manager.h"

// Establish WiFi connection within a specified timeout
bool WiFiManager::connect(unsigned long timeout) {

    if (WiFi.status() == WL_CONNECTED) {
        return true;
    }

    Serial.print("Starting WiFi connection to: ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long startTime = millis();

    while ((millis() - startTime) < timeout) {

        if (WiFi.status() == WL_CONNECTED) {

            Serial.println();
            Serial.print("WiFi connected. Device IP: ");
            Serial.println(WiFi.localIP());

            return true;
        }

        Serial.print("*");
        delay(500);
    }

    Serial.println();
    Serial.println("WiFi connection attempt timed out");

    WiFi.disconnect(true);

    return false;
}


// Disconnect from WiFi network
void WiFiManager::disconnect() {
    if (WiFi.status() == WL_CONNECTED) {
        WiFi.disconnect(true);
        Serial.println("WiFi disconnected");
    }
}


// Check connection status
bool WiFiManager::isConnected() {
    return (WiFi.status() == WL_CONNECTED);
}


// Synchronize device time using NTP
void WiFiManager::syncNTP() {

    const long gmtOffset = -8 * 3600;
    const int daylightOffset = 0;

    configTime(gmtOffset, daylightOffset, "pool.ntp.org");

    struct tm timeInfo;

    if (!getLocalTime(&timeInfo)) {
        Serial.println("NTP synchronization failed");
        return;
    }

    Serial.println("System time synchronized using NTP");
}
