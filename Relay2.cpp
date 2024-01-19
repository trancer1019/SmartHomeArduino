#include "Relay2.h"


// Реализация конструктора
Relay2::Relay2(uint8_t R1PIN, uint8_t R2PIN)  : R1PIN(R1PIN), R2PIN(R2PIN) {
  pinMode(R1PIN, OUTPUT);
  pinMode(R2PIN, OUTPUT);
  set(0);
}


//назначить состояние
void Relay2::set(uint8_t state) 
{
  this->state = state;

  if(state==0) {
    digitalWrite(R1PIN, false);
    digitalWrite(R2PIN, false);
  }
  else if(state==1) {
    digitalWrite(R1PIN, true);
    digitalWrite(R2PIN, false);
  }
  else {
    digitalWrite(R1PIN, true);
    digitalWrite(R2PIN, true);
  }
}

//считать состояние
uint8_t Relay2::get() {
  return state;
}