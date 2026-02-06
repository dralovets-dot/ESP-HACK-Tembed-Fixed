// ESP-HACK для LilyGO T-Embed CC1101
// Тестовая версия с правильными пинами

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <RotaryEncoder.h>
#include <SPI.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>

// ============================================
// ПИНЫ LILYGO T-EMBED CC1101 (ОФИЦИАЛЬНЫЕ)
// ============================================

// CC1101 (Sub-GHz)
#define CC1101_GDO0     3
#define CC1101_GDO2     38
#define CC1101_CS       12
#define CC1101_MOSI     9
#define CC1101_MISO     10
#define CC1101_SCK      11

// Дисплей ST7789 (TFT)
#define TFT_BL          21

// Энкодер
#define ENCODER_A       4
#define ENCODER_B       5
#define ENCODER_BTN     0

// SD карта
#define SD_CS           13

// ============================================
// ГЛОБАЛЬНЫЕ ОБЪЕКТЫ
// ============================================

TFT_eSPI tft = TFT_eSPI();
RotaryEncoder encoder(ENCODER_A, ENCODER_B, RotaryEncoder::LatchMode::TWO03);

// Переменные для меню
int menuIndex = 0;
const char* menuItems[] = {"WiFi", "Bluetooth", "SubGHz", "IR", "GPIO", "Settings"};
const int menuCount = 6;

// ============================================
// ФУНКЦИИ
// ============================================

void initHardware() {
  // Включение периферии (GPIO46)
  pinMode(46, OUTPUT);
  digitalWrite(46, HIGH);
  delay(100);
  
  // Подсветка дисплея
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  
  // Инициализация TFT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
  // Инициализация энкодера
  pinMode(ENCODER_BTN, INPUT_PULLUP);
  
  // Инициализация SPI для CC1101
  SPI.begin(CC1101_SCK, CC1101_MISO, CC1101_MOSI, CC1101_CS);
}

void drawMenu() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("ESP-HACK T-Embed");
  tft.drawLine(10, 40, 310, 40, TFT_WHITE);
  
  // Рисуем пункты меню
  for (int i = 0; i < menuCount; i++) {
    int y = 60 + i * 35;
    if (i == menuIndex) {
      tft.fillRect(5, y - 5, 310, 30, TFT_WHITE);
      tft.setTextColor(TFT_BLACK, TFT_WHITE);
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft.setCursor(20, y);
    tft.print(menuItems[i]);
  }
}

void checkEncoder() {
  encoder.tick();
  RotaryEncoder::Direction dir = encoder.getDirection();
  
  if (dir == RotaryEncoder::Direction::CLOCKWISE) {
    menuIndex++;
    if (menuIndex >= menuCount) menuIndex = 0;
    drawMenu();
    delay(150); // Дебаунс
  }
  else if (dir == RotaryEncoder::Direction::COUNTERCLOCKWISE) {
    menuIndex--;
    if (menuIndex < 0) menuIndex = menuCount - 1;
    drawMenu();
    delay(150);
  }
  
  // Кнопка энкодера
  if (digitalRead(ENCODER_BTN) == LOW) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(50, 100);
    tft.print("SELECTED:");
    tft.setCursor(50, 150);
    tft.print(menuItems[menuIndex]);
    delay(1000);
    drawMenu();
  }
}

// ============================================
// SETUP И LOOP
// ============================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP-HACK T-Embed CC1101 Starting...");
  
  initHardware();
  
  // Тестовый экран
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(20, 80);
  tft.print("T-Embed");
  tft.setCursor(20, 130);
  tft.print("CC1101");
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(20, 200);
  tft.print("Hardware OK!");
  
  delay(2000);
  drawMenu();
  
  Serial.println("Init complete. Use encoder to navigate.");
}

void loop() {
  checkEncoder();
  delay(10);
}
