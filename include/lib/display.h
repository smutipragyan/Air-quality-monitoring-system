#pragma once

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "scheduler.h"

// OLED configuration
constexpr uint8_t OLED_WIDTH  = 128;
constexpr uint8_t OLED_HEIGHT = 64;
constexpr int OLED_RST_PIN    = -1;     // Reset not used
constexpr uint8_t OLED_I2C_ADDR = 0x3C; // Typical address for 0.96" OLED

class OLEDDisplay {
private:
    // Static display object shared across the program
    static Adafruit_SSD1306 display;

public:
    // Initialize OLED hardware
    static void begin();

    // Update screen with latest sensor readings
    static void render(float temperatureF,
                       float humidity,
                       int co2,
                       int pm1,
                       int pm25,
                       int pm10,
                       int aqi,
                       float tvoc,
                       float h2,
                       float ethanol);
};
