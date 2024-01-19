#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h> // Включение библиотеки Arduino для использования типа string
#include "DeviceBase.h" //Базовый класс для всех устройств

// Заголовок класса
class Relay : public DeviceBase {
public:
    Relay(uint8_t PIN);  // Конструктор

    void set(uint8_t state); //назначить состояние реле
    uint8_t get(); //считать состояние

private:
    const uint8_t PIN; //вывод для управления реле
    uint8_t state; //состояние 
};

#endif
