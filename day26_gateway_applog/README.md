# Day26 gateway_applog：系统运行日志模块与异常数据处理

## 一、今日目标

今天在 Day25 `gateway_datasource` 的基础上，新增 `app_log` 系统运行日志模块。

Day25 已经实现：

```text
data_source 读取数据
↓
sensor_parser 解析
↓
build_json 生成 JSON
↓
sensor_log 写入传感器日志
↓
mqtt_client 发布 MQTT
```

Day26 新增：

```text
app_log 记录程序运行状态
app_log 记录异常数据
app_log 记录 INFO / WARN / ERROR 日志
```

最终目标：

```text
网关既能保存传感器数据 sensor_log.txt
也能保存系统运行日志 app_log.txt
```

## 二、项目结构

```text
day26_gateway_applog/
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
├── app_log.c
├── app_log.h
├── Makefile
├── README.md
├── sensor_log.txt
├── app_log.txt
└── notes/
    └── day26_note.md
```

## 三、新增模块说明

### 1. app_log.h

声明系统日志模块接口：

```c
void app_log_info(const char *msg);
void app_log_warn(const char *msg);
void app_log_error(const char *msg);
```

### 2. app_log.c

实现系统日志写入功能：

```text
INFO：普通运行信息
WARN：警告信息，例如解析失败
ERROR：错误信息，例如 MQTT 初始化失败
```

日志文件：

```text
app_log.txt
```

日志格式：

```text
[时间] [日志等级] 日志内容
```

示例：

```text
[2026-05-29 20:10:30] [INFO] gateway applog start
[2026-05-29 20:10:34] [WARN] parse sensor data failed
[2026-05-29 20:10:40] [INFO] gateway applog end
```

## 四、数据源说明

本实验仍然使用 mock 数据模式。

在 `data_source.c` 中加入一条异常数据：

```text
bad_data_from_stm32
```

用于测试：

```text
当 STM32 发来错误格式数据时，网关不会崩溃，而是记录 WARN 日志并继续运行。
```

模拟数据：

```text
temp=25,humi=60,device_id=stm32_01
temp=26,humi=61,device_id=stm32_01
bad_data_from_stm32
temp=28,humi=63,device_id=stm32_01
temp=29,humi=64,device_id=stm32_01
```

## 五、编译命令

```bash
make clean
make
```

编译成功后生成：

```text
gateway_applog
main.o
data_source.o
sensor_parser.o
log_writer.o
mqtt_client.o
app_log.o
```

## 六、运行方式

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
./gateway_applog
```

查看传感器日志：

```bash
cat sensor_log.txt
```

查看系统运行日志：

```bash
cat app_log.txt
```

## 七、实验现象

程序端输出示例：

```text
gateway applog start
data source: mock mode
mqtt connect success
publish topic: gateway/stm32_01/sensor

raw data: temp=25,humi=60,device_id=stm32_01
parse success
json: {"temp":25,"humi":60,"device_id":"stm32_01"}
write log success
mqtt publish success

raw data: bad_data_from_stm32
parse sensor data failed

gateway applog end
```

MQTT 订阅端收到正常数据：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
{"temp":26,"humi":61,"device_id":"stm32_01"}
{"temp":28,"humi":63,"device_id":"stm32_01"}
{"temp":29,"humi":64,"device_id":"stm32_01"}
```

说明：

```text
错误数据不会被发布到 MQTT
错误数据不会写入 sensor_log.txt
错误数据会记录到 app_log.txt
程序不会因为一条错误数据退出
```

## 八、sensor_log.txt 和 app_log.txt 的区别

### sensor_log.txt

保存传感器业务数据：

```text
[2026-05-29 20:10:30] {"temp":25,"humi":60,"device_id":"stm32_01"}
```

用途：

```text
记录传感器历史数据
便于后续数据分析
便于验证 MQTT payload
```

### app_log.txt

保存程序运行状态：

```text
[2026-05-29 20:10:30] [INFO] gateway applog start
[2026-05-29 20:10:34] [WARN] parse sensor data failed
```

用途：

```text
调试程序
排查错误
定位异常数据
记录系统运行过程
```

## 九、核心流程

```text
data_source_read()
↓
raw_buf
↓
parse_sensor_data()
↓
如果解析失败：app_log_warn()，continue
↓
build_json()
↓
write_log()
↓
mqtt_publish_json()
↓
app_log_info()
```

## 十、今日总结

Day26 完成了系统运行日志模块。

相比 Day25，Day26 的提升是：

```text
增加 app_log 模块
区分 sensor_log 和 app_log
增加 INFO / WARN / ERROR 日志等级
增加异常数据处理能力
程序遇到坏数据不会退出
```

这让网关项目更接近真实 Linux 后台服务程序。
