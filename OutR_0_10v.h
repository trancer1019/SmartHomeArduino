#ifndef OUTR_0_10V_H
#define OUTR_0_10V_H

#include <Arduino.h> // Включение библиотеки Arduino для использования типа string
#include "DeviceBase.h" //Базовый класс для всех устройств

// Заголовок класса
class OutR_0_10v : public DeviceBase {
public:
    OutR_0_10v(uint8_t RPIN, uint8_t DACPIN);  // Конструктор
    
    void set(uint8_t state); //назначить состояние реле
    uint8_t get(); //считать состояние
    
private:
    const uint8_t RPIN; //вывод для управления реле
    const uint8_t DACPIN; //вывод для управления ЦАП
    uint8_t state; //состояние 
};

#endif
