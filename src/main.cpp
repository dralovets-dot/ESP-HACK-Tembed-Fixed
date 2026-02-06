// ESP-HACK для LilyGO T-Embed CC1101
// УПРОЩЁННАЯ версия без динамической памяти

#include <Arduino.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include <SPI.h>

// ============================================
// ПИНЫ (ОФИЦИАЛЬНЫЕ от LilyGO)
// ============================================
#define TFT_BL      21
#define ENC_A       4
#define ENC_B       5
#define ENC_BTN     0

// ============================================
// ОБЪЕКТЫ (статические, не указатели!)
// ============================================
TFT_eSPI tft;

volatile int encPos = 0;
int lastEncA = 0;

// ============================================
// ИНИЦИАЛИЗАЦИЯ
// ============================================
void setup() {
  // Отключаем WiFi/BT
  WiFi.mode(WIFI_OFF);
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting...");
  
  // 1. Питание периферии (ОБЯЗАТЕЛЬНО!)
  pinMode(46, OUTPUT);
  digitalWrite(46, HIGH);
  delay(300);
  
  // 2. Подсветка
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  delay(100);
  
  // 3. Инициализация TFT (без SPI.begin!)
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  
  // 4. Энкодер
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_BTN, INPUT_PULLUP);
  lastEncA = digitalRead(ENC_A);
  
  // Тестовый экран
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 50);
  tft.print("T-Embed OK!");
  tft.setTextSize(1);
  tft.setCursor(10, 90);
  tft.print("Rotate encoder");
  
  Serial.println("Ready!");
}

// ============================================
// ОСНОВНОЙ ЦИКЛ
// ============================================
void loop() {
  // Чтение энкодера
  int curA = digitalRead(ENC_A);
  if (curA != lastEncA) {
    int curB = digitalRead(ENC_B);
    if (curB != curA) encPos++;
    else encPos--;
    lastEncA = curA;
    
    // Вывод на экран
    tft.fillRect(10, 120, 150, 20, TFT_BLACK);
    tft.setCursor(10, 120);
    tft.print("Pos: ");
    tft.print(encPos);
  }
  
  // Кнопка
  if (digitalRead(ENC_BTN) == LOW) {
    tft.fillScreen(TFT_BLUE);
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
    tft.setCursor(40, 100);
    tft.print("BUTTON!");
    delay(300);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(10, 50);
    tft.print("T-Embed OK!");
  }
  
  delay(5);
}
