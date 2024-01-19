#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include <Arduino.h>  // Включение библиотеки Arduino для использования типа string

// Заголовок класса
class LEDController {
public:
  LEDController(int pin);  // Конструктор

  void blink();   //Функция для запуска однократного мигания
  void update();  //Функция для обновления состояния в основном loop

private:
  int ledPin;
  unsigned long startTime;
  const unsigned long ledDuration = 20;  // Длительность зажигания в миллисекундах
};

#endif
