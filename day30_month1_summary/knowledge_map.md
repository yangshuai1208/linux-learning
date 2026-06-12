# 第一月知识图谱

## 1. Linux C 基础

掌握内容：

```text
gcc 编译
多文件编译
头文件
源文件
结构体
指针
字符串处理
错误返回值
函数封装
```

重点理解：

```text
.h 放声明
.c 放实现
main.c 做调度
模块之间通过头文件连接
```

## 2. Makefile

掌握内容：

```text
CC
TARGET
OBJS
LIBS
-c 编译
-o 链接
clean 目标
Tab 缩进
```

示例：

```makefile
CC = gcc
TARGET = gateway_stats
OBJS = main.o data_source.o sensor_parser.o log_writer.o mqtt_client.o app_log.o gateway_stats.o
LIBS = -lmosquitto

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)
```

注意：

```text
Makefile 命令行前面必须是 Tab，不是空格。
-o 前后要有空格。
-lmosquitto 前面要有空格。
```

## 3. 文件 I/O

掌握函数：

```text
fopen()
fprintf()
fgets()
fclose()
open()
read()
write()
close()
```

项目应用：

```text
sensor_log.txt：保存传感器业务数据
app_log.txt：保存程序运行状态
gateway_report.txt：保存本次运行统计
```

## 4. 时间函数

掌握函数：

```text
time()
localtime()
strftime()
```

项目应用：

```text
给 sensor_log 添加时间戳
给 app_log 添加时间戳
给 gateway_report 添加生成时间
```

示例格式：

```text
2026-xx-xx xx:xx:xx
```

## 5. MQTT

掌握概念：

```text
MQTT Broker
Topic
Payload
publish
subscribe
Mosquitto
QoS
```

核心命令：

```bash
sudo service mosquitto start
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

核心 API：

```text
mosquitto_lib_init()
mosquitto_new()
mosquitto_connect()
mosquitto_publish()
mosquitto_disconnect()
mosquitto_destroy()
mosquitto_lib_cleanup()
```

项目 Topic：

```text
gateway/stm32_01/sensor
```

Payload 示例：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
```

## 6. 模块化设计

项目模块：

```text
data_source
sensor_parser
log_writer
mqtt_client
app_log
gateway_stats
main.c
```

模块化好处：

```text
结构清晰
方便调试
方便扩展
方便复用
更接近真实工程
```

模块职责：

```text
data_source：提供数据源
sensor_parser：解析数据并生成 JSON
log_writer：保存传感器业务日志
mqtt_client：发布 MQTT 消息
app_log：记录程序运行状态
gateway_stats：统计运行结果
main.c：负责主流程调度
```

## 7. 异常处理

项目中的异常处理：

```text
空指针判断
返回值判断
解析失败判断
MQTT 发布失败判断
日志写入失败判断
continue 跳过异常数据
```

核心思想：

```text
网关不能因为一条坏数据直接退出。
```

异常数据：

```text
bad_data_from_stm32
```

处理方式：

```text
解析失败
记录 WARN 日志
跳过该数据
继续处理下一条
```

## 8. 日志系统

三类输出文件：

```text
sensor_log.txt
app_log.txt
gateway_report.txt
```

区别：

```text
sensor_log 看数据
app_log 看过程
gateway_report 看结果
```

日志分层的意义：

```text
方便调试
方便复盘
方便展示
方便定位问题
```

## 9. Git / GitHub

常用命令：

```bash
git status
git add 文件名
git commit -m "提交信息"
git pull --no-rebase origin main
git push
```

注意：

```text
git add 后面有空格。
不要提交 .o、可执行文件和运行日志。
```

.gitignore 应忽略：

```gitignore
*.o
a.out
gateway_stats
sensor_log.txt
app_log.txt
gateway_report.txt
```

## 10. 项目表达能力

需要能讲清楚：

```text
项目背景
项目架构
核心模块
数据流
技术难点
解决方案
项目亮点
后续扩展
```

面试表达核心：

```text
我做了一个 Linux C 物联网网关项目，模拟 STM32 设备上传温湿度数据。网关通过 data_source 模块读取数据，由 sensor_parser 模块解析为结构体并生成 JSON，然后通过 log_writer 保存业务日志，通过 mqtt_client 使用 Mosquitto 发布到 MQTT Broker。同时加入 app_log 系统日志模块记录程序运行状态，加入 gateway_stats 模块统计本次运行结果并生成报告。
```

## 11. 第一月知识闭环

第一月形成的闭环：

```text
Linux C 基础
↓
Makefile 多文件工程
↓
文件 I/O 和日志
↓
MQTT 通信
↓
模块化网关项目
↓
异常处理和运行统计
↓
GitHub 文档展示
↓
简历和面试表达
```

## 12. 下一步知识方向

第二月需要继续学习：

```text
Linux 串口 termios
真实 STM32 串口联调
MQTT 断线重连
多设备管理
配置文件读取
Qt 上位机
systemd 后台服务
项目演示视频
```
