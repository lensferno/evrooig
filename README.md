# evrooig

只是一个简单的单片机课设

DHT11环境温度/湿度检测+RTC电子时钟，SSDSSD1306 128x64LED显示屏输出界面，可使用4x4矩阵键盘修改时间

## 环境

开发框架：platformio

```text
platform = timsp430
board = lpmsp430f5529
framework = arduino
build_flags = -std=c++0x
```

开发：vscode/clion，安装platformio插件

## 硬件

开发板：Ti MSP430F5529 LaunchPad

温湿度传感器：DHT11

LED显示屏：SSDSSD1306 128x64，SPI协议

4x4矩阵键盘

## IO引脚连接

DHT11: 
  - data: P1.6 (GPIO)

LED显示屏:
  - SPI_CS(CS): P2.6 (GPIO)
  - SPI_DC(DC): P2.4 (GPIO)
  - SPI_RESET(RST): P2.5 (GPIO)
  - SPI_MOSI(D1): P3.0
  - SPI_CLK(D0): P3.2
  
4x4矩阵键盘:
  - row: P1_5 P1_4 P1_3 P1_2 (GPIO)
  - col: P4_3 P4_0 P3_7 P8_2 (GPIO)

## 键盘按键定义

```cpp
/**
 *     C1 C2 C3 C4
 *　R1  *  *  *  *
 *　R2  *  *  *  *
 *　R3  *  *  *  *
 *　R4  *  *  *  *
 */

enum Keys {
  NUM7,   NUM8,   NUM9,   KEY_ADD,
  NUM4,   NUM5,   NUM6,   KEY_MINUS,
  NUM1,   NUM2,   NUM3,   KEY_OK,
  NUM0,   KEY_L,  KEY_R,  KEY_CANCEL,
  NO_KEY = -1,
};
```

例：R2高电平时，C3读得高电平，识别为按下`NUM6`按键