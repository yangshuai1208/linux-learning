# Day27 笔记：网关运行统计 gateway_stats 模块

## 1. 今日完成内容

今天在 Day26 `gateway_applog` 基础上，新增了 `gateway_stats` 运行统计模块。

完成内容：

```text
复制 Day26 工程为 Day27
新增 gateway_stats.h
新增 gateway_stats.c
修改 main.c
修改 Makefile
编译生成 gateway_stats
运行网关程序
生成 gateway_report.txt
```

## 2. 今日核心目标

Day26 已经能记录程序运行过程：

```text
app_log.txt
```

Day27 进一步增加运行统计结果：

```text
gateway_report.txt
```

目标是让程序结束时可以清楚看到：

```text
一共处理了多少条数据
解析成功多少条
解析失败多少条
MQTT 发布成功多少条
MQTT 发布失败多少条
```

## 3. 为什么需要 gateway_stats

真实网关项目不能只看终端输出。

还需要知道系统运行质量：

```text
数据是否稳定
解析失败多不多
MQTT 发布是否正常
本次运行是否有异常
```

所以需要统计模块。

`gateway_stats` 的作用：

```text
统计本次程序运行结果
打印统计信息
保存统计报告
方便测试和展示
```

## 4. gateway_stats.h 详解

代码：

```c
#ifndef GATEWAY_STATS_H
#define GATEWAY_STATS_H

typedef struct
{
    int total_count;
    int parse_success_count;
    int parse_fail_count;
    int mqtt_success_count;
    int mqtt_fail_count;
} GatewayStats;

void gateway_stats_init(GatewayStats *stats);
void gateway_stats_print(const GatewayStats *stats);
int gateway_stats_save_report(const GatewayStats *stats);

#endif
```

作用：

```text
声明 GatewayStats 结构体
声明统计模块对外接口
```

## 5. GatewayStats 结构体

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

字段说明：

```text
total_count：总数据条数
parse_success_count：解析成功条数
parse_fail_count：解析失败条数
mqtt_success_count：MQTT 发布成功条数
mqtt_fail_count：MQTT 发布失败条数
```

这 5 个字段可以反映网关本次运行状态。

## 6. gateway_stats_init()

代码：

```c
void gateway_stats_init(GatewayStats *stats)
{
    if (stats == NULL)
    {
        return;
    }

    stats->total_count = 0;
    stats->parse_success_count = 0;
    stats->parse_fail_count = 0;
    stats->mqtt_success_count = 0;
    stats->mqtt_fail_count = 0;
}
```

作用：

```text
初始化统计结构体，把所有计数清零。
```

为什么要初始化：

```text
局部变量中的值可能是随机值
必须先清零再使用
```

主函数中调用：

```c
GatewayStats stats;
gateway_stats_init(&stats);
```

## 7. gateway_stats_print()

代码：

```c
void gateway_stats_print(const GatewayStats *stats)
{
    if (stats == NULL)
    {
        return;
    }

    printf("========== gateway stats ==========\n");
    printf("total count        : %d\n", stats->total_count);
    printf("parse success count: %d\n", stats->parse_success_count);
    printf("parse fail count   : %d\n", stats->parse_fail_count);
    printf("mqtt success count : %d\n", stats->mqtt_success_count);
    printf("mqtt fail count    : %d\n", stats->mqtt_fail_count);
    printf("===================================\n");
}
```

作用：

```text
把本次运行统计结果打印到终端。
```

输出示例：

```text
========== gateway stats ==========
total count        : 5
parse success count: 4
parse fail count   : 1
mqtt success count : 4
mqtt fail count    : 0
===================================
```

## 8. gateway_stats_save_report()

代码功能：

```text
把统计结果保存到 gateway_report.txt。
```

核心代码：

```c
fp = fopen("gateway_report.txt", "w");
```

这里使用 `"w"` 模式：

```text
每次运行重新生成报告
旧报告会被覆盖
```

这和 app_log 的 `"a"` 不一样。

## 9. "w" 和 "a" 的区别

### "w"

```c
fopen("gateway_report.txt", "w");
```

含义：

```text
写入模式
文件不存在则创建
文件存在则清空再写入
```

适合：

```text
保存本次运行报告
```

### "a"

```c
fopen("app_log.txt", "a");
```

含义：

```text
追加模式
文件不存在则创建
文件存在则追加到末尾
```

适合：

```text
保存长期运行日志
```

## 10. gateway_report.txt 内容

报告格式：

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

这个文件可以作为：

```text
测试记录
GitHub 展示材料
项目运行结果证明
```

## 11. main.c 中如何统计

Day27 在 `main.c` 中新增：

```c
GatewayStats stats;
gateway_stats_init(&stats);
```

每成功读取一条数据：

```c
stats.total_count++;
```

解析失败：

```c
stats.parse_fail_count++;
```

解析成功：

```c
stats.parse_success_count++;
```

MQTT 发布失败：

```c
stats.mqtt_fail_count++;
```

MQTT 发布成功：

```c
stats.mqtt_success_count++;
```

程序结束前：

```c
gateway_stats_print(&stats);
gateway_stats_save_report(&stats);
```

## 12. total_count 统计位置

代码：

```c
ret = data_source_read(raw_buf, sizeof(raw_buf));
if (ret != 0)
{
    printf("read data source failed\n");
    app_log_warn("read data source failed");
    continue;
}

stats.total_count++;
```

含义：

```text
只要成功读取到一条数据，就计入 total_count。
```

注意：

```text
读取失败的数据不计入 total_count。
```

## 13. parse_fail_count 统计位置

代码：

```c
ret = parse_sensor_data(raw_buf, &sensor);
if (ret != 0)
{
    stats.parse_fail_count++;

    printf("parse sensor data failed\n");
    app_log_warn("parse sensor data failed");
    continue;
}
```

含义：

```text
数据读到了，但是格式不正确，解析失败。
```

例如：

```text
bad_data_from_stm32
```

会让：

```text
parse_fail_count + 1
```

## 14. parse_success_count 统计位置

代码：

```c
stats.parse_success_count++;
```

放在解析成功后。

正常数据：

```text
temp=25,humi=60,device_id=stm32_01
```

会让：

```text
parse_success_count + 1
```

## 15. mqtt_success_count 统计位置

代码：

```c
ret = mqtt_publish_json(mosq, json_buf);
if (ret != 0)
{
    stats.mqtt_fail_count++;
    ...
    continue;
}

stats.mqtt_success_count++;
```

含义：

```text
MQTT 发布成功后，mqtt_success_count 加 1。
```

## 16. mqtt_fail_count 统计位置

当 MQTT 发布失败时：

```c
stats.mqtt_fail_count++;
```

可能原因：

```text
Mosquitto 没启动
网络连接断开
MQTT broker 异常
publish 函数返回失败
```

## 17. Day27 的完整数据流

正常数据：

```text
data_source_read()
↓
stats.total_count++
↓
parse_sensor_data()
↓
stats.parse_success_count++
↓
build_json()
↓
write_log()
↓
mqtt_publish_json()
↓
stats.mqtt_success_count++
```

异常数据：

```text
data_source_read()
↓
stats.total_count++
↓
parse_sensor_data() 失败
↓
stats.parse_fail_count++
↓
app_log_warn()
↓
continue
```

程序结束：

```text
gateway_stats_print()
↓
gateway_stats_save_report()
↓
生成 gateway_report.txt
```

## 18. Makefile 修改点

Day27 新增：

```text
gateway_stats.c
gateway_stats.h
```

所以 Makefile 增加：

```makefile
OBJS = main.o data_source.o sensor_parser.o log_writer.o mqtt_client.o app_log.o gateway_stats.o
```

新增编译规则：

```makefile
gateway_stats.o: gateway_stats.c gateway_stats.h
	$(CC) -c gateway_stats.c
```

注意：

```text
Makefile 命令行前面必须是 Tab，不是空格。
```

最终链接命令：

```bash
gcc main.o data_source.o sensor_parser.o log_writer.o mqtt_client.o app_log.o gateway_stats.o -o gateway_stats -lmosquitto
```

## 19. 今日涉及到的命令

复制工程：

```bash
cd ~/linux-learning-submit
cp -r day26_gateway_applog day27_gateway_stats
cd day27_gateway_stats
```

清理旧文件：

```bash
make clean
rm -f sensor_log.txt app_log.txt gateway_report.txt
```

编辑文件：

```bash
nano gateway_stats.h
nano gateway_stats.c
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
./gateway_stats
```

查看输出文件：

```bash
cat sensor_log.txt
cat app_log.txt
cat gateway_report.txt
```

## 20. 三个输出文件的区别

### sensor_log.txt

```text
记录传感器业务数据
```

例如：

```text
[时间] {"temp":25,"humi":60,"device_id":"stm32_01"}
```

### app_log.txt

```text
记录程序运行过程
```

例如：

```text
[时间] [WARN] parse sensor data failed
```

### gateway_report.txt

```text
记录本次运行统计结果
```

例如：

```text
total count        : 5
parse success count: 4
parse fail count   : 1
mqtt success count : 4
mqtt fail count    : 0
```

一句话：

```text
sensor_log 看数据
app_log 看过程
gateway_report 看结果
```

## 21. 今日实验成功标志

成功运行后应满足：

```text
make 编译成功
./gateway_stats 运行成功
MQTT 订阅端收到 4 条 JSON
sensor_log.txt 记录 4 条正常 JSON
app_log.txt 记录程序运行状态
gateway_report.txt 生成统计报告
parse fail count 为 1
mqtt success count 为 4
mqtt fail count 为 0
```

## 22. 面试考点

### 问：为什么需要 gateway_stats？

答：

```text
gateway_stats 用于统计网关运行结果，可以观察本次运行处理了多少条数据、成功多少条、失败多少条，方便测试、分析稳定性和展示项目结果。
```

### 问：app_log 和 gateway_stats 的区别是什么？

答：

```text
app_log 记录程序运行过程，gateway_stats 统计程序运行结果。
```

### 问：为什么要生成 gateway_report.txt？

答：

```text
gateway_report.txt 可以保存本次运行的统计结果，作为测试记录和项目展示证据。
```

### 问：为什么要统计 parse_fail_count？

答：

```text
真实串口通信中可能出现脏数据或格式异常数据，统计解析失败次数可以帮助分析数据质量和系统稳定性。
```

### 问：为什么 GatewayStats 用结构体保存？

答：

```text
结构体可以把一组相关统计数据封装在一起，方便传参、维护和扩展。
```

### 问：为什么 gateway_stats_save_report() 使用 "w" 模式？

答：

```text
因为 gateway_report.txt 表示本次运行报告，每次运行重新生成即可，不需要一直追加。
```

## 23. 今日总结

今天完成了 `gateway_stats` 运行统计模块。

掌握内容：

```text
GatewayStats 结构体
运行数据统计
统计模块封装
gateway_report.txt 生成
fopen "w" 模式
统计成功/失败次数
app_log 和 stats 的区别
Makefile 新增模块
```

最关键一句话：

```text
Day27 让网关项目从“能记录运行过程”升级为“能统计运行结果”，进一步接近真实 Linux 物联网网关项目。
```
