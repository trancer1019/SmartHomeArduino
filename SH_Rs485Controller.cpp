#include "SH_Rs485Controller.h"

uint8_t in_packet_buffer_position = 0;  //текущая позиция в буфере приема
uint8_t in_packetCRC;                   //CRC принимаемого покета
uint8_t in_packetNCOM;                  //N команды принимаемого покета
uint8_t in_packetRegnum;                //Регистр принимаемого покета


// Реализация конструктора
SH_Rs485Controller::SH_Rs485Controller(uint8_t DeviceAdress, uint8_t Rs485DeRePin, uint8_t Rs485RxPin, uint8_t Rs485TxPin)
  : DeviceAdress(DeviceAdress), Rs485DeRePin(Rs485DeRePin), Rs485RxPin(Rs485RxPin), Rs485TxPin(Rs485TxPin) {
  pinMode(Rs485DeRePin, OUTPUT);
  Serial2.begin(115200, SERIAL_8N1, Rs485RxPin, Rs485TxPin);  // Настройка скорости передачи данных для UART2
}


//назначить состояние
void SH_Rs485Controller::update() {
  if (Serial2.available() > 0) {
    char incomingByte = Serial2.read();

    //-если байт начала покета
    if (incomingByte == 0xAA) {
      in_packet_buffer_position = 1;  //переключаем позицию
      //Serial.println("Позиция 0");    //TODO
    }
    //-позиция байта адреса устройства
    else if (in_packet_buffer_position == 1) {
      if (incomingByte == DeviceAdress)  //-если адрес текущего устройства
      {
        in_packet_buffer_position = 2;  //переключаем позицию
        //Serial.println("Позиция 1");    //TODO
      } else {
        in_packet_buffer_position = -1;  //сбрасываем позицию
        //Serial.println("Сброс 1");       //TODO
      }
    }
    //-позиция байта N_команды + CRC
    else if (in_packet_buffer_position == 2) {
      in_packetNCOM = incomingByte & 0x0F;  //считываем номер команды
      in_packetCRC = incomingByte >> 4;     //считываем CRC

      if (in_packetNCOM == 1 || in_packetNCOM == 2)  //если номер команды Чтение / Запись
      {
        in_packet_buffer_position = 3;  //переключаем позицию
        //Serial.println("Позиция 2. Номер команды " + String(in_packetNCOM) + ". CRC " + String(in_packetCRC));  //TODO
      } else {
        in_packet_buffer_position = -1;  //сбрасываем позицию
        //Serial.println("Сброс 2. Номер команды " + String(in_packetNCOM) + ". CRC " + String(in_packetCRC));  //TODO
      }
    }
    //-позиция байта Номер регистра
    else if (in_packet_buffer_position == 3) {
      if (in_packetNCOM == 1)  //команда чтения
      {
        if (checkCRC(in_packetCRC, in_packetNCOM, incomingByte))  //проверка CRC
        {
          //Serial.println("Позиция 3_1. Чтение регистра " + String((uint8_t)incomingByte));  //TODO
          readRegister(incomingByte);  //запуск команды чтения регистра
        }
        in_packet_buffer_position = -1;  //сбрасываем позицию
        //Serial.println("Сброс 3_1. Регистр " + String((uint8_t)incomingByte));  //TODO
      } else if (in_packetNCOM == 2)  //команда записи
      {
        in_packetRegnum = incomingByte;  //сохраняем номер регистра
        in_packet_buffer_position = 4;   //переключаем позицию
        //Serial.println("Позиция 3_2. Сохранение регистра " + String((uint8_t)incomingByte));  //TODO
      } else {
        in_packet_buffer_position = -1;  //сбрасываем позицию
        //Serial.println("Сброс 3_3. Регистр " + String((uint8_t)incomingByte));  //TODO
      }
    }
    //-позиция байта Данные регистра
    else if (in_packet_buffer_position == 4) {
      if (checkCRC(in_packetCRC, in_packetNCOM, in_packetRegnum, incomingByte))  //проверка CRC
      {
        //Serial.println("Позиция 4. Запись регистра " + String(in_packetRegnum) + ". Значение регистра" + String((uint8_t)incomingByte));  //TODO
        writeRegister(in_packetRegnum, incomingByte);  //запуск команды записи регистра
      }
      in_packet_buffer_position = -1;  //сбрасываем позицию
      //Serial.println("Сброс 4. Регистр" + String(in_packetRegnum) + ". Значение регистра" + String((uint8_t)incomingByte));  //TODO
    }
  }
}



///-------------------------------------------------------------
// функция вычисления CRC4
uint8_t SH_Rs485Controller::calculateCRC4(const uint8_t *data, size_t length) {
  uint8_t crc = 0;

  for (size_t i = 0; i < length; ++i) {
    crc ^= data[i];  // выполняем XOR с каждым байтом данных

    for (int j = 0; j < 4; ++j) {
      if (crc & 0x8) {
        crc = (crc << 1) ^ 0x3;  // полином x^4 + x^1 + 1
      } else {
        crc <<= 1;
      }
    }
  }

  return crc & 0xF;  // возвращаем младшие 4 бита CRC
}
//-функция проверки CRC
bool SH_Rs485Controller::checkCRC(uint8_t CRC, uint8_t NCOM, uint8_t registerNumber, uint8_t registerValue) {
  uint8_t data[] = { DeviceAdress, NCOM, registerNumber, registerValue };
  size_t dataLength = sizeof(data) / sizeof(data[0]);

  // Вычисление CRC4
  uint8_t crcResult = calculateCRC4(data, dataLength);

  // Сравнение рассчитанной CRC с переданной CRC
  //Serial.println("CRC: " + String(crcResult));  //TODO
  return (CRC == crcResult);
}
//-функция проверки CRC
bool SH_Rs485Controller::checkCRC(uint8_t CRC, uint8_t NCOM, uint8_t registerNumber) {
  uint8_t data[] = { DeviceAdress, NCOM, registerNumber };
  size_t dataLength = sizeof(data) / sizeof(data[0]);

  // Вычисление CRC4
  uint8_t crcResult = calculateCRC4(data, dataLength);

  // Сравнение рассчитанной CRC с переданной CRC
  //Serial.println("CRC: " + String(crcResult));  //TODO
  return (CRC == crcResult);
}

///-------------------------------------------------------------
//-функция для отправки данных по rs485
void SH_Rs485Controller::rs485Send(const uint8_t *buffer, size_t size) {
  digitalWrite(Rs485DeRePin, HIGH);
  Serial2.write(buffer, size);
  Serial2.flush();  //ждем завершения передачи
  digitalWrite(Rs485DeRePin, LOW);
}

//функция для отправки состояния запрашиваемого регистра
void SH_Rs485Controller::sendRegstate(uint8_t registerNumber, uint8_t registerValue) {
  if (registerValue == 0xAA) registerValue = registerValue + 1; //если значение регистра = 0xAA

  uint8_t data[] = { DeviceAdress, 0x03, registerNumber, registerValue };
  size_t dataLength = sizeof(data) / sizeof(data[0]);
  // Вычисление CRC4
  uint8_t crcResult = calculateCRC4(data, dataLength);

  uint8_t data_send[] = { 0xAA, DeviceAdress, crcResult << 4 | 0x03, registerNumber, registerValue };
  size_t data_sendLength = sizeof(data_send) / sizeof(data_send[0]);
  rs485Send(data_send, data_sendLength);
}

//функция для отправки подтвержения записи регистра
void SH_Rs485Controller::sendConfirm(uint8_t registerNumber) {
  uint8_t data[] = { DeviceAdress, 0x04, registerNumber };
  size_t dataLength = sizeof(data) / sizeof(data[0]);
  // Вычисление CRC4
  uint8_t crcResult = calculateCRC4(data, dataLength);

  uint8_t data_send[] = { 0xAA, DeviceAdress, crcResult << 4 | 0x04, registerNumber };
  size_t data_sendLength = sizeof(data_send) / sizeof(data_send[0]);
  rs485Send(data_send, data_sendLength);
}

extern uint8_t deviceReadRegister(uint8_t registerNumber);  //обявленеи внешней функции
//функция для чтения регистра и отправки ответа
void SH_Rs485Controller::readRegister(uint8_t registerNumber) {
  // Ваш код для чтения регистра и отправки ответа
  Serial.println("Reed " + String(registerNumber));

  uint8_t registerValue = deviceReadRegister(registerNumber);  //выполняем команду

  sendRegstate(registerNumber, registerValue);  //отправляем состояние регистра
}

extern void deviceWriteRegister(uint8_t registerNumber, uint8_t registerValue);  //обявленеи внешней функции
//функция для записи значения в регистр и отправки ответа
void SH_Rs485Controller::writeRegister(uint8_t registerNumber, uint8_t registerValue) {
  // Ваш код для записи значения в регистр и отправки ответа
  Serial.println("Write " + String(registerNumber) + " " + String(registerValue));

  deviceWriteRegister(registerNumber, registerValue);  //выполняем команду

  sendConfirm(registerNumber);  //отправлем подтверждение приема данных
}