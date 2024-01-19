#ifndef DEVICEBASE_H
#define DEVICEBASE_H

class DeviceBase {
public:
  virtual void set(uint8_t state) = 0;
  virtual uint8_t get() = 0;
};

#endif