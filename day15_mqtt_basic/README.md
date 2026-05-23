# Day15 MQTT 基本概念

## 一、今日目标

今天学习 MQTT 的基本概念，为后续物联网网关项目中的 MQTT 数据上传做准备。

今日重点：

```text
1. 理解 MQTT 是什么
2. 理解 Broker
3. 理解 Topic
4. 理解 Publish
5. 理解 Subscribe
6. 理解 MQTT 为什么适合物联网
7. 理解 MQTT 和 TCP 的区别
```

## 二、MQTT 是什么

MQTT 是一种轻量级的物联网通信协议。

它常用于：

```text
传感器数据上传
设备状态上报
远程控制
云平台通信
边缘网关数据转发
```

MQTT 的核心模型是：

```text
发布 / 订阅模型
```

也就是：

```text
发布者 publish 消息
订阅者 subscribe 主题
Broker 负责中转消息
```

## 三、MQTT 核心角色

MQTT 中主要有三个角色：

```text
Publisher：发布者
Subscriber：订阅者
Broker：消息服务器
```

关系如下：

```text
Publisher
    |
    | publish
    v
  Broker
    |
    | forward
    v
Subscriber
```

## 四、Broker

Broker 是 MQTT 的消息服务器。

作用：

```text
接收客户端发布的消息
管理客户端连接
根据 topic 把消息转发给订阅者
```

可以理解为：

```text
MQTT 的消息中转站
```

后续 Day16 要安装的 Mosquitto 就是一个 MQTT Broker。

## 五、Topic

Topic 是 MQTT 的消息主题。

它用于区分不同类型的消息。

例如：

```text
gateway/stm32_01/sensor
gateway/stm32_01/temp
gateway/stm32_01/humi
gateway/stm32_01/status
gateway/stm32_01/cmd
```

在网关项目中可以这样设计：

```text
传感器数据：gateway/stm32_01/sensor
设备状态：gateway/stm32_01/status
控制命令：gateway/stm32_01/cmd
```

## 六、Publish

Publish 表示发布消息。

例如 Linux 网关把 STM32 采集到的数据发布到 MQTT Broker：

```text
topic: gateway/stm32_01/sensor
payload: {"temp":25,"humi":60,"device_id":"stm32_01"}
```

含义：

```text
网关把数据发给 Broker
```

## 七、Subscribe

Subscribe 表示订阅消息。

例如服务器订阅：

```text
gateway/stm32_01/sensor
```

只要有设备向这个 topic 发布数据，服务器就能收到。

含义：

```text
谁订阅了某个 topic，谁就能收到该 topic 下的消息。
```

## 八、MQTT 和 TCP 的区别

TCP 是点对点通信：

```text
client 直接连接 server
client 和 server 直接收发数据
```

MQTT 是发布 / 订阅通信：

```text
所有客户端连接 Broker
发布者只管 publish
订阅者只管 subscribe
Broker 负责中转
```

对比：

```text
TCP：点对点
MQTT：发布 / 订阅
```

## 九、MQTT 为什么适合物联网

MQTT 适合物联网的原因：

```text
协议轻量
通信开销小
适合低带宽网络
适合设备数量多的场景
发布者和订阅者解耦
适合传感器数据上报
```

对于本项目来说，MQTT 很适合：

```text
Linux 网关把 STM32 的温湿度数据发布到服务器或云平台。
```

## 十、和物联网网关项目的关系

后续网关数据流：

```text
STM32 采集温湿度
↓
USART1 串口发送
↓
Linux termios 读取串口
↓
Linux 解析数据
↓
MQTT publish 上传到 Broker
↓
服务器 / 云平台 subscribe 接收数据
```

示例消息：

```text
topic: gateway/stm32_01/sensor
payload: {"temp":25,"humi":60,"device_id":"stm32_01"}
```

## 十一、今日总结

今天完成了 MQTT 基本概念学习。

重点掌握：

```text
Broker：消息服务器
Topic：消息主题
Publish：发布消息
Subscribe：订阅消息
```

MQTT 是后续物联网网关数据上传的重要基础。
