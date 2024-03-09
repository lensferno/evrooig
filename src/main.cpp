#include "Arduino.h"
#include "SPI.h"
#include "U8g2lib.h"
#include "app/app.h"
#include "bcd/bcd.h"
#include "config.h"
#include "dht11/dht11.h"
#include "driverlib.h"
#include "keybord/keybord.h"
#include "legacymsp430.h"
#include "msp430f5529.h"
#include "renderer/renderer.h"
#include "rtc_a.h"
#include "types.h"

typedef uint8_t uint8;
typedef int8_t int8;

static const uint8_t DISPLAY_CS_PIN = P2_6;
static const uint8_t DISPLAY_DC_PIN = P2_4;
static const uint8_t DISPLAY_RESET_PIN = P2_5;

static const gpio DHT11_PIN = P1_6;

static const u32 DHT11_COLLECTION_INTERVAL = 2 * TIME_SECOND;
u32 DHT11CollectStart = millis();

static const u32 RENDER_FPS = 24;
static const i32 FRAME_TIME = TIME_SECOND / RENDER_FPS;

u32 prevThemeSwitchTime = millis();
static const u32 THEME_SIWTCH_TNTERVAL = 16 * TIME_SECOND;

Device device(DHT11_DATA_PIN);

U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, DISPLAY_CS_PIN,
                                            DISPLAY_DC_PIN, DISPLAY_RESET_PIN);

Renderer renderer(&u8g2, &device);

void initRTC() {
  RTCCTL01 = RTCBCD + RTCHOLD + RTCMODE;

  RTCYEAR = 0x2024;  // 年
  RTCMON = 0x03;     // 月
  RTCDAY = 0x04;     // 日
  RTCDOW = 0x01;     // 星期

  RTCHOUR = 0x16;  // 时
  RTCMIN = 0x46;   // 分
  RTCSEC = 0x40;   // 秒

  RTCCTL01 &= ~RTCHOLD;
}

void setup() {
  Serial.begin(19200);

  initKeys();
  initRTC();
  renderer.begin();
}

u32 prevFrameStart = 0;

void readTime() {
  if ((RTCCTL01 & RTCRDY) && !device.editing) {
    device.time.year = bcd2dec(RTCYEAR);
    device.time.month = bcd2dec(RTCMON);
    device.time.day = bcd2dec(RTCDAY);
    device.time.weekday = bcd2dec(RTCDOW);

    device.time.hour = bcd2dec(RTCHOUR);
    device.time.minute = bcd2dec(RTCMIN);
    device.time.second = bcd2dec(RTCSEC);
  }
}

u8 getMaxDay(int year, u8 month) {
  switch (month) {
    case 2:
      if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        return 29;
      } else {
        return 28;
      }
    case 4:
    case 6:
    case 9:
    case 11:
      return 30;
    default:
      return 31;
  }
}

u8 inputValue = 0;

void editValue(int digit) {
  switch (device.focusItem) {
    case YEAR: {
      inputValue = inputValue * 10 + digit;
      if (inputValue > 99) {
        inputValue = digit;
      }
      device.time.year = inputValue;
      break;
    }
    case MONTH: {
      inputValue = inputValue * 10 + digit;
      if (inputValue < 1 || inputValue > 12) {
        inputValue = digit;
      }
      device.time.month = inputValue;
      break;
    }
    case DAY: {
      inputValue = inputValue * 10 + digit;
      if (inputValue > getMaxDay(device.time.year, device.time.month)) {
        inputValue = digit;
      }
      device.time.day = inputValue;
      break;
    }
    case HOUR: {
      inputValue = inputValue * 10 + digit;
      if (inputValue > 23) {
        inputValue = digit;
      }
      device.time.hour = inputValue;
      break;
    }
    case MINUTE: {
      inputValue = inputValue * 10 + digit;
      if (inputValue > 59) {
        inputValue = digit;
      }
      device.time.minute = inputValue;
      break;
    }
    case SECOND: {
      inputValue = inputValue * 10 + digit;
      if (inputValue > 59) {
        inputValue = digit;
      }
      device.time.second = inputValue;
      break;
    }
    default:
      break;
  }
}

void handleKeyEvent(int key) {
  switch (key) {
    case KEY_OK:
      while (!(RTCCTL01 & RTCRDY)) {
        __nop();
      }
      RTCYEAR = dec2bcd(device.time.year);
      RTCMON = dec2bcd(device.time.month);
      RTCDAY = dec2bcd(device.time.day);
      RTCDOW = dec2bcd(device.time.weekday);
      RTCHOUR = dec2bcd(device.time.hour);
      RTCMIN = dec2bcd(device.time.minute);
      RTCSEC = dec2bcd(device.time.second);
      inputValue = 0;
      device.editing = !device.editing;
      break;
    case KEY_CANCEL:
      inputValue = 0;
      device.editing = !device.editing;
      break;
    case NUM0:
      editValue(0);
      break;
    case NUM1:
      editValue(1);
      break;
    case NUM2:
      editValue(2);
      break;
    case NUM3:
      editValue(3);
      break;
    case NUM4:
      editValue(4);
      break;
    case NUM5:
      editValue(5);
      break;
    case NUM6:
      editValue(6);
      break;
    case NUM7:
      editValue(7);
      break;
    case NUM8:
      editValue(8);
      break;
    case NUM9:
      editValue(9);
      break;
    case KEY_ADD:
    case KEY_MINUS:
    case KEY_L:
      inputValue = 0;
      if (device.focusItem == YEAR) {
        device.focusItem = SECOND;
      } else {
        device.focusItem--;
      }
      break;
    case KEY_R:
      inputValue = 0;
      if (device.focusItem == SECOND) {
        device.focusItem = YEAR;
      } else {
        device.focusItem++;
      }
      break;
    case NO_KEY:
    default:
      break;
  }
}

void loop() {
  readTime();
  prevFrameStart = millis();

  // frame starts

  int key = readKeyOnReleased();
  if (key != NO_KEY) {
    Serial.print("Key ");
    Serial.println(key);
    handleKeyEvent(key);
  }

  if ((millis() - DHT11CollectStart) > DHT11_COLLECTION_INTERVAL) {
    device.dht11.read();
    DHT11CollectStart = millis();
  }

  if ((millis() - prevThemeSwitchTime) > THEME_SIWTCH_TNTERVAL) {
    renderer.dark = !renderer.dark;
    prevThemeSwitchTime = millis();
  }

  renderer.renderFrame();

  // frame ends

  u32 prevFrameTimeCost = millis() - prevFrameStart;
  if (FRAME_TIME > prevFrameTimeCost) {
    delay(FRAME_TIME - prevFrameTimeCost);
  }

  return;
}