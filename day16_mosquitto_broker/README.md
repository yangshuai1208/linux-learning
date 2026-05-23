# Day16 Mosquitto Broker 本地 MQTT 收发测试

## 一、今日目标

今天完成 Mosquitto MQTT Broker 的安装与本地收发测试。

主要目标：

```text
1. 安装 Mosquitto Broker
2. 安装 mosquitto-clients 命令行工具
3. 启动 Mosquitto 服务
4. 使用 mosquitto_sub 订阅 topic
5. 使用 mosquitto_pub 发布 JSON 数据
6. 验证 MQTT publish -> broker -> subscribe 本地闭环
```

## 二、实验环境

```text
系统环境：Ubuntu / WSL
MQTT Broker：Mosquitto
测试工具：mosquitto_pub、mosquitto_sub
默认端口：1883
测试 topic：gateway/stm32_01/sensor
```

## 三、安装命令

```bash
sudo apt update
sudo apt install mosquitto mosquitto-clients -y
```

说明：

```text
mosquitto：MQTT Broker 服务端
mosquitto-clients：MQTT 命令行测试工具
```

其中 `mosquitto-clients` 提供：

```text
mosquitto_pub：发布消息
mosquitto_sub：订阅消息
```

## 四、启动 Mosquitto 服务

```bash
sudo service mosquitto start
```

查看状态：

```bash
sudo service mosquitto status
```

如果服务正常，说明本地 MQTT Broker 已经启动。

## 五、订阅 topic

第一个终端执行：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

参数说明：

```text
mosquitto_sub：订阅消息
-h localhost：连接本机 Broker
-t gateway/stm32_01/sensor：订阅指定 topic
```

该终端会等待接收消息。

## 六、发布消息

第二个终端执行：

```bash
mosquitto_pub -h localhost -t gateway/stm32_01/sensor -m '{"temp":25,"humi":60,"device_id":"stm32_01"}'
```

参数说明：

```text
mosquitto_pub：发布消息
-h localhost：连接本机 Broker
-t gateway/stm32_01/sensor：发布到指定 topic
-m：指定消息内容
```

注意：

```text
JSON 外面使用英文单引号 '
JSON 里面使用英文双引号 "
不要使用中文引号
```

## 七、实验现象

订阅终端收到：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
```

说明：

```text
Mosquitto Broker 正常运行
mosquitto_sub 订阅成功
mosquitto_pub 发布成功
topic 匹配正确
MQTT 本地收发闭环成功
```

## 八、MQTT 本地闭环

本次实验的数据流：

```text
mosquitto_pub
↓
Mosquitto Broker
↓
mosquitto_sub
```

对应 MQTT 模型：

```text
Publisher
↓
Broker
↓
Subscriber
```

## 九、今日总结

今天完成了本地 MQTT Broker 的安装和测试。

掌握内容：

```text
1. Mosquitto 是 MQTT Broker
2. mosquitto_pub 用于发布消息
3. mosquitto_sub 用于订阅消息
4. MQTT 默认端口是 1883
5. topic 必须完全一致，订阅端才能收到消息
6. MQTT 可以用于后续 Linux 网关上传 STM32 传感器数据
```

后续可以在 C 程序中使用 MQTT 客户端库，把串口读取到的 STM32 数据发布到 Broker。
