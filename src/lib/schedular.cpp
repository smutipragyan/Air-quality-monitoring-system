#include "lib/scheduler.h"

// -------- Static variable initialization --------
float Scheduler::tempF = 0;
float Scheduler::relHumidity = 0;
float Scheduler::vocLevel = 0;
float Scheduler::hydrogen = 0;
float Scheduler::ethanolGas = 0;

int Scheduler::co2ppm = 0;
int Scheduler::pm1 = 0;
int Scheduler::pm25 = 0;
int Scheduler::pm10 = 0;
int Scheduler::airQualityIndex = 0;

bool Scheduler::displayActive = true;
volatile bool Scheduler::displayToggleRequested = false;

unsigned long Scheduler::lastDisplayUpdate = 0;
unsigned long Scheduler::lastSerialPrint = 0;
unsigned long Scheduler::lastMQTTSend = 0;
unsigned long Scheduler::displayTimer = 0;

bool Scheduler::mqttActive = false;
bool Scheduler::wifiConnected = false;

unsigned long Scheduler::lastConnectionTry = 0;
int Scheduler::retryCounter = 0;


// -------- AQI calculation --------
int Scheduler::computeAQI(int pm25Value, int pm10Value) {

    const int pm25Breakpoints[] = {0, 12, 35, 55, 150, 250, 500};
    const int pm25AQIValues[]   = {0, 50, 100, 150, 200, 300, 500};

    const int pm10Breakpoints[] = {0, 54, 154, 254, 354, 424, 604};
    const int pm10AQIValues[]   = {0, 50, 100, 150, 200, 300, 500};

    auto calculate = [](int value, const int bp[], const int aqi[]) {
        for (int i = 1; i < 7; i++) {
            if (value <= bp[i]) {
                return ((aqi[i] - aqi[i-1]) * (value - bp[i-1])) /
                       (bp[i] - bp[i-1]) + aqi[i-1];
            }
        }
        return 500;
    };

    int aqiPM25 = calculate(pm25Value, pm25Breakpoints, pm25AQIValues);
    int aqiPM10 = calculate(pm10Value, pm10Breakpoints, pm10AQIValues);

    return max(aqiPM25, aqiPM10);
}


// -------- System initialization --------
void Scheduler::init() {

    Serial.println("System scheduler started");

    GasSensor::initialize();
    EnvironmentSensor::initialize();
    ParticleSensor::initialize();

    OLEDDisplay::begin();

    displayTimer = millis();

    pinMode(USER_BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(USER_BUTTON_PIN), handleButtonPress, FALLING);

    lastDisplayUpdate = millis();
    lastSerialPrint = millis();
    lastMQTTSend = millis();

    manageConnections();
}


// -------- Network connection manager --------
void Scheduler::manageConnections() {

    if (!wifiConnected) {

        Serial.println("Connecting to WiFi...");

        if (WiFiManager::connect(WIFI_TIMEOUT_MS)) {
            wifiConnected = true;
            Serial.println("WiFi connected");

            if (MQTTClient::init()) {
                mqttActive = true;
                retryCounter = 0;
            }
        } else {
            retryCounter++;
        }
    }

    else if (!mqttActive) {

        if (MQTTClient::init()) {
            mqttActive = true;
            retryCounter = 0;
        } else {
            retryCounter++;
        }
    }

    if (retryCounter >= MAX_MQTT_RETRIES) {
        Serial.println("Connection retry limit reached");
        lastConnectionTry = millis();
        retryCounter = 0;
    }
}


// -------- Sensor update routine --------
void Scheduler::readSensors() {

    if (EnvironmentSensor::update()) {
        tempF = EnvironmentSensor::getTemperatureF();
        relHumidity = EnvironmentSensor::getHumidity();
        co2ppm = EnvironmentSensor::getCO2();
    }

    GasSensor::update();
    vocLevel = GasSensor::getTVOC();
    hydrogen = GasSensor::getHydrogen();
    ethanolGas = GasSensor::getEthanol();

    if (ParticleSensor::update()) {
        pm1 = ParticleSensor::getPM1();
        pm25 = ParticleSensor::getPM25();
        pm10 = ParticleSensor::getPM10();

        airQualityIndex = computeAQI(pm25, pm10);
    }
}


// -------- Main scheduler loop --------
void Scheduler::run() {

    unsigned long now = millis();

    if (mqttActive && wifiConnected) {
        MQTTClient::loop();
    }

    if (displayToggleRequested) {
        displayToggleRequested = false;
        displayActive = !displayActive;
        displayTimer = now;
    }

    if (displayActive && (now - lastDisplayUpdate >= 500)) {

        lastDisplayUpdate = now;

        readSensors();

        OLEDDisplay::render(
            tempF,
            relHumidity,
            co2ppm,
            pm1,
            pm25,
            pm10,
            airQualityIndex,
            vocLevel,
            hydrogen,
            ethanolGas
        );
    }

    if (now - lastSerialPrint >= 10000) {

        lastSerialPrint = now;

        Serial.print("Temp: "); Serial.print(tempF);
        Serial.print(" | Humidity: "); Serial.print(relHumidity);
        Serial.print(" | CO2: "); Serial.print(co2ppm);
        Serial.print(" | PM2.5: "); Serial.print(pm25);
        Serial.print(" | AQI: "); Serial.println(airQualityIndex);
    }

    if (mqttActive && wifiConnected && now - lastMQTTSend >= 60000) {

        lastMQTTSend = now;

        MQTTClient::publish("homeassistant/sensor/esp32_temperature/state", String(tempF));
        MQTTClient::publish("homeassistant/sensor/esp32_humidity/state", String(relHumidity));
        MQTTClient::publish("homeassistant/sensor/esp32_co2/state", String(co2ppm));
        MQTTClient::publish("homeassistant/sensor/esp32_pm25/state", String(pm25));
        MQTTClient::publish("homeassistant/sensor/esp32_aqi/state", String(airQualityIndex));
    }
}


// -------- Display toggle request --------
void Scheduler::requestDisplayToggle() {
    displayToggleRequested = true;
}

bool Scheduler::displayEnabled() {
    return displayActive;
}


// -------- Button interrupt --------
void IRAM_ATTR handleButtonPress() {
    Scheduler::requestDisplayToggle();
}
