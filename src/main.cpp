#include <Arduino.h>
#include <WiFi.h>
#include "TFT_eSPI.h"

// Официальные пины из даташита
#define BOARD_LORA_CS   12  // CC1101 CS
#define BOARD_SD_CS     13  // SD Card CS
#define TFT_CS          41  // Display CS (определится из User_Setup)

TFT_eSPI tft;

void setup(void) {
    // Отключаем WiFi для стабильности
    WiFi.mode(WIFI_OFF);
    
    Serial.begin(115200);
    delay(500);
    
    // ВАЖНО: Все CS пины в HIGH перед инициализацией!
    // Это предотвращает конфликты на SPI шине
    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    pinMode(BOARD_SD_CS, OUTPUT);
    digitalWrite(BOARD_SD_CS, HIGH);
    pinMode(BOARD_LORA_CS, OUTPUT);
    digitalWrite(BOARD_LORA_CS, HIGH);
    
    // Питание периферии (GPIO46)
    pinMode(46, OUTPUT);
    digitalWrite(46, HIGH);
    delay(200);
    
    // Подсветка
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);
    
    Serial.println("Init TFT...");
    
    // Инициализация дисплея (официальный метод)
    tft.begin();
    tft.setRotation(1); // 0-3, подберите под себя
    tft.fillScreen(TFT_BLACK);
    
    Serial.println("TFT OK!");
    
    // Тестовый вывод
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 80);
    tft.print("ESP-HACK");
    tft.setCursor(20, 120);
    tft.print("T-Embed");
    tft.setCursor(20, 160);
    tft.print("CC1101 OK!");
}

void loop(void) {
    // Мигаем экраном для теста
    delay(2000);
    tft.fillScreen(TFT_BLUE);
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.setCursor(50, 150);
    tft.print("TEST OK");
    delay(1000);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(20, 80);
    tft.print("ESP-HACK");
    tft.setCursor(20, 120);
    tft.print("T-Embed");
    tft.setCursor(20, 160);
    tft.print("CC1101 OK!");
}
