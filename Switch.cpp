#include "Switch.h"


// Реализация конструктора
Switch::Switch(uint8_t PIN)  : PIN(PIN) {
  pinMode(PIN, INPUT);
  //set(0);
}


//назначить состояние
void Switch::set(uint8_t state) 
{
  /*this->state = state;
  if(state==0) {
    digitalWrite(PIN, false);
  }
  else {
    digitalWrite(PIN, true);
  }*/
}

//считать состояние
uint8_t Switch::get() {
  int pinVal = digitalRead(PIN);
  return pinVal==0;
  //return state;
}