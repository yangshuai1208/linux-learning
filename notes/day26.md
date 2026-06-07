# Day26 笔记：系统运行日志 app_log 模块

## 1. 今日完成内容

今天在 Day25 `gateway_datasource` 基础上新增了系统运行日志模块。

完成内容：

```text
复制 Day25 工程为 Day26
新增 app_log.h
新增 app_log.c
修改 main.c 接入 app_log
修改 data_source.c 加入异常模拟数据
修改 Makefile 加入 app_log.o
编译生成 gateway_applog
测试正常数据和异常数据
生成 sensor_log.txt 和 app_log.txt
```

## 2. 今日核心思想

Day25 的日志只有一种：

```text
sensor_log.txt
```

它记录的是传感器业务数据。

Day26 新增：

```text
app_log.txt
```

它记录的是程序运行状态。

区别：

```text
sensor_log.txt：记录传感器数据
app_log.txt：记录程序状态、警告、错误
```

真实网关项目中，不能只保存传感器数据，还要能知道程序运行过程中发生了什么。

## 3. app_log.h

代码：

```c
#ifndef APP_LOG_H
#define APP_LOG_H

void app_log_info(const char *msg);
void app_log_warn(const char *msg);
void app_log_error(const char *msg);

#endif
```

作用：

```text
声明 app_log 模块对外提供的三个函数。
```

三个函数含义：

```text
app_log_info()：记录普通信息
app_log_warn()：记录警告信息
app_log_error()：记录错误信息
```

## 4. app_log.c

`app_log.c` 实现系统运行日志功能。

核心函数是：

```c
static void app_log_write(const char *level, const char *msg)
```

它是内部函数，不直接给其他文件调用。

对外暴露的是：

```c
void app_log_info(const char *msg);
void app_log_warn(const char *msg);
void app_log_error(const char *msg);
```

这三个函数最终都会调用：

```c
app_log_write()
```

## 5. static 函数

代码：

```c
static void app_log_write(const char *level, const char *msg)
```

`static` 修饰函数时，表示：

```text
这个函数只在当前 app_log.c 文件内部使用。
```

好处：

```text
隐藏内部实现
避免其他文件误调用
减少函数名冲突
```

这是一种模块封装思想。

## 6. app_log_write() 详解

核心代码：

```c
fp = fopen("app_log.txt", "a");
```

作用：

```text
以追加模式打开 app_log.txt。
```

如果文件不存在：

```text
自动创建。
```

如果文件存在：

```text
在文件末尾继续追加，不覆盖旧内容。
```

## 7. 时间戳生成

代码：

```c
now = time(NULL);
time_info = localtime(&now);
strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", time_info);
```

作用：

```text
给每一条系统日志加时间戳。
```

函数解释：

```text
time()：获取当前时间戳
localtime()：转换为本地时间
strftime()：格式化为字符串
```

生成格式：

```text
2026-05-29 20:10:30
```

## 8. 写入 app_log.txt

代码：

```c
fprintf(fp, "[%s] [%s] %s\n", time_buf, level, msg);
```

写入格式：

```text
[时间] [日志等级] 日志内容
```

示例：

```text
[2026-05-29 20:10:30] [INFO] gateway applog start
[2026-05-29 20:10:34] [WARN] parse sensor data failed
```

## 9. app_log_info()

代码：

```c
void app_log_info(const char *msg)
{
    app_log_write("INFO", msg);
}
```

作用：

```text
记录普通运行信息。
```

适合记录：

```text
程序启动
初始化成功
解析成功
MQTT 发布成功
程序结束
```

## 10. app_log_warn()

代码：

```c
void app_log_warn(const char *msg)
{
    app_log_write("WARN", msg);
}
```

作用：

```text
记录警告信息。
```

适合记录：

```text
读取数据失败
解析数据失败
收到异常数据
```

WARN 通常表示：

```text
程序还能继续运行，但出现了异常情况。
```

## 11. app_log_error()

代码：

```c
void app_log_error(const char *msg)
{
    app_log_write("ERROR", msg);
}
```

作用：

```text
记录错误信息。
```

适合记录：

```text
MQTT 初始化失败
日志写入失败
JSON 构建失败
严重资源初始化失败
```

ERROR 通常表示：

```text
功能执行失败，可能影响程序正常运行。
```

## 12. main.c 接入 app_log

Day26 在 `main.c` 中加入：

```c
#include "app_log.h"
```

程序启动时：

```c
app_log_info("gateway applog start");
```

数据源初始化成功：

```c
app_log_info("data source init success");
```

MQTT 初始化失败：

```c
app_log_error("mqtt client init failed");
```

解析失败：

```c
app_log_warn("parse sensor data failed");
```

MQTT 发布成功：

```c
app_log_info("mqtt publish success");
```

程序结束：

```c
app_log_info("gateway applog end");
```

## 13. 异常数据处理

Day26 在 mock 数据中加入：

```text
bad_data_from_stm32
```

这条数据不符合解析格式：

```text
temp=%d,humi=%d,device_id=%31s
```

所以：

```c
parse_sensor_data()
```

会返回失败。

main.c 中处理方式：

```c
ret = parse_sensor_data(raw_buf, &sensor);
if (ret != 0)
{
    printf("parse sensor data failed\n");
    app_log_warn("parse sensor data failed");
    continue;
}
```

这里的关键是：

```c
continue;
```

## 14. continue 的作用

`continue` 表示：

```text
跳过本轮循环，继续下一轮。
```

在网关程序中，这非常重要。

原因：

```text
真实网关不能因为一条错误数据就退出。
```

正确做法是：

```text
记录警告日志
跳过错误数据
继续处理后面的正常数据
```

## 15. sensor_log 和 app_log 区别

### sensor_log.txt

保存业务数据：

```text
[2026-05-29 20:10:30] {"temp":25,"humi":60,"device_id":"stm32_01"}
```

特点：

```text
只记录解析成功的传感器 JSON
异常数据不会写入
```

### app_log.txt

保存程序运行状态：

```text
[2026-05-29 20:10:34] [WARN] parse sensor data failed
```

特点：

```text
记录程序启动、初始化、成功、失败、异常
```

一句话：

```text
sensor_log 看数据，app_log 看程序。
```

## 16. Makefile 修改

Day26 新增：

```text
app_log.c
app_log.h
```

所以 Makefile 中加入：

```makefile
OBJS = main.o data_source.o sensor_parser.o log_writer.o mqtt_client.o app_log.o
```

新增规则：

```makefile
app_log.o: app_log.c app_log.h
	$(CC) -c app_log.c
```

注意：

```text
Makefile 命令前面必须是 Tab，不是空格。
```

最终链接命令类似：

```bash
gcc main.o data_source.o sensor_parser.o log_writer.o mqtt_client.o app_log.o -o gateway_applog -lmosquitto
```

## 17. 今日命令总结

复制工程：

```bash
cd ~/linux-learning-submit
cp -r day25_gateway_datasource day26_gateway_applog
cd day26_gateway_applog
```

清理旧文件：

```bash
make clean
rm -f sensor_log.txt app_log.txt
```

编辑文件：

```bash
nano app_log.h
nano app_log.c
nano data_source.c
nano main.c
nano Makefile
```

编译：

```bash
make clean
make
```

启动 MQTT：

```bash
sudo service mosquitto start
```

订阅：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

运行：

```bash
./gateway_applog
```

查看日志：

```bash
cat sensor_log.txt
cat app_log.txt
```

## 18. 今日实验成功标志

成功现象：

```text
程序可以正常运行
正常数据能解析成功
正常数据能写入 sensor_log.txt
正常数据能通过 MQTT publish
异常数据 bad_data_from_stm32 会解析失败
异常数据不会导致程序退出
异常数据会写入 app_log.txt 的 WARN 日志
```

订阅端应收到 4 条正常 JSON：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
{"temp":26,"humi":61,"device_id":"stm32_01"}
{"temp":28,"humi":63,"device_id":"stm32_01"}
{"temp":29,"humi":64,"device_id":"stm32_01"}
```

## 19. 今日遇到的关键思想

今天从“能跑”进一步升级到“能排错”。

真实项目中，只能运行成功还不够，还要能回答：

```text
程序什么时候启动？
数据源是否初始化成功？
MQTT 是否连接成功？
哪条数据解析失败？
程序是否继续运行？
```

这些问题都需要 `app_log.txt` 支持。

## 20. 和前几天的关系

Day24：

```text
模块化工程
```

Day25：

```text
新增 data_source 数据源模块
```

Day26：

```text
新增 app_log 系统日志模块
异常数据处理
```

项目逐渐从 demo 变成真实网关雏形：

```text
数据源模块
解析模块
日志模块
MQTT 模块
系统日志模块
主流程调度
```

## 21. 面试考点

### 问：为什么需要 app_log？

答：

```text
app_log 用于记录程序运行状态、错误和异常，方便后续调试、排查问题和定位故障。
```

### 问：sensor_log 和 app_log 有什么区别？

答：

```text
sensor_log 保存传感器业务数据，app_log 保存程序运行状态和错误信息。
```

### 问：为什么异常数据用 continue？

答：

```text
网关程序应长期运行，不能因为一条异常数据就退出。continue 可以跳过错误数据并继续处理下一条数据。
```

### 问：日志为什么要分 INFO / WARN / ERROR？

答：

```text
日志分级可以区分普通信息、警告和错误，方便快速定位问题严重程度。
```

### 问：static 函数有什么作用？

答：

```text
static 修饰函数时，函数只在当前 .c 文件内部可见，可以隐藏模块内部实现，减少命名冲突。
```

## 22. 今日总结

今天完成了 `app_log` 系统运行日志模块。

掌握内容：

```text
app_log 模块封装
INFO / WARN / ERROR 日志等级
系统运行日志
异常数据处理
continue 跳过错误数据
static 内部函数
sensor_log 和 app_log 分离
Makefile 新增模块
```

最关键一句话：

```text
Day26 让网关项目具备了运行状态记录和异常处理能力，使项目更接近真实 Linux 后台网关服务。
```
