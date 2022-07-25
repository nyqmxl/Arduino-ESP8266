/*
  开发人员：萌小狸宝宝
  编译时间：2022年5月17日
  编译版本：ESP8266 V3.02
  Executable segment sizes:
  ICACHE : 16384           - flash instruction cache
  IROM   : 251872          - code in flash         (default or ICACHE_FLASH_ATTR)
  IRAM   : 26277   / 49152 - code in IRAM          (IRAM_ATTR, ISRs...)
  DATA   : 1504  )         - initialized variables (global, static) in RAM/HEAP
  RODATA : 1064  ) / 81920 - constants             (global, static) in RAM/HEAP
  BSS    : 26968 )         - zeroed variables      (global, static) in RAM/HEAP
  使用 1.0  版本的库 ESP8266WiFi 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\ESP8266WiFi
  使用 6.19.4  版本的库 ArduinoJson 在文件夹： C:\Users\panda.zhong\Documents\Arduino\libraries\ArduinoJson
  项目使用了 280717 字节，占用了 (26%) 程序存储空间。最大为 1044464 字节。
  全局变量使用了29536字节，(36%)的动态内存，余留52384字节局部变量。最大为81920字节。
*/

#pragma once

#define NETWORK_HPP

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <AddrList.h>
#include <lwip/dns.h>

ICACHE_FLASH_ATTR auto NetInit(String name);
ICACHE_FLASH_ATTR auto NetConnect();
ICACHE_FLASH_ATTR auto NetMsg();
ICACHE_FLASH_ATTR auto NetScanning();

ICACHE_FLASH_ATTR auto NetInit(String names)
{
  WiFi.mode(WIFI_STA);
  WiFi.hostname(names);
  if (WiFi.status() != WL_CONNECTED) WiFi.begin(WiFi.SSID().c_str(), WiFi.psk().c_str());
  //  WiFi.begin("萌小狸宝宝", "13226253380");
}

ICACHE_FLASH_ATTR auto NetConnect()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi.disconnect();
    delay(100);
  }
  WiFi.beginSmartConfig();
  uint64_t connect_times = millis();
  while (millis() < 60 * 1000 || millis() - connect_times <= 60 * 1000)
  {
    if (WiFi.smartConfigDone())
    {
      WiFi.setAutoConnect(true);
      WiFi.setAutoReconnect(true);
      delay(5000);
      return true;
    }
    delay(100);//一定要加延时，不然直接重启。
  }
  WiFi.stopSmartConfig();
  WiFi.begin(WiFi.SSID().c_str(), WiFi.psk().c_str());
  return false;
}

ICACHE_FLASH_ATTR auto NetMsg()
{
  DynamicJsonDocument doc(1024);
  doc[F("无线信息")][F("状态")] = WiFi.status();
  doc[F("无线信息")][F("主机")] = WiFi.hostname();
  doc[F("无线信息")][F("名称")] = WiFi.SSID();
  doc[F("无线信息")][F("密码")] = WiFi.psk();
  doc[F("无线信息")][F("信号")] = WiFi.RSSI();
  doc[F("无线信息")][F("信道")] = WiFi.channel();
  doc[F("无线信息")][F("本地")] = WiFi.macAddress();
  doc[F("无线信息")][F("路由")] = WiFi.BSSIDstr();
  for (auto a : addrList)
  {
    if (a.isV6())
      doc[F("无线信息")][F("IPv6")].add(a.toString());
    else
    {
      doc[F("无线信息")][F("IPv4")][0] = a.toString();
      doc[F("无线信息")][F("IPv4")][1] = a.netmask();
      doc[F("无线信息")][F("IPv4")][2] = a.gw();
    }
  }
  for (byte f1 = 0; f1 < DNS_MAX_SERVERS; f1++)
  {
    IPAddress dns = WiFi.dnsIP(f1);
    if (dns.isSet()) doc[F("无线信息")][F("DNS")].add(dns);
  }
  String message;
  serializeJson(doc, message);
  doc.clear();
#ifdef FILE_HPP
  return FileWrite(F("/config/netmsg.json"), message);
#else
  return message;
#endif
}

ICACHE_FLASH_ATTR auto NetScanning()
{
  int8_t data_scan = WiFi.scanComplete();
  data_scan = data_scan < 30 ? data_scan : 30;
  DynamicJsonDocument doc(5120);
  switch (data_scan)
  {
    case -2:
      doc[F("无线扫描")][String(data_scan)] = F("启动扫描");
      WiFi.scanNetworks(/*async=*/true, /*hidden=*/true);
      break;
    case -1:
      doc[F("无线扫描")][String(data_scan)] = F("正在扫描");
      break;
    default:
      for (int8_t f1 = 0; f1 < data_scan; f1++)
      {
        doc[F("无线扫描")][String(data_scan)][f1][F("名称")] = WiFi.SSID(f1);
        doc[F("无线扫描")][String(data_scan)][f1][F("地址")] = WiFi.BSSIDstr(f1);
        doc[F("无线扫描")][String(data_scan)][f1][F("信号")] = WiFi.RSSI(f1);
        doc[F("无线扫描")][String(data_scan)][f1][F("信道")] = WiFi.channel(f1);
        switch (WiFi.encryptionType(f1))
        {
          case 2: doc[F("无线扫描")][String(data_scan)][f1][F("加密")] = "TKIP(WPA/PSK)"; break; //TKIP WPA PSK
          case 4: doc[F("无线扫描")][String(data_scan)][f1][F("加密")] = "CCMP(WPA2/PSK)"; break;//CCMP WPA2 PSK
          case 5: doc[F("无线扫描")][String(data_scan)][f1][F("加密")] = "WEP"; break;
          case 7: doc[F("无线扫描")][String(data_scan)][f1][F("加密")] = "NONE"; break;
          case 8: doc[F("无线扫描")][String(data_scan)][f1][F("加密")] = "AUTO"; break;
        }
        doc[F("无线扫描")][String(data_scan)][f1][F("隐藏")] = WiFi.isHidden(f1);
      }
      WiFi.scanDelete();
      break;
  }
  String message;
  serializeJson(doc, message);
  doc.clear();
#ifdef FILE_HPP
  return FileWrite(F("/config/netscanning.json"), message);
#else
  return message;
#endif
}
