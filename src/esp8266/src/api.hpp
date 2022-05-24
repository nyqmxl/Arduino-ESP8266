
/*
  开发人员：萌小狸宝宝
  编译时间：2022年5月17日
  编译版本：ESP8266 V3.02
  Executable segment sizes:
  ICACHE : 16384           - flash instruction cache
  IROM   : 257968          - code in flash         (default or ICACHE_FLASH_ATTR)
  IRAM   : 26905   / 49152 - code in IRAM          (IRAM_ATTR, ISRs...)
  DATA   : 1504  )         - initialized variables (global, static) in RAM/HEAP
  RODATA : 1192  ) / 81920 - constants             (global, static) in RAM/HEAP
  BSS    : 28736 )         - zeroed variables      (global, static) in RAM/HEAP
  使用 1.0  版本的库 ESP8266WiFi 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\ESP8266WiFi
  使用 6.19.4  版本的库 ArduinoJson 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\ArduinoJson
  使用 2.32.15  版本的库 U8g2 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\U8g2
  使用 1.0.1  版本的库 DFRobot_EnvironmentalSensor-master 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\DFRobot_EnvironmentalSensor-master
  使用 1.0  版本的库 Wire 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\Wire
  使用 1.0.1  版本的库 DFRobot_RTU-master 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\DFRobot_RTU-master
  使用 1.0  版本的库 SPI 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\SPI
  项目使用了 287569 字节，占用了 (27%) 程序存储空间。最大为 1044464 字节。
  全局变量使用了31432字节，(38%)的动态内存，余留50488字节局部变量。最大为81920字节。
*/

// 定义文件系统开启ESP8266文件系统，优点：解决内存不足，缺点：加速Flase损坏。
// #define FILE_SYSTEM

#pragma once

#define API_HPP

#ifdef FILE_SYSTEM
#include "core/file.hpp"
#endif
#include "core/sysmsg.hpp"
#include "core/network.hpp"
#include "core/device.hpp"

ICACHE_FLASH_ATTR auto ApiInit();
ICACHE_FLASH_ATTR auto ApiHelp();
ICACHE_FLASH_ATTR auto ApiMain(String parameter_doc);

ICACHE_FLASH_ATTR auto ApiInit()
{
#ifdef FILE_HPP
  FileInit();
#endif
#ifdef NETWORK_HPP
  String names = String(F("梦云物联设备-") + String(WiFi.macAddress()));
  names.replace(F(":"), F(""));
  WiFi.mode(WIFI_STA);
  WiFi.hostname(names.c_str());
  WiFi.begin(WiFi.SSID().c_str(), WiFi.psk().c_str());
#endif
#ifdef DEVICE_HPP
  OledInit();
#endif
}

ICACHE_FLASH_ATTR auto ApiHelp()
{
  DynamicJsonDocument doc(1024);
  doc[F("文档")] = F("此文件可以帮助机器自我学习，固定格式。");
  doc[F("语法")][0] = F("系统重启");
  doc[F("语法")][1] = F("系统信息");
  doc[F("语法")][2] = F("无线配网");
  doc[F("语法")][3] = F("无线扫描");
  doc[F("语法")][4] = F("无线信息");
  doc[F("语法")][5] = F("环境数据");
  String message;
  serializeJson(doc, message);
  doc.clear();
#ifdef FILE_HPP
  return FileWrite(F("/config/apihelp.json"), message);
#else
  return message;
#endif
}

ICACHE_FLASH_ATTR auto ApiMain(String doc)
{
  String data[] = {F("系统重启"), F("系统信息"), F("无线扫描"), F("无线信息"), F("无线配网"), F("环境数据"), F("环境显示")};
#ifdef FILE_HPP
  int8_t dataIf;
  for (byte f1 = 0; f1 <= sizeof(data) / sizeof(String); f1++)
  {
    if (doc != data[f1] && f1 == sizeof(data) / sizeof(String)) dataIf = 7;
    if (doc == data[f1])
    {
      switch (f1)
      {
        case 0: SysReboot(); break;
        case 1: dataIf = SysMsg() ? f1 : 6; break;
        case 2: dataIf = NetScanning() ? f1 : 6; break;
        case 3: dataIf = NetMsg() ? f1 : 6; break;
        case 4: dataIf = NetConnect() ? f1 : 6; break;
        case 5: dataIf = SEN0500() ? f1 : 6; break;
        case 6: OledMain(0); dataIf = f1; break;
        default: ApiHelp(); dataIf = f1; break;
      }
      break;
    }
  }
  // dataIf = ApiApp1(data, doc);
  switch (dataIf)
  {
    case 1: doc = F("/config/sysmsgbase.json"); break;
    case 2: doc = F("/config/netscanning.json"); break;
    case 3: doc = F("/config/netmsg.json"); break;
    case 4: return (dataIf == 4) ? F("true") : F("false"); break;
    case 5: doc = F("/config/sen0500.json"); break;
    case 6: return F("true"); break;
    default: doc = F("/config/apihelp.json"); break;
  }
  doc = FileRead(doc);
#else
  for (byte f1 = 0; f1 <= sizeof(data) / sizeof(String); f1++)
  {
    if (doc != data[f1] && f1 == sizeof(data) / sizeof(String)) doc = ApiHelp();
    if (doc == data[f1])
    {
      switch (f1)
      {
        case 0: SysReboot(); break;
        case 1: doc = SysMsg(); break;
        case 2: doc = NetScanning(); break;
        case 3: doc = NetMsg(); break;
        case 4: doc = String(NetConnect()); break;
        case 5: doc = SEN0500(); break;
        case 6: OledMain(0); doc = F("true"); break;
        default: break;
      }
      break;
    }
  }
#endif
  return doc;
}
