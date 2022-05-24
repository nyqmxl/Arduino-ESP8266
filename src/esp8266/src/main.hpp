/*
  开发人员：萌小狸宝宝
  编译时间：2022年5月17日
  编译版本：ESP8266 V3.02
  Executable segment sizes:
  ICACHE : 16384           - flash instruction cache
  IROM   : 459504          - code in flash         (default or ICACHE_FLASH_ATTR)
  IRAM   : 28041   / 49152 - code in IRAM          (IRAM_ATTR, ISRs...)
  DATA   : 1548  )         - initialized variables (global, static) in RAM/HEAP
  RODATA : 2592  ) / 81920 - constants             (global, static) in RAM/HEAP
  BSS    : 30648 )         - zeroed variables      (global, static) in RAM/HEAP
  使用 6.19.4  版本的库 ArduinoJson 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\ArduinoJson
  使用 2.15.0  版本的库 WebSockets_Generic 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\WebSockets_Generic
  使用 1.0  版本的库 ESP8266WiFi 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\ESP8266WiFi
  使用 1.0  版本的库 Hash 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\Hash
  使用 2.32.15  版本的库 U8g2 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\U8g2
  使用 1.0.1  版本的库 DFRobot_EnvironmentalSensor-master 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\DFRobot_EnvironmentalSensor-master
  使用 1.0  版本的库 Wire 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\Wire
  使用 1.0.1  版本的库 DFRobot_RTU-master 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\DFRobot_RTU-master
  使用 1.0  版本的库 SPI 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\SPI
  项目使用了 491685 字节，占用了 (47%) 程序存储空间。最大为 1044464 字节。
  全局变量使用了34788字节，(42%)的动态内存，余留47132字节局部变量。最大为81920字节。
*/

// 定义文件系统开启ESP8266文件系统，优点：解决内存不足，缺点：加速Flase损坏。
// 源文件定义在api.hpp中：默认未定义
// #define FILE_SYSTEM

#pragma once

#include <ESP8266mDNS.h>
#include "serial.hpp"
#include "button.hpp"
#include "ws.hpp"

ICACHE_FLASH_ATTR void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  String names = String(F("梦云物联设备-") + String(WiFi.macAddress()));
  names.replace(F(":"), F(""));
  MDNS.begin(names);
#ifdef API_HPP
  ApiInit();
#endif
#ifdef BUTTON_HPP
  ButtonInit();
#endif
#ifdef WS_HPP
  WsInit();
#endif
}

ICACHE_FLASH_ATTR void loop()
{
  MDNS.update();
#ifdef BUTTON_HPP
  ButtonMain();
#endif
#ifdef SERIAL_HPP
  serial();
#endif
#ifdef WS_HPP
  WsMain();
#endif
#ifdef API_HPP
  ApiMain("环境显示");
#endif
}
