#ifndef SH_RS485CONTROLLER_H
#define SH_RS485CONTROLLER_H

#include <Arduino.h>  // Включение библиотеки Arduino для использования типа string

// Заголовок класса
class SH_Rs485Controller {
public:
  SH_Rs485Controller(uint8_t DeviceAdress, uint8_t Rs485DeRePin, uint8_t Rs485RxPin, uint8_t Rs485TxPin);  // Конструктор

  void update();  //Функция для обновления состояния в основном loop

private:
  const uint8_t DeviceAdress;  //адрес устройства
  const uint8_t Rs485DeRePin;  //пин DE_RE для RS485
  const uint8_t Rs485RxPin;    //пин RX для RS485
  const uint8_t Rs485TxPin;    //пин TX для RS485

  uint8_t calculateCRC4(const uint8_t *data, size_t length);

  bool checkCRC(uint8_t CRC, uint8_t NCOM, uint8_t registerNumber, uint8_t registerValue);
  bool checkCRC(uint8_t CRC, uint8_t NCOM, uint8_t registerNumber);

  void rs485Send(const uint8_t *buffer, size_t size);

  void sendRegstate(uint8_t registerNumber, uint8_t registerValue);
  void sendConfirm(uint8_t registerNumber);
  
  void writeRegister(uint8_t registerNumber, uint8_t registerValue);
  void readRegister(uint8_t registerNumber);
};

#endif
