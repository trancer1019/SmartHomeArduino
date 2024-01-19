#include "OutPWM.h"


// Реализация конструктора
OutPWM::OutPWM(uint8_t PIN)  : PIN(PIN) {
  pinMode(PIN, OUTPUT);  // Установка порта на выход
  set(0);
}


//назначить состояние
void OutPWM::set(uint8_t state) 
{
  this->state = state;

  analogWrite(PIN, state);
}

//считать состояние
uint8_t OutPWM::get() {
  return state;
}