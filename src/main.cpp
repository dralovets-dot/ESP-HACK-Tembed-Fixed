#include <Arduino.h>
#include <WiFi.h>
#include <SPI.h>

// Настройки ДО подключения TFT_eSPI
#define USER_SETUP_LOADED
#include "User_Setup.h"
#include "TFT_eSPI.h"

#define BOARD_LORA_CS   12
#define BOARD_SD_CS     13

TFT_eSPI tft;

void setup() {
    WiFi.mode(WIFI_OFF);
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("Starting...");
    
    // 1. Включаем питание периферии (ОБЯЗАТЕЛЬНО!)
    pinMode(46, OUTPUT);
    digitalWrite(46, HIGH);
    delay(500);
    
    // 2. Подтягиваем ВСЕ CS к HIGH (важно!)
    pinMode(BOARD_LORA_CS, OUTPUT);
    digitalWrite(BOARD_LORA_CS, HIGH);
    pinMode(BOARD_SD_CS, OUTPUT);
    digitalWrite(BOARD_SD_CS, HIGH);
    
    // 3. Инициализируем SPI вручную ДО tft.begin()
    Serial.println("Init SPI...");
    SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS); // SCK, MISO (не используется), MOSI, SS
    
    // 4. Теперь инициализируем TFT
    Serial.println("Init TFT...");
    
    // Подсветка
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    
    // Создаём TFT с явными размерами
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    
    Serial.println("TFT OK!");
    
    // Тест
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 100);
    tft.print("WORKING!");
}

void loop() {
    delay(1000);
}
