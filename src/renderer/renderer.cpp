//
// Created by lensferno on 2024/3/4.
//

#include "renderer.h"

#include "Arduino.h"
#include "SPI.h"
#include "font/chinese_ui_text.h"

static const u8 UI_TEXT_SIZE = 10;
static const u8 LINE_HETGHT = UI_TEXT_SIZE + 2;

Renderer::Renderer(U8G2* u8g2, Device* device) : u8g2(u8g2), device(device) {}

void Renderer::begin() {
  u8g2->begin();
  u8g2->setFont(u8g2_font_courB10_tr);
  u8g2->setFontRefHeightExtendedText();
  u8g2->setFontPosTop();
  u8g2->setFontDirection(0);
  u8g2->enableUTF8Print();
}

void Renderer::drawSensor(u8 xOffset, u8 yOffset) {
  u8g2->setFontMode(1);
  u8g2->setDrawColor(2);
  u8g2->setFont(u8g2_font_courB10_tr);
  if (device->dht11.getStatus() != DHT_STATUS_OK) {
    u8g2->drawStr(0, 0, String(device->dht11.getStatus()).c_str());
  } else {
    u8 xOffset = 2, yOffset = 1;
    u8g2->setBitmapMode(1);

    // draw '温度'
    u8g2->drawXBMP(xOffset, yOffset, UI_TEXT_CHINESE_SIZE_12,
                   UI_TEXT_CHINESE_SIZE_12, UI_TEXT_12_BITMAP_TEMPERATURE[0]);
    xOffset += UI_TEXT_CHINESE_SIZE_12 + 1;
    u8g2->drawXBMP(xOffset, yOffset, UI_TEXT_CHINESE_SIZE_12,
                   UI_TEXT_CHINESE_SIZE_12, UI_TEXT_12_BITMAP_TEMPERATURE[1]);
    xOffset += UI_TEXT_CHINESE_SIZE_12;

    // draw temperature data
    char temperatureStr[2 + 1] = "00";
    sprintf(temperatureStr, "%2d", device->dht11.getTemperature());
    u8g2->drawStr(xOffset + 4, yOffset, temperatureStr);
    xOffset += u8g2->getStrWidth(temperatureStr) + 2 + 1;

    // draw '℃'
    u8g2->drawXBMP(xOffset, yOffset, UI_TEXT_CHINESE_SIZE_12,
                   UI_TEXT_CHINESE_SIZE_12, UI_TEXT_12_BMAP_CELIUS[0]);
    xOffset += UI_TEXT_SIZE + 6;

    // draw '湿度'
    u8g2->drawXBMP(xOffset, yOffset, UI_TEXT_CHINESE_SIZE_12,
                   UI_TEXT_CHINESE_SIZE_12, UI_TEXT_12_BMAP_HUMIDIYI[0]);
    xOffset += UI_TEXT_CHINESE_SIZE_12 + 1;
    u8g2->drawXBMP(xOffset, yOffset, UI_TEXT_CHINESE_SIZE_12,
                   UI_TEXT_CHINESE_SIZE_12, UI_TEXT_12_BMAP_HUMIDIYI[1]);
    xOffset += UI_TEXT_CHINESE_SIZE_12;

    // draw humidity data
    char humidityStr[2 + 1 + 1] = "00%";
    sprintf(humidityStr, "%2d%%", device->dht11.getHumidity());
    u8g2->drawStr(xOffset + 4, yOffset, humidityStr);
  }
}

void Renderer::drawDateTime(u8 xOffset, u8 yOffset) {
  u8g2->setFontMode(1);
  u8g2->setDrawColor(2);
  u8g2->setFont(u8g2_font_courB10_tr);

  // 年
  char numTmpStr[4 + 1] = "";
  sprintf(numTmpStr, "20%d", device->time.year);
  if (device->editing && device->focusItem == YEAR) {
    u8g2->drawBox(xOffset, yOffset, u8g2->getStrWidth(numTmpStr) + 1,
                  (u8g2->getAscent() - u8g2->getDescent()));
  }
  u8g2->drawStr(xOffset, yOffset, numTmpStr);
  xOffset += u8g2->getStrWidth(numTmpStr) + 1;
  // “年”
  u8g2->drawXBMP(xOffset, yOffset, UI_TEXT_CHINESE_SIZE_12,
                 UI_TEXT_CHINESE_SIZE_12, UI_TEXT_12_BMAP_YEAR[0]);
  xOffset += UI_TEXT_SIZE + 2;

  // 月
  sprintf(numTmpStr, "%d", device->time.month);
  if (device->editing && device->focusItem == MONTH) {
    u8g2->drawBox(xOffset, yOffset, u8g2->getStrWidth(numTmpStr) + 1,
                  (u8g2->getAscent() - u8g2->getDescent()));
  }
  u8g2->drawStr(xOffset, yOffset, numTmpStr);
  xOffset += u8g2->getStrWidth(numTmpStr) + 1;
  // “月”
  u8g2->drawXBMP(xOffset, yOffset, UI_TEXT_CHINESE_SIZE_12,
                 UI_TEXT_CHINESE_SIZE_12, UI_TEXT_12_BMAP_MONTH[0]);
  xOffset += UI_TEXT_SIZE;

  // 日
  sprintf(numTmpStr, "%d", device->time.day);
  if (device->editing && device->focusItem == DAY) {
    u8g2->drawBox(xOffset, yOffset, u8g2->getStrWidth(numTmpStr) + 1,
                  (u8g2->getAscent() - u8g2->getDescent()));
  }
  u8g2->drawStr(xOffset, yOffset, numTmpStr);
  xOffset += u8g2->getStrWidth(numTmpStr) + 1;
  // “日”
  u8g2->drawXBMP(xOffset, yOffset, UI_TEXT_CHINESE_SIZE_12,
                 UI_TEXT_CHINESE_SIZE_12, UI_TEXT_12_BMAP_DAY[0]);

  // 下一行
  yOffset += LINE_HETGHT;
  xOffset = 2;

  // 时
  char timeStr[8 + 1] = "";
  sprintf(timeStr, "%02d", device->time.hour);
  if (device->editing && device->focusItem == HOUR) {
    u8g2->drawBox(xOffset, yOffset, u8g2->getStrWidth(timeStr) + 1,
                  (u8g2->getAscent() - u8g2->getDescent()));
  }
  u8g2->drawStr(xOffset, yOffset, timeStr);
  xOffset += u8g2->getStrWidth(timeStr);
  u8g2->drawStr(xOffset, yOffset, ":");
  xOffset += u8g2->getStrWidth(":");

  // 分
  sprintf(timeStr, "%02d", device->time.minute);
  if (device->editing && device->focusItem == MINUTE) {
    u8g2->drawBox(xOffset, yOffset, u8g2->getStrWidth(timeStr) + 1,
                  (u8g2->getAscent() - u8g2->getDescent()));
  }
  u8g2->drawStr(xOffset, yOffset, timeStr);
  xOffset += u8g2->getStrWidth(timeStr);
  u8g2->drawStr(xOffset, yOffset, ":");
  xOffset += u8g2->getStrWidth(":");

  // 秒
  sprintf(timeStr, "%02d", device->time.second);
  if (device->editing && device->focusItem == SECOND) {
    u8g2->drawBox(xOffset, yOffset, u8g2->getStrWidth(timeStr) + 1,
                  (u8g2->getAscent() - u8g2->getDescent()));
  }
  u8g2->drawStr(xOffset, yOffset, timeStr);

  xOffset = 2;
  yOffset += LINE_HETGHT;
  yOffset += 2;

  // 时间的三条图形
  u8g2->drawFrame(xOffset, yOffset, 128 - 4, LINE_HETGHT / 2);
  yOffset += 2;
  u8g2->drawBox(xOffset + 2, yOffset, (128 - 2) / 24 * device->time.hour,
                (LINE_HETGHT / 2) - 2 * 2);
  yOffset += LINE_HETGHT / 2;

  u8g2->drawFrame(2, yOffset, 128 - 4, LINE_HETGHT / 2);
  yOffset += 2;
  u8g2->drawBox(xOffset + 2, yOffset, (128 - 2) / 60 * device->time.minute,
                (LINE_HETGHT / 2) - 2 * 2);
  yOffset += LINE_HETGHT / 2;

  u8g2->drawFrame(2, yOffset, 128 - 4, LINE_HETGHT / 2);
  yOffset += 2;
  u8g2->drawBox(xOffset + 2, yOffset, (128 - 2) / 60 * device->time.second,
                (LINE_HETGHT / 2) - 2 * 2);
}

void Renderer::renderFrame() {
  u8g2->clearBuffer();

  // 画背景色
  if (dark) {
    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, 128, 64);
  }

  drawSensor(2, 1);
  drawDateTime(2, UI_TEXT_SIZE + 4);

  u8g2->sendBuffer();
}
