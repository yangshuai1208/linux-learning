# Day27 gateway_stats：网关运行统计模块

## 一、今日目标

今天在 Day26 `gateway_applog` 基础上，新增 `gateway_stats` 运行统计模块。

Day26 已经实现：

```text
data_source 数据源读取
sensor_parser 数据解析
log_writer 传感器日志
mqtt_client MQTT 发布
app_log 系统运行日志
```

Day27 新增：

```text
gateway_stats 运行统计模块
gateway_report.txt 运行报告
```

目标是让网关程序在运行结束后，可以统计本次运行结果：

```text
总数据条数
解析成功条数
解析失败条数
MQTT 发布成功条数
MQTT 发布失败条数
```

## 二、项目结构

```text
day27_gateway_stats/
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
├── gateway_stats.c
├── gateway_stats.h
├── Makefile
├── README.md
├── sensor_log.txt
├── app_log.txt
├── gateway_report.txt
└── notes/
    └── day27_note.md
```

## 三、新增模块说明

### 1. gateway_stats.h

声明运行统计结构体和函数：

```c
typedef struct
{
    int total_count;
    int parse_success_count;
    int parse_fail_count;
    int mqtt_success_count;
    int mqtt_fail_count;
} GatewayStats;
```

函数接口：

```c
void gateway_stats_init(GatewayStats *stats);
void gateway_stats_print(const GatewayStats *stats);
int gateway_stats_save_report(const GatewayStats *stats);
```

### 2. gateway_stats.c

实现运行统计功能：

```text
初始化统计数据
打印统计结果
保存统计报告
```

生成文件：

```text
gateway_report.txt
```

## 四、核心流程

```text
data_source_read()
↓
total_count++
↓
parse_sensor_data()
↓
解析成功：parse_success_count++
解析失败：parse_fail_count++
↓
build_json()
↓
write_log()
↓
mqtt_publish_json()
↓
MQTT 成功：mqtt_success_count++
MQTT 失败：mqtt_fail_count++
↓
gateway_stats_save_report()
```

## 五、编译命令

```bash
make clean
make
```

编译成功后生成：

```text
gateway_stats
main.o
data_source.o
sensor_parser.o
log_writer.o
mqtt_client.o
app_log.o
gateway_stats.o
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
./gateway_stats
```

查看日志和报告：

```bash
cat sensor_log.txt
cat app_log.txt
cat gateway_report.txt
```

## 七、实验现象

程序端输出示例：

```text
gateway stats start
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

========== gateway stats ==========
total count        : 5
parse success count: 4
parse fail count   : 1
mqtt success count : 4
mqtt fail count    : 0
===================================
save gateway report success
gateway stats end
```

MQTT 订阅端收到 4 条正常 JSON：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
{"temp":26,"humi":61,"device_id":"stm32_01"}
{"temp":28,"humi":63,"device_id":"stm32_01"}
{"temp":29,"humi":64,"device_id":"stm32_01"}
```

## 八、运行报告示例

`gateway_report.txt` 示例：

```text
Gateway Run Report
Time: 2026-xx-xx xx:xx:xx
-----------------------------------
total count        : 5
parse success count: 4
parse fail count   : 1
mqtt success count : 4
mqtt fail count    : 0
-----------------------------------
```

说明：

```text
本次共处理 5 条数据
其中 4 条解析成功
1 条解析失败
4 条成功发布 MQTT
0 条 MQTT 发布失败
```

## 九、三个输出文件区别

### sensor_log.txt

保存传感器业务数据：

```text
[时间] {"temp":25,"humi":60,"device_id":"stm32_01"}
```

### app_log.txt

保存程序运行状态：

```text
[时间] [INFO] gateway stats start
[时间] [WARN] parse sensor data failed
```

### gateway_report.txt

保存本次运行统计结果：

```text
total count
parse success count
parse fail count
mqtt success count
mqtt fail count
```

一句话区分：

```text
sensor_log 看数据
app_log 看过程
gateway_report 看结果
```

## 十、今日总结

Day27 完成了 `gateway_stats` 运行统计模块。

相比 Day26，Day27 的提升是：

```text
可以统计本次运行处理结果
可以生成 gateway_report.txt
可以量化解析成功率和 MQTT 发布成功率
可以作为项目测试记录和 GitHub 展示材料
```

当前网关已经具备：

```text
数据源模块
解析模块
传感器日志模块
MQTT 发布模块
系统运行日志模块
运行统计模块
运行报告输出
```

这让项目更接近真实 Linux 物联网网关服务。
