# Day23 gateway_mqtt：循环网关 + MQTT 发布

## 一、今日目标

今天将 Day22 的循环网关程序升级为 MQTT 上传版本。

Day22 实现：

```text
模拟数据
↓
解析
↓
生成 JSON
↓
写日志
```

Day23 新增：

```text
MQTT publish
```

完整流程：

```text
模拟 STM32 数据
↓
parse_sensor_data()
↓
build_json()
↓
write_log()
↓
mosquitto_publish()
↓
mosquitto_sub 接收
```

## 二、项目结构

```text
day23_gateway_mqtt/
├── gateway_mqtt.c
├── README.md
├── sensor_log.txt
└── notes/
    └── day23_note.md
```

## 三、实验环境

```text
系统环境：Ubuntu / WSL
MQTT Broker：Mosquitto
开发库：libmosquitto-dev
测试 topic：gateway/stm32_01/sensor
默认端口：1883
```

## 四、编译命令

```bash
gcc gateway_mqtt.c -o gateway_mqtt -lmosquitto
```

注意：

```text
-lmosquitto 不能漏
否则会出现 undefined reference 相关错误
```

## 五、运行方式

终端 1 订阅 MQTT topic：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

终端 2 运行网关程序：

```bash
./gateway_mqtt
```

查看日志：

```bash
cat sensor_log.txt
```

## 六、实验现象

网关程序输出：

```text
gateway mqtt start
mqtt connect success
publish topic: gateway/stm32_01/sensor

raw data: temp=25,humi=60,device_id=stm32_01
parse success
json: {"temp":25,"humi":60,"device_id":"stm32_01"}
write log success
mqtt publish success
```

订阅端收到：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
{"temp":26,"humi":61,"device_id":"stm32_01"}
{"temp":27,"humi":62,"device_id":"stm32_01"}
{"temp":28,"humi":63,"device_id":"stm32_01"}
```

说明：

```text
网关程序成功连接 MQTT Broker
传感器数据成功解析成 JSON
JSON 成功写入日志
JSON 成功通过 MQTT 发布
mosquitto_sub 成功接收消息
```

## 七、核心功能

本实验实现了：

```text
1. 模拟 STM32 周期上报数据
2. 解析传感器字符串
3. 生成 JSON 数据
4. 写入 sensor_log.txt
5. 使用 Mosquitto C API 发布 MQTT 消息
```

## 八、核心流程

```text
raw_data_list[i]
↓
parse_sensor_data()
↓
SensorData
↓
build_json()
↓
json_buf
↓
write_log()
↓
mqtt_publish_json()
↓
MQTT Broker
↓
mosquitto_sub
```

## 九、和真实网关的关系

当前版本使用模拟数据。

后续真实版本会替换为：

```text
STM32 USART1
↓
Linux read() 读取串口
↓
parse_sensor_data()
↓
build_json()
↓
write_log()
↓
mosquitto_publish()
```

最终目标：

```text
Linux 网关自动读取 STM32 数据，并通过 MQTT 上传到服务器或云平台。
```

## 十、今日总结

Day23 完成了网关程序和 MQTT publish 的整合。

当前网关已经具备：

```text
循环处理数据
JSON 封装
日志保存
MQTT 上传
```

这一步已经形成物联网网关项目的核心雏形。
