// EDITED_FROM: https://github.com/adidax/dht11, fit for msp430f5529
//
//    FILE: dht11.h
// VERSION: 0.4.1
// PURPOSE: DHT11 Temperature & Humidity Sensor library for Arduino
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET: http://www.micro4you.com/files/sensor/DHT11.pdf
//
//     URL: http://playground.arduino.cc/Main/DHT11Lib
//
// HISTORY:
// George Hadjikyriacou - Original version
// see dht.cpp file
//

#ifndef DHT11_H
#define DHT11_H

#include "types.h"

enum DHT11Status {
  DHT_STATUS_OK,
  DHT_STATUS_ERROR_TIMEOUT,
  DHT_STATUS_ERROR_CHECKSUM,
};

class DHT11 {
private:
  gpio pin;

  int humidity;
  int temperature;
  DHT11Status status;

  inline DHT11Status handShake();

public:
  explicit DHT11(gpio pin);

  DHT11Status read();

  inline int getHumidity() const { return humidity; };

  inline int getTemperature() const { return temperature; };

  inline DHT11Status getStatus() const { return status; };
};

#endif