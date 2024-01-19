#include "Relay.h"


// Реализация конструктора
Relay::Relay(uint8_t PIN)  : PIN(PIN) {
  pinMode(PIN, OUTPUT);
  set(0);
}


//назначить состояние
void Relay::set(uint8_t state) 
{
  this->state = state;
  if(state==0) {
    digitalWrite(PIN, false);
  }
  else {
    digitalWrite(PIN, true);
  }
}

//считать состояние
uint8_t Relay::get() {
  return state;
}