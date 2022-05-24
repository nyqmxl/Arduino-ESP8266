/*
  开发人员：萌小狸宝宝
  编译时间：2022年5月17日
  编译版本：ESP8266 V3.02
  Executable segment sizes:
  ICACHE : 16384           - flash instruction cache
  IROM   : 252068          - code in flash         (default or ICACHE_FLASH_ATTR)
  IRAM   : 26465   / 49152 - code in IRAM          (IRAM_ATTR, ISRs...)
  DATA   : 1504  )         - initialized variables (global, static) in RAM/HEAP
  RODATA : 1036  ) / 81920 - constants             (global, static) in RAM/HEAP
  BSS    : 26936 )         - zeroed variables      (global, static) in RAM/HEAP
  项目使用了 281073 字节，占用了 (26%) 程序存储空间。最大为 1044464 字节。
  全局变量使用了29476字节，(35%)的动态内存，余留52444字节局部变量。最大为81920字节。
*/

#pragma once

#define SERIAL_HPP

#include "api.hpp"

ICACHE_FLASH_ATTR auto serial();

ICACHE_FLASH_ATTR auto serial()
{
  if (Serial.available())
  {
    String dataJson = Serial.readString();
#ifdef API_HPP
    dataJson.replace(F("\r"), F(""));
    dataJson.replace(F("\n"), F(""));
    dataJson = ApiMain(dataJson);
#endif
    Serial.println(dataJson);
  }
}
