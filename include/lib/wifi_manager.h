#pragma once

#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"   // Contains WiFi credentials

class WiFiManager {
public:
    // Establish WiFi connection within a given timeout
    static bool beginConnection(unsigned long timeoutMs);

    // Disconnect from the current WiFi network
    static void stopConnection();

    // Check current WiFi status
    static bool connected();

    // Synchronize device time using NTP servers
    static void updateTime();
};

#endif
