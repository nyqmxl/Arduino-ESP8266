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
  项目使用了 278073 字节，占用了 (26%) 程序存储空间。最大为 1044464 字节。
  全局变量使用了29392字节，(35%)的动态内存，余留52528字节局部变量。最大为81920字节。
*/

#pragma once

#define BUTTON_HPP
#define BUTTON_K1 0

ICACHE_FLASH_ATTR auto ButtonInit();
ICACHE_FLASH_ATTR auto ButtonMain();

ICACHE_FLASH_ATTR auto ButtonInit()
{
  pinMode(BUTTON_K1, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

ICACHE_FLASH_ATTR auto ButtonMain()
{
  uint32_t burronTime = millis();
  uint8_t burronCount = 0;
  if (!digitalRead(BUTTON_K1))
  {
    while (millis() - burronTime <= 3000)
    {
      delay(10);
      if (!digitalRead(BUTTON_K1))
      {
        while (!digitalRead(BUTTON_K1)) delay(10);
        if (millis() - burronTime <= 3000)
          burronCount++;
        else
          break;
      }
    }
    switch (burronCount)
    {
      case 0:
        digitalWrite(LED_BUILTIN, LOW);
#ifdef NETWORK_HPP
        NetConnect();
#else
        delay(3000);
#endif
        digitalWrite(LED_BUILTIN, HIGH);
        break;
        // case 1: break; // 按键执行功能代码
        // default: break;
    }
  }
}
