# Day25 gateway_datasource：新增 data_source 数据源模块

## 一、今日目标

今天在 Day24 模块化网关基础上，新增 `data_source` 数据源模块。

Day24 的主流程中，模拟数据直接写在 `main.c` 里：

```text
main.c 直接使用 raw_data_list
```

Day25 改为：

```text
main.c 不直接关心数据来自哪里
通过 data_source_read() 获取数据
```

目标流程：

```text
data_source 获取数据
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
day25_gateway_datasource/
├── main.c
├── config.h
├── data_source.c
├── data_source.h
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
    └── day25_note.md
```

## 三、新增模块说明

### 1. config.h

用于配置当前数据源模式。

```c
#define USE_MOCK_DATA 1
#define LOOP_COUNT 5
#define SERIAL_DEV "/dev/ttyUSB0"
```

说明：

```text
USE_MOCK_DATA 1：使用模拟数据
USE_MOCK_DATA 0：后续切换真实串口数据
LOOP_COUNT：循环读取次数
SERIAL_DEV：串口设备路径
```

### 2. data_source.c / data_source.h

负责统一提供数据。

核心函数：

```text
data_source_init()
data_source_read()
data_source_cleanup()
```

当前使用 mock 模式：

```text
模拟 STM32 周期上报数据
```

后续可切换为 serial 模式：

```text
从 /dev/ttyUSB0 读取真实 STM32 串口数据
```

## 四、编译命令

```bash
make clean
make
```

编译成功后生成：

```text
gateway_datasource
main.o
data_source.o
sensor_parser.o
log_writer.o
mqtt_client.o
```

## 五、运行方式

启动 Mosquitto：

```bash
sudo service mosquitto start
```

终端 1 订阅 MQTT：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

终端 2 运行程序：

```bash
./gateway_datasource
```

查看日志：

```bash
cat sensor_log.txt
```

## 六、实验现象

程序输出示例：

```text
gateway datasource start
data source: mock mode
mqtt connect success
publish topic: gateway/stm32_01/sensor

raw data: temp=25,humi=60,device_id=stm32_01
parse success
json: {"temp":25,"humi":60,"device_id":"stm32_01"}
write log success
mqtt publish success

gateway datasource end
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
data_source mock 数据读取成功
传感器字符串解析成功
JSON 生成成功
日志写入成功
MQTT 发布成功
mosquitto_sub 接收成功
```

## 七、核心流程

```text
data_source_read()
↓
raw_buf
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
```

## 八、今日总结

Day25 完成了数据源模块封装。

相比 Day24，Day25 的提升是：

```text
main.c 和数据来源解耦
支持 mock 数据模式
预留真实串口模式
项目结构更接近真实物联网网关
```

后续只需要把：

```c
#define USE_MOCK_DATA 1
```

改成：

```c
#define USE_MOCK_DATA 0
```

再解决 `/dev/ttyUSB0` 串口挂载，就可以切换到真实 STM32 数据源。
