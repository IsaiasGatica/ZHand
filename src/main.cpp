#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <ESP32Servo.h>
#include <webServer.cpp>
#include <Wire.h>
#include <SPI.h>

#define EMGRecord

Servo myservo;

void setup()
{
  Serial.begin(115200);
  initFS();
  initWiFi();

  myservo.attach(16);

  initWebSocket();
  loadServer();
  server.begin();
}

void loop()
{

  ws.cleanupClients();

  myservo.write(SPosicion);
}