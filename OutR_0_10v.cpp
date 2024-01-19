#include "OutR_0_10v.h"


// Реализация конструктора
OutR_0_10v::OutR_0_10v(uint8_t RPIN, uint8_t DACPIN)  : RPIN(RPIN), DACPIN(DACPIN) {
  pinMode(RPIN, OUTPUT);
  set(0);
}


//назначить состояние
void OutR_0_10v::set(uint8_t state) 
{
  this->state = state;

  if(state==0) {
    digitalWrite(RPIN, false);
    dacWrite(DACPIN, 0);
  }
  else {
    digitalWrite(RPIN, true);
    dacWrite(DACPIN, state);
  }
}

//считать состояние
uint8_t OutR_0_10v::get() {
  return state;
}