#include "FloorHeating.h"

#define hysteresis 1.0  // Значение гистерезиса

// Реализация конструктора
FloorHeating::FloorHeating(uint8_t SENSPIN, uint8_t RPIN)  : SENSPIN(SENSPIN), RPIN(RPIN) {
  pinMode(RPIN, OUTPUT);
  set(0);
}

//считать значение
float FloorHeating::gettemp()
{
  const double VCC = 3.3;             // on board 3.3v vcc
  const double R2 = 10000;            // 10k ohm series resistor
  const double adc_resolution = 4095; // 12-bit adc
  
  const double A = 0.001129148;   // thermistor equation parameters
  const double B = 0.000234125;
  const double C = 0.0000000876741; 

  double Vout, Rth, temperature, temperature1, adc_value; 
  adc_value = analogRead(SENSPIN);  
  Vout = (adc_value * VCC) / adc_resolution;
  Rth = (VCC * R2 / Vout) - R2;

/*  Steinhart-Hart Thermistor Equation:
 *  Temperature in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3)
 *  where A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8  */
  temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)),3))));   // Temperature in kelvin
  temperature = temperature - 273.15;  // Temperature in degree celsius
  //temperature1=(temperature*1.8)+32;// Temperature in degree fahrenheight

  return temperature;
}

//назначить состояние
void FloorHeating::set(uint8_t state) 
{
  if(state<35) {
      this->state = state;
  }
  else {
      this->state = 35;
  }

  upd();
}

//считать состояние
uint8_t FloorHeating::get() {
  return gettemp();
}

//обновить состояние
void FloorHeating::upd()
{
  // Чтение текущей температуры
  float currentTemperature = gettemp();

  float upperLimit = state + hysteresis;
  float lowerLimit = state - hysteresis;

  // Применение гистерезиса
  if (currentTemperature > upperLimit) {
    digitalWrite(RPIN, LOW);  // Включаем реле
  } else if (currentTemperature < lowerLimit) {
    digitalWrite(RPIN, HIGH);  // Выключаем реле
  }
}
