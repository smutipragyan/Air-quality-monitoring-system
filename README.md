# Air-quality-monitoring-system
Overview

This project is an IoT-based Air Quality Monitoring System built using an ESP32 microcontroller.
The system measures multiple environmental parameters such as CO₂ concentration, particulate matter (PM1.0 / PM2.5 / PM10), temperature, humidity, and volatile organic compounds (TVOC).

Sensor data is displayed locally on an OLED display and can also be transmitted to a Home Assistant dashboard using MQTT for remote monitoring.

The project demonstrates sensor interfacing, embedded firmware design, and IoT communication using ESP32.

Features

Real-time air quality monitoring

Multi-sensor integration

OLED display visualization

MQTT data publishing

WiFi connectivity

Automatic reconnection handling

AQI calculation based on PM2.5 and PM10

Modular firmware architecture

Hardware Components

ESP32 Development Board

PMS7003 Particulate Matter Sensor

SCD41 CO₂ / Temperature / Humidity Sensor

SGP30 Gas Sensor (TVOC, Hydrogen, Ethanol)

0.96" OLED Display (SSD1306)

Breadboard and jumper wires
