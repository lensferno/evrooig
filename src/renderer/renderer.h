//
// Created by lensferno on 2024/3/4.
//

#ifndef EVROOIG_U8G2RENDER_H
#define EVROOIG_U8G2RENDER_H

#include "../app/app.h"
#include "U8g2lib.h"

class Renderer {
  private:
  U8G2* u8g2;
  Device* device;

  void drawSensor(u8 xOffset, u8 yOffset);
  void drawDateTime(u8 xOffset, u8 yOffset);

  public:
  bool dark;

  Renderer(U8G2* u8g2, Device* device);
  void begin();
  void renderFrame();
};

#endif  // EVROOIG_U8G2RENDER_H
