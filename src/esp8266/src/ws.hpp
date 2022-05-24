/*
  开发人员：萌小狸宝宝
  编译时间：2022年5月17日
  编译版本：ESP8266 V3.02
  Executable segment sizes:
  ICACHE : 16384           - flash instruction cache
  IROM   : 263684          - code in flash         (default or ICACHE_FLASH_ATTR)
  IRAM   : 26905   / 49152 - code in IRAM          (IRAM_ATTR, ISRs...)
  DATA   : 1504  )         - initialized variables (global, static) in RAM/HEAP
  RODATA : 1664  ) / 81920 - constants             (global, static) in RAM/HEAP
  BSS    : 29864 )         - zeroed variables      (global, static) in RAM/HEAP
  使用 6.19.4  版本的库 ArduinoJson 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\ArduinoJson
  使用 2.15.0  版本的库 WebSockets_Generic 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\WebSockets_Generic
  使用 1.0  版本的库 ESP8266WiFi 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\ESP8266WiFi
  使用 1.0  版本的库 Hash 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\Hash
  使用 2.32.15  版本的库 U8g2 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\U8g2
  使用 1.0.1  版本的库 DFRobot_EnvironmentalSensor-master 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\DFRobot_EnvironmentalSensor-master
  使用 1.0  版本的库 Wire 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\Wire
  使用 1.0.1  版本的库 DFRobot_RTU-master 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\DFRobot_RTU-master
  使用 1.0  版本的库 SPI 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\SPI
  项目使用了 293757 字节，占用了 (28%) 程序存储空间。最大为 1044464 字节。
  全局变量使用了33032字节，(40%)的动态内存，余留48888字节局部变量。最大为81920字节。
*/

#pragma once

#define WS_HPP

#include <WebSocketsServer_Generic.h>
#include <ArduinoJson.h>
#include <Hash.h>
#include "api.hpp"

WebSocketsServer webSocket = WebSocketsServer(80);
String data_ip;

ICACHE_FLASH_ATTR bool ws_ip_search(String ip);
ICACHE_FLASH_ATTR bool ws_ip_add(String ip);
ICACHE_FLASH_ATTR bool ws_ip_delete();
ICACHE_FLASH_ATTR void WebSocketEvent(uint8_t num, WStype_t type, unsigned char* payload, size_t length);
ICACHE_FLASH_ATTR void WsInit();
ICACHE_FLASH_ATTR void WsMain();


ICACHE_FLASH_ATTR bool ws_ip_search(String ip)
{
  switch (ip[0])
  {
    case 'f':
      return true;
      break;
    case '1':
      if (ip.indexOf(F("10")) != false) return true;
      for (byte f1 = 16; f1 < 32; f1++)
      {
        String f1d = String(String(F("172.")) + String(f1d));
        if (ip.indexOf(f1d) != false)return true;
      }
      if (ip.indexOf(F("192.168")) != false) return true;
      break;
    default:
      if (ip == data_ip) return true;
      break;
  }
  return false;
}

ICACHE_FLASH_ATTR bool ws_ip_add(String ip)
{
  data_ip = ip;
  return true;
}

ICACHE_FLASH_ATTR bool ws_ip_delete()
{
  data_ip.clear();
  return true;
}

ICACHE_FLASH_ATTR void WebSocketEvent(uint8_t num, WStype_t type, unsigned char* payload, size_t length)
{
  String ip = webSocket.remoteIP(num).toString();
  switch (type)
  {
    case WStype_DISCONNECTED:
      {
        DynamicJsonDocument doc(1024);
        doc[F("在线")] = false;
        doc[F("用户")] = num;
        doc[F("消息")] = F("客户端断开");
        ip.clear();
        serializeJson(doc, ip);
        webSocket.broadcastTXT(ip);
        ws_ip_delete();
      }
      break;
    case WStype_CONNECTED:
      {
        DynamicJsonDocument doc(1024);
        if (ws_ip_search(ip))
        {
          doc[F("认证")] = true;
          doc[F("在线")] = true;
          doc[F("地址")] = ip;
          doc[F("用户")] = num;
          doc[F("消息")] = F("帮助文档请键入“帮助文档”获取。聊天请键入文字。（最多可以输入：") + String(uint16_t(ESP.getFreeHeap() / 32)) + "个字的消息）";
          ip.clear();
          serializeJson(doc, ip);
          doc.clear();
          webSocket.broadcastTXT(ip);
        }
        else
        {
          doc[F("认证")] = false;
          doc[F("地址")] = ip;
          doc[F("用户")] = num;
          ip.clear();
          serializeJson(doc, ip);
          doc.clear();
          webSocket.broadcastTXT(ip);
          webSocket.disconnect(num);
        }
      }
      break;
    case WStype_TEXT:
      {
        String data = reinterpret_cast<const char*>(payload);
#ifdef API_HPP
        bool dataBool = false;
        String dataArray[] = {F("系统重启"), F("系统信息"), F("无线扫描"), F("无线信息"), F("无线配网"), F("环境数据"), F("帮助文档")};
        for (byte f1 = 0; f1 <= sizeof(dataArray) / sizeof(String); f1++)
          if (data == dataArray[f1]) dataBool = true;
        if (dataBool)
        {
          data = ApiMain(data);
          webSocket.sendTXT(num, data);
        }
        else if (data.indexOf(F("认证")) != -1)
        {
          data.replace(F("认证："), F(""));
          data.replace(F("认证:"), F(""));
          data.replace(F("认证"), "");
          data_ip = data;
          data = F("{\"认证\": \"") + data + F("\"}");
          webSocket.broadcastTXT(data);
        }
        else
        {
          data = F("{\"用户\": \"") + String(num) + F("\", \"地址\": \"") + ip + F("\", \"消息\": \"") + data + F("\"}");
          webSocket.broadcastTXT(data);
        }
#endif
      }
      break;
    default: break;
  }
}

ICACHE_FLASH_ATTR void WsInit()
{
  webSocket.begin();
  webSocket.onEvent(WebSocketEvent);
}

ICACHE_FLASH_ATTR void WsMain()
{
  webSocket.loop();
}
