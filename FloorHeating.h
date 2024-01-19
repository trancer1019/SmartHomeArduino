#ifndef FLOORHEATING_H
#define FLOORHEATING_H

#include <Arduino.h> // Включение библиотеки Arduino для использования типа string
#include "DeviceBase.h" //Базовый класс для всех устройств

// Заголовок класса
class FloorHeating : public DeviceBase {
public:
    FloorHeating(uint8_t SENSPIN, uint8_t RPIN);  // Конструктор
    
    void set(uint8_t state); //назначить требуемую темпиратуру
    uint8_t get(); //считать состояние

    void upd(); //обновить состояние
    
private:
    const uint8_t SENSPIN; //вывод для управления реле
    const uint8_t RPIN; //вывод для управления реле
    uint8_t state; //состояние 

    float gettemp(); //считать текущую темпиратуру
};

#endif
