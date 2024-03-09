#include "dht11/dht11.h"

#ifndef APP_H
#define APP_H

typedef struct {
  u8 year;
  u8 month;
  u8 day;
  u8 weekday;

  u8 hour;
  u8 minute;
  u8 second;
} Time;

enum UIStatus { NORMAL, EDITING };
enum FOCUS_POS { YEAR, MONTH, DAY, HOUR, MINUTE, SECOND };

class Device {
  public:
  Time time;
  DHT11 dht11;

  bool editing;
  int focusItem;

  Device(Time& time, DHT11& dht11) : time(time), dht11(dht11){};
  Device(gpio dht11Pin) : dht11(DHT11(dht11Pin)) {
    Time time = {
        0, 0, 0, 0, 0, 0, 0,
    };

    this->time = time;
  };
};

#endif