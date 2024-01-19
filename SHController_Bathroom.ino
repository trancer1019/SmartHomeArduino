#include "DeviceBase.h"  //Базовый класс для всех устройствhome
#include "Relay.h"
#include "Relay2.h"
#include "OutR_0_10v.h"
#include "OutPWM.h"
#include "FloorHeating.h"

#include "LEDController.h"
#include "SH_Rs485Controller.h"

#define DEVICEADRESS 01  // Адрес устройства

#define RS485_DE_RE 22  //пин DE_RE для RS485
#define RS485_RX 16     //пин RX для RS485
#define RS485_TX 17     //пин TX для RS485

#define BoardLedPin 2  // Пин светодиода на плате

///-----------------------------------------------------
Relay relay2__2_nc(19);  // Реле вых2|вык
Relay relay3__3_nc(18);  // Реле вых3|вык
Relay relay4__4_nc(5);   // Реле вых4|вык

Relay relay5__51_52(12);        // Реле вых51|вых52
Relay relay6__61_62(14);        // Реле вых61|вых62
Relay2 relay7_8__71_72(4, 13);  // Реле вых71|вых72

OutR_0_10v outr_0_10v(15, 25);  //Выход 0-10В + реле

OutPWM outpwm1(26);  //Выход ШИМ 1
OutPWM outpwm2(23);  //Выход ШИМ 2

FloorHeating floorheating(36, 21);  //Термодатчик + Реле вых1

DeviceBase* devices[] = { &relay2__2_nc, &relay3__3_nc, &relay4__4_nc, &relay5__51_52, &relay6__61_62, &relay7_8__71_72, &outr_0_10v, &outpwm1, &outpwm2 };

///-----------------------------------------------------
LEDController boardLedController(BoardLedPin);                                      // Создание объекта класса для управления светодиодом, подключенным к пину BoardLedPin
SH_Rs485Controller rs485Controller(DEVICEADRESS, RS485_DE_RE, RS485_RX, RS485_TX);  // Создание объекта класса для обмена по rs485

///-----------------------------------------------------
void setup() {
  Serial.begin(115200);

  boardLedController.blink();  //мигнуть светодиодом
}

void loop() {
  floorheating.upd();           //выполнение функции обновления соостояни
  boardLedController.update();  //выполнение функции обновления соостояни
  rs485Controller.update();     //выполнение функции обновления соостояни
}

///-----------------------------------------------------
//функция для записи регистра
void deviceWriteRegister(uint8_t registerNumber, uint8_t registerValue) {
  boardLedController.blink();

  if (registerNumber == 0xFF && registerValue == 0xFA) updLoop();  //переводим в режим программирования

  if (registerNumber < 1 || registerNumber > sizeof(devices) / sizeof(devices[0])) return;  //проверка на наличие номера устройства

  devices[registerNumber - 1]->set(registerValue);
}

//функция для чтения регистра
uint8_t deviceReadRegister(uint8_t registerNumber) {
  boardLedController.blink();

  if (registerNumber < 1 || registerNumber > sizeof(devices) / sizeof(devices[0])) return 0;  //проверка на наличие номера устройства

  uint8_t registerValue = devices[registerNumber - 1]->get();
  return registerValue;
}


///--------ОБНОВЛЕНЕИ ПРОШИВКИ ПО WI-FI-------------------------------------
/*
  Rui Santos
  Complete project details
   - Arduino IDE: https://RandomNerdTutorials.com/esp32-ota-over-the-air-arduino/
   - VS Code: https://RandomNerdTutorials.com/esp32-ota-over-the-air-vs-code/
  
  This sketch shows a Basic example from the AsyncElegantOTA library: ESP32_Async_Demo
  https://github.com/ayushsharma82/AsyncElegantOTA
*/

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
 
const char* ssid = "RedmiKulygin";
const char* password = "12580000";
 
AsyncWebServer server(80);

void updLoop() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Ожидаем соединения
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "ESP32. Контроллер №" + String(DEVICEADRESS));
  });
 
  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");

  while (1) {
    AsyncElegantOTA.loop();
  }
}