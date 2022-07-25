/*
  开发人员：萌小狸宝宝
  编译时间：2022年5月17日
  编译版本：ESP8266 V3.02
  Executable segment sizes:
  ICACHE : 16384           - flash instruction cache
  IROM   : 249292          - code in flash         (default or ICACHE_FLASH_ATTR)
  IRAM   : 26245   / 49152 - code in IRAM          (IRAM_ATTR, ISRs...)
  DATA   : 1504  )         - initialized variables (global, static) in RAM/HEAP
  RODATA : 1032  ) / 81920 - constants             (global, static) in RAM/HEAP
  BSS    : 26856 )         - zeroed variables      (global, static) in RAM/HEAP
  使用 1.0  版本的库 ESP8266WiFi 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\ESP8266WiFi
  使用 6.19.4  版本的库 ArduinoJson 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\ArduinoJson
  项目使用了 278073 字节，占用了 (26%) 程序存储空间。最大为 1044464 字节。
  全局变量使用了29392字节，(35%)的动态内存，余留52528字节局部变量。最大为81920字节。
*/

#pragma once

#define SYSMSG_HPP

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

ICACHE_FLASH_ATTR auto SysReboot();
ICACHE_FLASH_ATTR auto SysMsg();

ICACHE_FLASH_ATTR auto SysReboot()
{
  ESP.restart();
  //ESP.reset();
}

ICACHE_FLASH_ATTR auto SysMsg()
{
  DynamicJsonDocument doc(1024);
  doc[F("系统信息")][F("开发人员")] = F("萌小狸宝宝");
  doc[F("系统信息")][F("编译版本")] = F("22.05.17");
  doc[F("系统信息")][F("编译时间")] = F("2022年5月17日");
  doc[F("系统信息")][F("使用场景")] = F("环境传感器（i2c）");
  doc[F("系统信息")][F("固件版本")] = ESP.getFullVersion();
  doc[F("系统信息")][F("CPU频率")] = ESP.getCpuFreqMHz();
  doc[F("系统信息")][F("CPU序列号")] = ESP.getChipId();
  doc[F("系统信息")][F("闪存序列号")] = ESP.getFlashChipId();
  doc[F("系统信息")][F("闪存大小")] = ESP.getFlashChipRealSize();
  doc[F("系统信息")][F("固件大小")] = ESP.getSketchSize();
  doc[F("系统信息")][F("固件MD5")] = ESP.getSketchMD5();
  doc[F("系统信息")][F("重置信息")] = ESP.getResetReason();
  // doc[F("系统信息")]["重置信息"] = ESP.getResetInfo();
  doc[F("系统信息")][F("可用堆")] = ESP.getFreeHeap();
  String message;
  serializeJson(doc, message);
  doc.clear();
#ifdef FILE_HPP
  return FileWrite(F("/config/sysmsgbase.json"), message);
#else
  return message;
#endif
}
