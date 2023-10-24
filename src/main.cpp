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
#include <Adafruit_ADS1X15.h>
#include <movingAvg.h>

int16_t adc0;
Servo myservo;
movingAvg avgTemp(30);
Adafruit_ADS1115 ads;

void setup()
{
  Serial.begin(115200);
  initFS();
  initWiFi();

  myservo.attach(16);
  avgTemp.begin();

  initWebSocket();

  loadServer();

  server.begin();

  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    while (1)
      ;
  }
}

void loop()
{

  // myservo.write(SPosicion);

  ws.cleanupClients();

  adc0 = ads.readADC_SingleEnded(0);
  int avg = avgTemp.reading(adc0);
  int sensorvalue = map(avg, 0, 3000, 30, 180);

  myservo.write(sensorvalue);
  Serial.print(">AD0:");
  Serial.println(adc0);
  Serial.println(" ");
  Serial.print(">Avg:");
  Serial.println(avg);
  Serial.println(" ");
  Serial.print(">map:");
  Serial.println(sensorvalue);
  Serial.println(" ");
}