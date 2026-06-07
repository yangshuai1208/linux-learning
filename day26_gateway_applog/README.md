# Day24 gateway_module：网关模块化拆分与 Makefile 构建

## 一、今日目标

今天将 Day23 的单文件 `gateway_mqtt.c` 拆分成多个模块，使项目结构更接近真实 Linux C 工程。

Day23：

```text
gateway_mqtt.c 一个大文件
```

Day24：

```text
main.c
sensor_parser.c / sensor_parser.h
log_writer.c / log_writer.h
mqtt_client.c / mqtt_client.h
Makefile
```

目标流程：

```text
模拟 STM32 数据
↓
sensor_parser 解析
↓
生成 JSON
↓
log_writer 写日志
↓
mqtt_client 发布 MQTT
↓
mosquitto_sub 接收
```

## 二、项目结构

```text
day24_gateway_module/
├── main.c
├── sensor_parser.c
├── sensor_parser.h
├── log_writer.c
├── log_writer.h
├── mqtt_client.c
├── mqtt_client.h
├── Makefile
├── README.md
├── sensor_log.txt
└── notes/
    └── day24_note.md
```

## 三、模块说明

### 1. main.c

负责主流程控制：

```text
初始化 MQTT
循环处理模拟数据
调用解析模块
调用日志模块
调用 MQTT 发布模块
释放资源
```

### 2. sensor_parser.c / sensor_parser.h

负责传感器数据解析和 JSON 生成：

```text
parse_sensor_data()
build_json()
SensorData 结构体
```

### 3. log_writer.c / log_writer.h

负责日志写入：

```text
write_log()
sensor_log.txt
```

### 4. mqtt_client.c / mqtt_client.h

负责 MQTT 连接、发布和清理：

```text
mqtt_client_init()
mqtt_publish_json()
mqtt_client_cleanup()
```

### 5. Makefile

负责多文件编译：

```text
make
make clean
```

## 四、编译命令

```bash
make
```

编译成功后生成：

```text
gateway_module
main.o
sensor_parser.o
log_writer.o
mqtt_client.o
```

清理命令：

```bash
make clean
```

## 五、运行方式

先启动 Mosquitto：

```bash
sudo service mosquitto start
```

终端 1 订阅 MQTT：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

终端 2 运行网关程序：

```bash
./gateway_module
```

查看日志：

```bash
cat sensor_log.txt
```

## 六、实验现象

网关程序输出：

```text
gateway module start
mqtt connect success
publish topic: gateway/stm32_01/sensor

raw data: temp=25,humi=60,device_id=stm32_01
parse success
json: {"temp":25,"humi":60,"device_id":"stm32_01"}
write log success
mqtt publish success

gateway module end
```

订阅端收到：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
{"temp":26,"humi":61,"device_id":"stm32_01"}
{"temp":27,"humi":62,"device_id":"stm32_01"}
{"temp":28,"humi":63,"device_id":"stm32_01"}
{"temp":29,"humi":64,"device_id":"stm32_01"}
```

说明：

```text
模块化网关程序编译成功
MQTT 连接成功
传感器字符串解析成功
JSON 生成成功
日志写入成功
MQTT 发布成功
```

## 七、今日总结

Day24 完成了 Linux 网关项目的模块化拆分。

当前项目已经具备：

```text
多文件工程结构
头文件声明
源文件实现
Makefile 自动编译
日志保存
MQTT 上传
```

这一步让项目从“单文件 demo”升级为“工程化项目雏形”。
