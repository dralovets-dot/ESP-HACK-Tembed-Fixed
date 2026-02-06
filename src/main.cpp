// ESP-HACK для LilyGO T-Embed CC1101
// ИСПРАВЛЕННАЯ версия с проверками

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

// ============================================
// ПИНЫ LILYGO T-EMBED CC1101 (ОФИЦИАЛЬНЫЕ)
// ============================================

// Дисплей ST7789 (TFT)
#define TFT_BL          21

// Энкодер
#define ENCODER_A       4
#define ENCODER_B       5
#define ENCODER_BTN     0

// ============================================
// ГЛОБАЛЬНЫЕ ОБЪЕКТЫ
// ============================================

TFT_eSPI *tft = nullptr;

volatile int encoderPos = 0;
volatile int lastEncoderA = 0;

// ============================================
// ПРЕРЫВАНИЯ ДЛЯ ЭНКОДЕРА
// ============================================

void IRAM_ATTR encoderISR() {
  int A = digitalRead(ENCODER_A);
  int B = digitalRead(ENCODER_B);
  
  if (A != lastEncoderA) {
    if (B != A) {
      encoderPos++;
    } else {
      encoderPos--;
    }
  }
  lastEncoderA = A;
}

// ============================================
// ФУНКЦИИ
// ============================================

void initHardware() {
  // Последовательность ВАЖНА!
  
  // 1. Включение периферии (GPIO46) - ОБЯЗАТЕЛЬНО ПЕРВЫМ!
  pinMode(46, OUTPUT);
  digitalWrite(46, HIGH);
  delay(200); // Ждём стабилизации питания
  
  // 2. Подсветка дисплея
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  delay(50);
  
  // 3. Инициализация SPI
  SPI.begin(11, 10, 9, 41); // SCK, MISO, MOSI, CS для TFT
  
  // 4. Создаём и инициализируем TFT
  tft = new TFT_eSPI();
  if (tft == nullptr) {
    Serial.println("Failed to allocate TFT!");
    while(1) delay(100);
  }
  
  tft->init();
  tft->setRotation(1); // 0-3, попробуйте разные
  tft->fillScreen(TFT_BLACK);
  
  // 5. Инициализация энкодера
  pinMode(ENCODER_A, INPUT_PULLUP);
  pinMode(ENCODER_B, INPUT_PULLUP);
  pinMode(ENCODER_BTN, INPUT_PULLUP);
  
  lastEncoderA = digitalRead(ENCODER_A);
  
  // Не используем прерывания пока — polling в loop
}

void drawTestScreen() {
  if (tft == nullptr) return;
  
  tft->fillScreen(TFT_BLACK);
  tft->setTextSize(2);
  tft->setTextColor(TFT_GREEN, TFT_BLACK);
  tft->setCursor(20, 60);
  tft->print("T-Embed CC1101");
  
  tft->setTextSize(1);
  tft->setTextColor(TFT_WHITE, TFT_BLACK);
  tft->setCursor(20, 100);
  tft->print("Hardware initialized OK!");
  
  tft->setCursor(20, 130);
  tft->print("Encoder test: rotate knob");
  
  tft->setCursor(20, 160);
  tft->print("Pos: 0");
}

void updateEncoderDisplay() {
  static int lastPos = 0;
  if (encoderPos != lastPos && tft != nullptr) {
    lastPos = encoderPos;
    
    // Стераем старое значение
    tft->fillRect(60, 160, 100, 20, TFT_BLACK);
    tft->setCursor(60, 160);
    tft->print(encoderPos);
  }
}

void checkButton() {
  static bool lastState = HIGH;
  bool currentState = digitalRead(ENCODER_BTN);
  
  if (lastState == HIGH && currentState == LOW) {
    // Кнопка нажата
    if (tft != nullptr) {
      tft->fillScreen(TFT_BLUE);
      tft->setTextSize(3);
      tft->setTextColor(TFT_WHITE, TFT_BLUE);
      tft->setCursor(50, 100);
      tft->print("BUTTON!");
      delay(500);
      drawTestScreen();
    }
  }
  lastState = currentState;
}

// ============================================
// SETUP И LOOP
// ============================================

void setup() {
  // Отключаем WiFi и BT для экономии питания и стабильности
  WiFi.mode(WIFI_OFF);
  btStop();
  
  Serial.begin(115200);
  delay(500); // Важная задержка для стабилизации USB
  
  Serial.println("\n\n=================================");
  Serial.println("ESP-HACK T-Embed CC1101 Starting...");
  Serial.println("=================================");
  
  // Инициализация
  initHardware();
  
  // Тестовый экран
  drawTestScreen();
  
  Serial.println("Init complete!");
  Serial.println("Rotate encoder or press button");
}

void loop() {
  // Чтение энкодера (polling)
  int currentA = digitalRead(ENCODER_A);
  if (currentA != lastEncoderA) {
    int B = digitalRead(ENCODER_B);
    if (B != currentA) {
      encoderPos++;
    } else {
      encoderPos--;
    }
    lastEncoderA = currentA;
    updateEncoderDisplay();
  }
  
  // Проверка кнопки
  checkButton();
  
  delay(5); // Дебаунс
}
