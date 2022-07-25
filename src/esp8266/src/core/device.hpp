/*
  开发人员：萌小狸宝宝
  编译时间：2022年5月17日
  编译版本：ESP8266 V3.02
  Executable segment sizes:
  ICACHE : 16384           - flash instruction cache
  IROM   : 256212          - code in flash         (default or ICACHE_FLASH_ATTR)
  IRAM   : 26905   / 49152 - code in IRAM          (IRAM_ATTR, ISRs...)
  DATA   : 1504  )         - initialized variables (global, static) in RAM/HEAP
  RODATA : 1164  ) / 81920 - constants             (global, static) in RAM/HEAP
  BSS    : 28616 )         - zeroed variables      (global, static) in RAM/HEAP
  使用 6.19.4  版本的库 ArduinoJson 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\ArduinoJson
  使用 2.32.15  版本的库 U8g2 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\U8g2
  使用 1.0.1  版本的库 DFRobot_EnvironmentalSensor-master 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\DFRobot_EnvironmentalSensor-master
  使用 1.0  版本的库 Wire 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\Wire
  使用 1.0.1  版本的库 DFRobot_RTU-master 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\DFRobot_RTU-master
  使用 1.0  版本的库 SPI 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\SPI
  项目使用了 285785 字节，占用了 (27%) 程序存储空间。最大为 1044464 字节。
  全局变量使用了31284字节，(38%)的动态内存，余留50636字节局部变量。最大为81920字节。
*/

#pragma once

#define DEVICE_HPP

#include <ArduinoJson.h>
#include <U8g2lib.h>
#include "DFRobot_EnvironmentalSensor.h"

U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 5, 4, U8X8_PIN_NONE);

ICACHE_FLASH_ATTR auto OledInit();
ICACHE_FLASH_ATTR auto OledMain(byte data);
ICACHE_FLASH_ATTR auto SEN0500();

ICACHE_FLASH_ATTR auto OledInit()
{
  u8g2.setBusClock(400000);
  u8g2.enableUTF8Print();
  return u8g2.begin();
}

ICACHE_FLASH_ATTR auto OledMain(byte data)
{
  DFRobot_EnvironmentalSensor environment(/*addr = */SEN050X_DEFAULT_DEVICE_ADDRESS, /*pWire = */&Wire);
  u8g2.clearBuffer();
  u8g2.setFontDirection(data);
  u8g2.setFont(u8g2_font_wqy12_t_gb2312b);
  uint8_t od_num[][2] = {{0, 12}, {0, 24}, {64, 24}, {0, 36}, {64, 36}, {0, 48}, {64, 48}, {0, 60}};
  String od_str[] = {
    String(F("环境传感器基础数据")),
    String(F("气压:")) + String(environment.getAtmospherePressure(HPA)),
    String(F("海拔:")) + String(environment.getElevation()),
    String(F("温度:")) + String(environment.getTemperature(TEMP_C)),
    String(F("华氏:")) + String(environment.getTemperature(TEMP_F)),
    String(F("湿度:")) + String(environment.getHumidity()),
    String(F("光照:")) + String(uint32_t(environment.getLuminousIntensity())),
    String(F("紫外线:")) + String(environment.getUltravioletIntensity()) + String(F(" mw/cm2"))
  };
  for (uint8_t f1 = 0; f1 < 8; f1++) u8g2.drawUTF8(od_num[f1][0], od_num[f1][1], od_str[f1].c_str());
  u8g2.sendBuffer();
}

ICACHE_FLASH_ATTR auto SEN0500()
{
  DFRobot_EnvironmentalSensor environment(/*addr = */SEN050X_DEFAULT_DEVICE_ADDRESS, /*pWire = */&Wire);
  DynamicJsonDocument doc(1024);
  doc[F("环境数据")][F("状态")] = !environment.begin();
  doc[F("环境数据")][F("气压强度")] = environment.getAtmospherePressure(HPA);
  doc[F("环境数据")][F("海拔高度")] = environment.getElevation();
  doc[F("环境数据")][F("摄氏温度")] = environment.getTemperature(TEMP_C);
  doc[F("环境数据")][F("华氏温度")] = environment.getTemperature(TEMP_F);
  doc[F("环境数据")][F("湿度数值")] = environment.getHumidity();
  doc[F("环境数据")][F("光照强度")] = environment.getLuminousIntensity();
  doc[F("环境数据")][F("紫外线强度")] = environment.getUltravioletIntensity();
  String message;
  serializeJson(doc, message);
  doc.clear();
#ifdef FILE_HPP
  return FileWrite(F("/config/sen0501.json"), message);
#else
  return message;
#endif
}
