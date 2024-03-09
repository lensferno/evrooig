#include "dht11.h"

// #include "TimerSerial.h"
#include "msp430f5529.h"

#if defined(ARDUINO) && (ARDUINO >= 100)

#include <Arduino.h>

#else
#include <WProgram.h>
#endif

enum DHT11DataByte {
  BYTE_HUMIDITY_INT = 0,
  BYTE_HUMIDITY_FLOAT = 1,
  BYTE_TEMPERATURE_INT = 2,
  BYTE_TEMPERATURE_FLOAT = 3,
  BYTE_CHECKSUM = 4,
};

inline DHT11Status DHT11::handShake() {
  u8 waitCnt = 0;

  // 发送请求，先拉低信号至少18ms以上，然后拉高20~40us，转回输入模式
  // 这里拉高32us就行了
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delay(19);
  digitalWrite(pin, HIGH);
  delayMicroseconds(26);
  pinMode(pin, INPUT);

  // 根据文档，dht11收到主机请求信号后，会先将信号拉低40~80us，再将信号拉高40~80us

  // 此处等待dht11拉低完成，考虑到外部因素，宽限拉低超时时间设为128us，超过128us都没有重新拉高
  // 可能是收不到dht11的信号，返回超时

  waitCnt = 0;
  while (digitalRead(pin) == LOW) {
    waitCnt++;
    delayMicroseconds(1);
    if (waitCnt > 128) {
      Serial.println("handshake wait low signal end timeout");
      Serial.println(pin);
      this->status = DHT_STATUS_ERROR_TIMEOUT;
      return DHT_STATUS_ERROR_TIMEOUT;
    }
  }

  // 等待dht11拉高完成，和第一次相似
  waitCnt = 0;
  while (digitalRead(pin) == HIGH) {
    waitCnt++;
    delayMicroseconds(1);
    if (waitCnt > 128) {
      Serial.println("handshake wait high signal end timeout");
      Serial.println(pin);
      this->status = DHT_STATUS_ERROR_TIMEOUT;
      return DHT_STATUS_ERROR_TIMEOUT;
    }
  }

  return DHT_STATUS_OK;
}

DHT11::DHT11(gpio pin)
    : pin(pin), humidity(0), temperature(0), status(DHT_STATUS_OK) {}

DHT11Status DHT11::read() {
  u8 data[5] = {0};
  u8 waitCnt = 0;

  // 与dht11开始通信，发送握手信号
  if (this->handShake() != DHT_STATUS_OK) {
    return status;
  }

  // 读数据
  for (unsigned char &byteIdx : data) {
    for (u8 bitIdx = 0; bitIdx < 8; bitIdx++) {
      byteIdx <<= 1;

      // 等待开始信号结束，根据文档，正常情况下最多持续50us左右，此处以60us来判断
      waitCnt = 0;
      while (digitalRead(pin) == LOW) {
        delayMicroseconds(1);
        if (waitCnt > 60) {
          Serial.println("read wait low signal end timeout");
          Serial.println(pin);
          this->status = DHT_STATUS_ERROR_TIMEOUT;
          return DHT_STATUS_ERROR_TIMEOUT;
        }
      }

      // 等待30us的数据信号
      delayMicroseconds(30);

      // 30us后仍是高电平，认为传输1（传输0时高电平仅持续26~28us）
      if (digitalRead(pin) == HIGH) {
        byteIdx |= 1;
      }

      // 等待1bit的高电平结束，超时时间30us+60us=90us，正常情况下只持续70us
      waitCnt = 0;
      while (digitalRead(pin) == HIGH) {
        delayMicroseconds(1);
        if (waitCnt > 60) {
          Serial.println("read wait high signal end timeout");
          Serial.println(pin);
          this->status = DHT_STATUS_ERROR_TIMEOUT;
          return DHT_STATUS_ERROR_TIMEOUT;
        }
      }
    }
  }

  // 校验数据
  u8 chksum = 0;
  for (u8 i = 0; i < BYTE_CHECKSUM; i++) {
    chksum += data[i];
  }

  if (chksum != data[BYTE_CHECKSUM]) {
    this->status = DHT_STATUS_ERROR_CHECKSUM;
    return DHT_STATUS_ERROR_CHECKSUM;
  }

  this->temperature = data[BYTE_TEMPERATURE_INT];
  this->humidity = data[BYTE_HUMIDITY_INT];

  this->status = DHT_STATUS_OK;
  return DHT_STATUS_OK;
}
