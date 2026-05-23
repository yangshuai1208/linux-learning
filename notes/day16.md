# Day16 笔记：Mosquitto Broker 本地 MQTT 收发测试

## 1. 今日完成内容

今天完成了 Mosquitto MQTT Broker 的安装和本地收发测试。

完成内容：

```text
安装 mosquitto
安装 mosquitto-clients
启动 Mosquitto Broker
使用 mosquitto_sub 订阅 topic
使用 mosquitto_pub 发布 JSON 数据
验证订阅端成功收到消息
```

最终实验现象：

```text
订阅端收到：
{"temp":25,"humi":60,"device_id":"stm32_01"}
```

说明 MQTT 本地通信闭环成功。

## 2. Mosquitto 是什么

Mosquitto 是一个开源 MQTT Broker。

Broker 的作用：

```text
接收发布者发送的消息
根据 topic 转发给订阅者
管理 MQTT 客户端连接
```

可以理解为：

```text
Mosquitto 是本地 MQTT 消息服务器。
```

## 3. mosquitto-clients

安装命令：

```bash
sudo apt install mosquitto-clients -y
```

`mosquitto-clients` 提供两个常用命令：

```text
mosquitto_pub：发布消息
mosquitto_sub：订阅消息
```

今天就是用这两个命令完成 MQTT 收发测试。

## 4. 启动 Mosquitto

启动命令：

```bash
sudo service mosquitto start
```

查看状态：

```bash
sudo service mosquitto status
```

Mosquitto 默认监听端口：

```text
1883
```

## 5. mosquitto_sub

订阅命令：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

参数解释：

```text
mosquitto_sub：MQTT 订阅工具
-h localhost：连接本机 Broker
-t gateway/stm32_01/sensor：订阅指定 topic
```

运行后终端会等待消息。

这是正常现象，不是卡死。

## 6. mosquitto_pub

发布命令：

```bash
mosquitto_pub -h localhost -t gateway/stm32_01/sensor -m '{"temp":25,"humi":60,"device_id":"stm32_01"}'
```

参数解释：

```text
mosquitto_pub：MQTT 发布工具
-h localhost：连接本机 Broker
-t gateway/stm32_01/sensor：发布到指定 topic
-m：消息内容
```

发布内容是 JSON 字符串：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
```

## 7. Topic

今天使用的 topic：

```text
gateway/stm32_01/sensor
```

含义：

```text
gateway：网关项目
stm32_01：设备编号
sensor：传感器数据
```

Topic 必须完全一致。

例如订阅：

```text
gateway/stm32_01/sensor
```

发布也必须是：

```text
gateway/stm32_01/sensor
```

否则订阅端收不到消息。

## 8. Payload

Payload 是 MQTT 消息内容。

今天的 payload：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
```

含义：

```text
temp：温度
humi：湿度
device_id：设备编号
```

后续网关项目中，这个 JSON 可以由 STM32 串口数据解析生成。

## 9. MQTT 本地通信流程

今天的通信流程：

```text
终端 1：
mosquitto_sub 订阅 gateway/stm32_01/sensor

终端 2：
mosquitto_pub 发布 JSON 到 gateway/stm32_01/sensor

Mosquitto Broker：
接收发布消息并转发给订阅者

终端 1：
收到 JSON 数据
```

完整模型：

```text
Publisher -> Broker -> Subscriber
```

## 10. 和 TCP 阶段的关系

前面学习的 TCP 是点对点通信：

```text
TCP client 直接连接 TCP server
```

MQTT 是发布 / 订阅通信：

```text
发布者和订阅者都连接 Broker
发布者不需要知道谁接收
订阅者不需要知道谁发送
Broker 负责消息转发
```

一句话区别：

```text
TCP 是点对点通信
MQTT 是发布 / 订阅通信
```

## 11. 和物联网网关项目的关系

后续网关项目的数据流：

```text
STM32 采集温湿度
↓
串口发送给 Linux
↓
Linux termios 读取串口
↓
Linux 解析成 JSON
↓
Linux 使用 MQTT publish 上传
↓
Mosquitto Broker 转发
↓
服务器或云平台 subscribe 接收
```

当前模拟数据：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
```

后续真实数据来自 STM32。

## 12. 常见问题

### 问题 1：Connection refused

原因：

```text
Mosquitto 服务没有启动。
```

解决：

```bash
sudo service mosquitto start
```

### 问题 2：mosquitto_pub command not found

原因：

```text
没有安装 mosquitto-clients。
```

解决：

```bash
sudo apt install mosquitto-clients -y
```

### 问题 3：订阅端收不到消息

检查：

```text
1. 是否先运行 mosquitto_sub
2. pub 和 sub 的 topic 是否完全一致
3. Mosquitto 是否已经启动
4. JSON 引号是否写错
```

## 13. 今日命令总结

安装：

```bash
sudo apt update
sudo apt install mosquitto mosquitto-clients -y
```

启动服务：

```bash
sudo service mosquitto start
```

查看状态：

```bash
sudo service mosquitto status
```

订阅：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

发布：

```bash
mosquitto_pub -h localhost -t gateway/stm32_01/sensor -m '{"temp":25,"humi":60,"device_id":"stm32_01"}'
```

## 14. 面试考点

### 问：Mosquitto 是什么？

答：

```text
Mosquitto 是一个开源 MQTT Broker，用于接收发布者消息，并根据 topic 转发给订阅者。
```

### 问：MQTT 默认端口是多少？

答：

```text
1883。
```

### 问：mosquitto_pub 和 mosquitto_sub 分别是什么？

答：

```text
mosquitto_pub 用于发布 MQTT 消息。
mosquitto_sub 用于订阅 MQTT 消息。
```

### 问：如何用命令行测试 MQTT？

答：

```bash
mosquitto_sub -h localhost -t topic
mosquitto_pub -h localhost -t topic -m message
```

### 问：为什么订阅端收不到消息？

答：

```text
常见原因是 Broker 未启动、topic 不一致、订阅端未先启动或消息格式输入错误。
```

## 15. 今日总结

今天完成了 MQTT 本地收发闭环。

最重要的结论：

```text
Mosquitto 是 Broker
mosquitto_pub 是发布者
mosquitto_sub 是订阅者
topic 是消息主题
payload 是消息内容
```

最终数据流：

```text
mosquitto_pub -> Mosquitto Broker -> mosquitto_sub
```

这为后续 C 程序通过 MQTT 上传 STM32 数据打下基础。
