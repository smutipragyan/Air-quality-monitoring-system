#include "lib/display.h"

// Initialize static display object
Adafruit_SSD1306 OLEDDisplay::display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RST_PIN);

void OLEDDisplay::begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
        Serial.println("OLED initialization failed");
        return;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

void OLEDDisplay::render(float temperatureF,
                         float humidity,
                         int co2,
                         int pm1,
                         int pm25,
                         int pm10,
                         int aqi,
                         float tvoc,
                         float h2,
                         float ethanol) {

    if (!isOledOn()) {
        return;  // Skip update if display is disabled
    }

    display.clearDisplay();
    display.setCursor(0, 0);

    display.print("Temp: ");
    display.print(temperatureF, 1);
    display.println(" F");

    display.print("Humidity: ");
    display.print(humidity, 1);
    display.println(" %");

    display.print("CO2: ");
    display.print(co2);
    display.println(" ppm");

    display.print("PM: ");
    display.print(pm1);
    display.print("/");
    display.print(pm25);
    display.print("/");
    display.print(pm10);
    display.println(" ug");

    display.print("AQI: ");
    display.println(aqi);

    display.print("TVOC: ");
    display.print(tvoc, 0);
    display.println(" ppb");

    display.print("H2: ");
    display.print(h2, 0);
    display.println(" res");

    display.print("Ethanol: ");
    display.print(ethanol, 0);
    display.println(" res");

    display.display();
}
