# Day18 MQTT C 语言 Subscribe 示例

## 一、今日目标

今天完成使用 C 语言程序订阅 MQTT 消息实验。

主要目标：

```text
1. 编写 mqtt_subscribe.c
2. 使用 Mosquitto C API 连接本地 Broker
3. 使用 mosquitto_subscribe() 订阅 topic
4. 使用回调函数接收 MQTT 消息
5. 使用 mosquitto_pub 或 Day17 mqtt_publish 测试消息接收
```

## 二、实验环境

```text
系统环境：Ubuntu / WSL
MQTT Broker：Mosquitto
开发库：libmosquitto-dev
订阅程序：mqtt_subscribe.c
测试 topic：gateway/stm32_01/sensor
默认端口：1883
```

## 三、编译命令

```bash
gcc mqtt_subscribe.c -o mqtt_subscribe -lmosquitto
```

注意：

```text
gcc 和 mqtt_subscribe.c 中间有空格
mqtt_subscribe.c 和 -o 中间有空格
-o 和 mqtt_subscribe 中间有空格
mqtt_subscribe 和 -lmosquitto 中间有空格
-lmosquitto 不能漏
```

## 四、运行方式

终端 1 运行 C 订阅程序：

```bash
./mqtt_subscribe
```

终端 2 发布测试消息：

```bash
mosquitto_pub -h localhost -t gateway/stm32_01/sensor -m '{"temp":25,"humi":60,"device_id":"stm32_01"}'
```

也可以使用 Day17 的 C 发布程序：

```bash
cd ~/linux-learning-submit/day17_mqtt_publish
./mqtt_publish
```

## 五、实验现象

订阅程序输出：

```text
mqtt subscribe client started
connect broker success
subscribe topic: gateway/stm32_01/sensor
recv mqtt message
topic: gateway/stm32_01/sensor
payload: {"temp":25,"humi":60,"device_id":"stm32_01"}
```

说明：

```text
C 程序成功连接 Mosquitto Broker
C 程序成功订阅 topic
发布端发送 JSON 后，订阅程序成功收到消息
MQTT subscribe 闭环成功
```

## 六、核心流程

C 程序订阅 MQTT 消息的基本流程：

```text
mosquitto_lib_init()
mosquitto_new()
mosquitto_connect_callback_set()
mosquitto_message_callback_set()
mosquitto_connect()
mosquitto_loop_forever()
mosquitto_destroy()
mosquitto_lib_cleanup()
```

连接成功后：

```text
on_connect() 被调用
在 on_connect() 中调用 mosquitto_subscribe()
```

收到消息后：

```text
on_message() 被调用
在 on_message() 中打印 topic 和 payload
```

## 七、今日总结

今天完成了 C 程序订阅 MQTT 消息实验。

Day17 是 C 程序发布消息：

```text
mqtt_publish -> Broker -> mosquitto_sub
```

Day18 是 C 程序订阅消息：

```text
mosquitto_pub / mqtt_publish -> Broker -> mqtt_subscribe
```

这一步为后续物联网网关接收远程控制命令打基础。

后续可以让 Linux 网关订阅：

```text
gateway/stm32_01/cmd
```

用于接收服务器下发的控制命令，例如：

```text
led_on
led_off
```
