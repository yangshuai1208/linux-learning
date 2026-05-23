# Day17 MQTT C 语言 Publish 示例

## 一、今日目标

今天完成使用 C 语言程序发布 MQTT 消息实验。

主要目标：

```text
1. 安装 libmosquitto-dev 开发库
2. 编写 mqtt_publish.c
3. 使用 Mosquitto C API 连接本地 Broker
4. 使用 mosquitto_publish() 发布 JSON 数据
5. 使用 mosquitto_sub 验证订阅端是否能收到消息
```

## 二、实验环境

```text
系统环境：Ubuntu / WSL
MQTT Broker：Mosquitto
开发库：libmosquitto-dev
发布程序：mqtt_publish.c
测试 topic：gateway/stm32_01/sensor
默认端口：1883
```

## 三、安装依赖

```bash
sudo apt install libmosquitto-dev -y
```

说明：

```text
libmosquitto-dev 提供 mosquitto.h 头文件和 libmosquitto 链接库。
```

## 四、编译命令

```bash
gcc mqtt_publish.c -o mqtt_publish -lmosquitto
```

注意：

```text
gcc 和 mqtt_publish.c 中间有空格
mqtt_publish.c 和 -o 中间有空格
-o 和 mqtt_publish 中间有空格
mqtt_publish 和 -lmosquitto 中间有空格
-lmosquitto 不能漏
```

## 五、运行方式

终端 1 先订阅 topic：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

终端 2 运行 C 程序：

```bash
./mqtt_publish
```

## 六、实验现象

C 程序输出：

```text
mqtt publish success
topic: gateway/stm32_01/sensor
payload: {"temp":25,"humi":60,"device_id":"stm32_01"}
```

订阅端收到：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
```

说明：

```text
C 程序成功连接 Mosquitto Broker
C 程序成功发布 MQTT 消息
mosquitto_sub 成功收到消息
MQTT publish -> broker -> subscribe 闭环成功
```

## 七、核心流程

C 程序发布 MQTT 消息的基本流程：

```text
mosquitto_lib_init()
mosquitto_new()
mosquitto_connect()
mosquitto_publish()
mosquitto_loop()
mosquitto_disconnect()
mosquitto_destroy()
mosquitto_lib_cleanup()
```

## 八、今日总结

今天完成了 C 程序发布 MQTT 消息实验。

当前数据流：

```text
mqtt_publish C 程序
↓
Mosquitto Broker
↓
mosquitto_sub 订阅端
```

这一步是后续物联网网关项目 MQTT 上传模块的基础。

后续可以将写死的 JSON 数据替换为 STM32 串口发送的真实温湿度数据。
