#  **ESP8266**核心模板



## 开源目标

> 增加物联网的灵活性，如强制绑定服务器等。



## V22.05.01版本更新内容：

1. 远程调用。
2. 支持地址双栈协议；
3. 统一函数调用方式；
4. WebSocket和串口两种通信方式；




## 默认使用的库文件

| 序号 | 库文件             | 版本   |
| ---- | ------------------ | ------ |
| 1    | ESP8266WiFi        | 3.0.2  |
| 2    | ArduinoJson        | 6.19.4 |
| 3    | WebSockets_Generic | 2.15.0 |



## 二次开发库文件

| 序号 | 库文件                             | 版本    |
| ---- | ---------------------------------- | ------- |
| 1    | U8g2                               | 2.32.15 |
| 2    | DFRobot_EnvironmentalSensor-master | 1.0.1   |
| 3    | DFRobot_RTU-master                 | 1.0.1   |

**二次开发说明**：仅需要修改`驱动.hpp`文件和`API.hpp`文件即可。



## API调用说明（输入功能调用）

| 序号 | 功能       | 描述                       |
| ---- | ---------- | -------------------------- |
| 1    | 系统重启   | 重启系统                   |
| 2    | 系统信息   | 获取基础信息               |
| 3    | 无线配网   | 配置联网                   |
| 4    | 无线信息   | 获取网络信息               |
| 5    | 无线扫描   | 扫描设备的周围无线网络环境 |
| 6    | 环境数据   | 获取传感器数据             |
| 7    | 环境显示   | 显示数据在OLED屏幕上       |
| 8    | 认证：+ IP | 设置广域网一个白名单（WS） |
| 9    | GPIO0      | 长按配网（串口）           |



---



> 如果您觉得这是一个好的开源项目请赞助我们，如果失去赞助该开源项目将无法维护。
> 
> 商业用途或合作请联系企鹅群



---



**由[梦云之家](http://mengyuns.com)开发和维护**


企鹅群：594467493


欢迎各位提出建议或测试BUG。
