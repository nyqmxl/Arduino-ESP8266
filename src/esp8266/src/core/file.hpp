/*
  开发人员：萌小狸宝宝
  编译时间：2022年5月17日
  编译版本：ESP8266 V3.02
  Executable segment sizes:
  ICACHE : 16384           - flash instruction cache
  IROM   : 277172          - code in flash         (default or ICACHE_FLASH_ATTR)
  IRAM   : 26361   / 49152 - code in IRAM          (IRAM_ATTR, ISRs...)
  DATA   : 1504  )         - initialized variables (global, static) in RAM/HEAP
  RODATA : 1468  ) / 81920 - constants             (global, static) in RAM/HEAP
  BSS    : 26872 )         - zeroed variables      (global, static) in RAM/HEAP
  使用 0.1.0  版本的库 LittleFS 在文件夹： C:\Users\panda.zhong\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\3.0.2\libraries\LittleFS
  项目使用了 306505 字节，占用了 (29%) 程序存储空间。最大为 1044464 字节。
  全局变量使用了29844字节，(36%)的动态内存，余留52076字节局部变量。最大为81920字节。
*/

#pragma once

#define FILE_HPP false

#include <LittleFS.h>

ICACHE_FLASH_ATTR bool FileInit();
ICACHE_FLASH_ATTR bool FileFormat();
ICACHE_FLASH_ATTR String FileListDir(String dirname);
ICACHE_FLASH_ATTR String FileRead(String path);
ICACHE_FLASH_ATTR bool FileWrite(String path, String message);
ICACHE_FLASH_ATTR bool FileAppend(String path, String message);
ICACHE_FLASH_ATTR bool FileRename(String path1, String path2);
ICACHE_FLASH_ATTR bool FileDelete(String path);

ICACHE_FLASH_ATTR bool FileInit()
{
  return LittleFS.begin();
}

ICACHE_FLASH_ATTR bool FileFormat()
{
  LittleFS.end();
  LittleFS.format();
  return LittleFS.begin();
}

ICACHE_FLASH_ATTR String FileListDir(String dirname)
{
  Dir root = LittleFS.openDir(dirname);
  dirname = F("[");
  while (root.next())
  {
    File file = root.openFile("r");
    dirname += F("\"") + root.fileName() + F("\"");
    file.close();
  }
  dirname += F("]");
  dirname.replace(F("\"\""), F("\",\""));
  return dirname;
}

ICACHE_FLASH_ATTR String FileRead(String path)
{
  File file = LittleFS.open(path, "r");
  if (!file) path = F("\0");
  if (file.available()) path = file.readString();
  file.close();
  return path;
}

ICACHE_FLASH_ATTR bool FileWrite(String path, String message)
{
  File file = LittleFS.open(path, "w");
  if (!file) return NULL;
  if (file.print(message))
  {
    file.close();
    return true;
  }
  file.close();
  return false;
}

ICACHE_FLASH_ATTR bool FileAppend(String path, String message)
{
  File file = LittleFS.open(path, "a");
  if (!file) return NULL;
  if (file.print(message))
  {
    file.close();
    return true;
  }
  file.close();
  return false;
}

ICACHE_FLASH_ATTR bool FileRename(String path1, String path2)
{
  return LittleFS.rename(path1, path2);
}

ICACHE_FLASH_ATTR bool FileDelete(String path)
{
  return LittleFS.remove(path);
}

#if FILE_HPP
ICACHE_FLASH_ATTR void FileTest()
{
  Serial.println(F("Formatting LittleFS filesystem"));
  Serial.print(F("初始化：")), Serial.println(FileInit());
  Serial.print(F("查看文件：")), Serial.println(FileListDir(F("/")));
  Serial.print(F("删除文件：")), Serial.println(FileDelete(F("/hello.txt")));
  Serial.print(F("写入文件：")), Serial.println(FileWrite(F("/hello.txt", "Hello ")));
  Serial.print(F("追加文件：")), Serial.println(FileAppend(F("/hello.txt", "World!\n")));
  Serial.print(F("查看文件：")), Serial.println(FileListDir(F("/")));
  Serial.print(F("读取文件：")), Serial.println(FileRead(F("/hello.txt")));
  Serial.print(F("格式化：")), Serial.println(FileFormat());
  Serial.print(F("查看文件：")), Serial.println(FileListDir(F("/")));
}
#endif
