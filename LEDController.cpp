#include "LEDController.h"


// Реализация конструктора
LEDController::LEDController(int pin)
  : ledPin(pin) {
  pinMode(ledPin, OUTPUT);
}


// Функция для запуска однократного мигания
void LEDController::blink() {
  digitalWrite(ledPin, HIGH);
  startTime = millis();
}

// Функция для обновления состояния в основном loop
void LEDController::update() {
  // Проверка времени зажигания светодиода
  if (millis() - startTime >= ledDuration) {
    // Выключение светодиода после заданной длительности
    digitalWrite(ledPin, LOW);
  }
}
