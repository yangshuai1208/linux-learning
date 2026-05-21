# Day15 笔记：MQTT 基本概念

## 1. 今日完成内容

今天学习 MQTT 基本概念：

```text
MQTT 是什么
Broker 是什么
Topic 是什么
Publish 是什么
Subscribe 是什么
MQTT 和 TCP 的区别
MQTT 为什么适合物联网
MQTT 在网关项目中的作用
```

今天不写 C 代码，主要理解 MQTT 通信模型。

## 2. MQTT 是什么

MQTT 是一种轻量级物联网通信协议。

它采用：

```text
发布 / 订阅模型
```

也就是：

```text
发布者不直接把消息发给订阅者
发布者把消息发给 Broker
Broker 再把消息转发给订阅者
```

这种方式适合物联网设备数据上传。

## 3. MQTT 的三个核心角色

MQTT 主要有三个角色：

```text
Publisher：发布者
Subscriber：订阅者
Broker：消息服务器
```

它们之间的关系：

```text
Publisher 发布消息到 Broker
Subscriber 从 Broker 订阅消息
Broker 负责消息中转
```

## 4. Broker

Broker 是 MQTT 的消息服务器。

作用：

```text
接收发布者的消息
保存或转发消息
把消息发送给订阅了对应 topic 的客户端
管理客户端连接
```

可以理解为：

```text
Broker 是 MQTT 系统里的中转站。
```

后续使用的 Mosquitto 就是一个 Broker。

## 5. Topic

Topic 是消息主题。

它用于区分不同类型的消息。

例如：

```text
gateway/stm32_01/sensor
gateway/stm32_01/temp
gateway/stm32_01/humi
gateway/stm32_01/status
gateway/stm32_01/cmd
```

Topic 可以按照层级设计：

```text
项目名 / 设备名 / 数据类型
```

例如：

```text
gateway/stm32_01/sensor
```

含义：

```text
gateway 项目中，stm32_01 这个设备的传感器数据。
```

## 6. Publish

Publish 表示发布消息。

例如 Linux 网关发布一条温湿度数据：

```text
topic: gateway/stm32_01/sensor
payload: {"temp":25,"humi":60,"device_id":"stm32_01"}
```

含义：

```text
Linux 网关把 STM32 数据发给 MQTT Broker。
```

Publisher 不需要知道谁会接收这条消息。

它只需要知道：

```text
我要把消息发到哪个 topic。
```

## 7. Subscribe

Subscribe 表示订阅消息。

例如服务器订阅：

```text
gateway/stm32_01/sensor
```

只要有客户端向这个 topic 发布消息，服务器就能收到。

Subscriber 不需要知道消息是谁发的。

它只需要知道：

```text
我要订阅哪个 topic。
```

## 8. MQTT 和 TCP 的区别

### TCP

TCP 是点对点通信。

```text
client 连接 server
client 和 server 直接通信
```

特点：

```text
通信双方直接绑定
需要明确知道对方 IP 和端口
适合直接连接通信
```

### MQTT

MQTT 是发布 / 订阅通信。

```text
所有客户端都连接 Broker
发布者 publish 消息
订阅者 subscribe topic
Broker 负责中转
```

特点：

```text
发布者和订阅者解耦
适合多个设备同时上传数据
适合物联网场景
```

## 9. 为什么 MQTT 适合物联网

MQTT 适合 IoT 的原因：

```text
协议轻量
数据包开销小
适合低带宽网络
适合设备资源有限场景
发布者和订阅者解耦
支持一对多消息分发
适合传感器周期性上报数据
```

例如本项目中：

```text
STM32 负责采集数据
Linux 网关负责上传数据
服务器或云平台负责接收数据
```

这正适合 MQTT。

## 10. 和 TCP 的项目定位区别

第 2 周学的 TCP 主要解决：

```text
Linux 如何进行网络连接和收发数据。
```

第 3 周 MQTT 主要解决：

```text
Linux 网关如何把设备数据发布到消息服务器。
```

TCP 更底层，MQTT 是建立在 TCP 之上的应用层协议。

可以理解为：

```text
TCP 负责可靠传输
MQTT 负责定义物联网消息发布订阅规则
```

## 11. 网关项目中的 MQTT 数据流

后续项目中的数据流：

```text
STM32F103C8T6
↓
USART1 串口
↓
Linux termios 读取
↓
解析成 JSON
↓
MQTT publish
↓
Mosquitto Broker
↓
服务器 / 云平台 subscribe
```

示例 JSON：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
```

示例 topic：

```text
gateway/stm32_01/sensor
```

## 12. 后续几天安排

接下来几天会继续学习：

```text
Day16：安装 Mosquitto，本地运行 MQTT broker
Day17：使用 Eclipse Paho C 写 publish demo
Day18：写 subscribe demo，完成本地 MQTT 收发闭环
Day19：Linux 串口接 STM32 数据
Day20：串口字符串解析成结构体或 JSON
Day21：gateway_v1：读串口 + 解析数据 + 打日志
```

## 13. 面试考点

### 问：MQTT 是什么？

答：

```text
MQTT 是一种轻量级的物联网通信协议，采用发布 / 订阅模型，常用于传感器数据上传和设备状态上报。
```

### 问：MQTT 的 Broker 是什么？

答：

```text
Broker 是 MQTT 消息服务器，负责接收发布者的消息，并把消息转发给订阅了对应 topic 的客户端。
```

### 问：Topic 是什么？

答：

```text
Topic 是 MQTT 中的消息主题，用于区分不同类型的数据。发布者向 topic 发布消息，订阅者订阅 topic 接收消息。
```

### 问：Publish 和 Subscribe 分别是什么意思？

答：

```text
Publish 是发布消息，Subscribe 是订阅消息。
发布者把消息发到某个 topic，订阅者订阅该 topic 后可以收到消息。
```

### 问：MQTT 为什么适合物联网？

答：

```text
MQTT 协议轻量，通信开销小，采用发布 / 订阅模型，适合设备资源有限、网络不稳定、数据量较小的物联网场景。
```

### 问：MQTT 和 TCP 有什么区别？

答：

```text
TCP 是点对点通信协议，MQTT 是基于 TCP 的应用层发布 / 订阅协议。MQTT 通过 Broker 解耦发布者和订阅者，更适合物联网设备数据分发。
```

## 14. 今日总结

今天掌握了 MQTT 的基础模型：

```text
Broker：消息服务器
Topic：消息主题
Publish：发布消息
Subscribe：订阅消息
```

最关键一句话：

```text
MQTT 通过 Broker 实现发布者和订阅者解耦，非常适合物联网设备数据上传。
```
