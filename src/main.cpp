#include <Arduino.h>
#include <WiFi.h>

// ВАЖНО: Сначала подключаем настройки!
#include "User_Setup.h"
#include "TFT_eSPI.h"

// Официальные пины
#define BOARD_LORA_CS   12
#define BOARD_SD_CS     13

TFT_eSPI tft;

void setup() {
    WiFi.mode(WIFI_OFF);
    Serial.begin(115200);
    delay(500);
    
    // CS пины в HIGH
    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    pinMode(BOARD_SD_CS, OUTPUT);
    digitalWrite(BOARD_SD_CS, HIGH);
    pinMode(BOARD_LORA_CS, OUTPUT);
    digitalWrite(BOARD_LORA_CS, HIGH);
    
    // Питание
    pinMode(46, OUTPUT);
    digitalWrite(46, HIGH);
    delay(200);
    
    // Подсветка
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    
    Serial.println("Init TFT...");
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    
    Serial.println("TFT OK!");
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 80);
    tft.print("ESP-HACK OK!");
}

void loop() {
    delay(1000);
}
