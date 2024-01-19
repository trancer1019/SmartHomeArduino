#ifndef RELAY2_H
#define RELAY2_H

#include <Arduino.h> // Включение библиотеки Arduino для использования типа string
#include "DeviceBase.h" //Базовый класс для всех устройств

// Заголовок класса
class Relay2 : public DeviceBase {
public:
    Relay2(uint8_t R1PIN, uint8_t R2PIN);  // Конструктор
    
    void set(uint8_t state); //назначить состояние реле
    uint8_t get(); //считать состояние
    
private:
    const uint8_t R1PIN; //вывод для управления реле
    const uint8_t R2PIN; //вывод для управления реле
    uint8_t state; //состояние 
};

#endif
