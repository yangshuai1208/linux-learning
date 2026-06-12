# 第一月学习总结：Linux C 与 Linux IoT Gateway

## 1. 第一月总体目标

第一个月的核心目标是完成 Linux C、网络编程、MQTT 和物联网网关项目的基础能力建设。

总体路线：

```text
Linux 基础
↓
C 语言工程化
↓
Makefile 多文件编译
↓
文件 I/O 和日志
↓
网络编程
↓
MQTT 通信
↓
Linux IoT Gateway 项目
↓
GitHub 展示和面试表达
```

## 2. 第一月完成内容

本月主要完成：

```text
Linux 基础环境
gcc 编译
Makefile
文件 I/O
pthread 基础
互斥锁
TCP/UDP 网络编程
select/poll I/O 多路复用
HTTP 静态服务器
MQTT publish / subscribe
串口读取预留
Linux IoT Gateway 项目
GitHub 每日提交
README / notes 文档整理
项目简历化和面试材料整理
```

## 3. Linux IoT Gateway 项目成果

项目名称：

```text
Linux IoT Gateway 物联网网关项目
```

项目功能：

```text
模拟 STM32 设备上传温湿度数据
Linux 网关读取数据
解析传感器字符串
生成 JSON
写入 sensor_log.txt
通过 MQTT 发布
记录 app_log.txt
生成 gateway_report.txt
处理异常数据
```

## 4. 项目模块

当前项目包含：

```text
main.c：主流程调度
data_source：数据源模块
sensor_parser：传感器解析模块
log_writer：业务日志模块
mqtt_client：MQTT 发布模块
app_log：系统运行日志模块
gateway_stats：运行统计模块
Makefile：多文件编译
README / docs / notes：项目文档
```

## 5. 项目数据流

```text
Mock STM32 Data
       ↓
data_source
       ↓
sensor_parser
       ↓
build_json
       ↓
log_writer → sensor_log.txt
       ↓
mqtt_client → Mosquitto Broker → mosquitto_sub
       ↓
gateway_stats → gateway_report.txt
```

系统运行日志：

```text
app_log → app_log.txt
```

## 6. 第一月技术收获

掌握内容：

```text
1. Linux C 程序编写和编译
2. 多文件工程组织方式
3. .h 和 .c 的分工
4. Makefile 编译规则
5. 文件读写和日志保存
6. 时间戳生成
7. MQTT 发布和订阅
8. Mosquitto C API 使用
9. 异常数据处理
10. 模块化设计思想
11. GitHub 项目提交
12. README / docs / notes 文档整理
13. 项目简历化表达
14. 面试项目讲解思路
```

## 7. 第一月项目亮点

```text
模块化结构清晰
支持 MQTT 数据发布
支持 sensor_log 和 app_log 双日志
支持异常数据跳过处理
支持 gateway_report 运行统计
GitHub 文档完整
预留真实 STM32 串口接入
可用于简历和面试讲解
```

## 8. 第一月不足

当前项目仍然存在不足：

```text
1. 当前数据源主要是 mock 数据
2. 真实 STM32 串口联调还未完全接入
3. MQTT 断线重连还未实现
4. 配置文件读取还未实现
5. 尚未支持多设备 device_id 管理
6. 尚未做后台服务 systemd
7. 尚未接入 Qt 上位机显示
8. 项目还需要录制演示视频和整理最终简历版本
```

## 9. 第一月面试表达

可以这样总结第一月项目：

```text
第一个月我完成了一个基于 Linux C 的物联网网关项目。项目模拟 STM32 设备上传温湿度数据，Linux 网关负责数据读取、字符串解析、JSON 构建、业务日志保存、MQTT 发布、系统运行日志记录和运行统计报告生成。项目采用模块化设计，通过 Makefile 管理多文件编译，并支持异常数据跳过处理。通过这个项目，我掌握了 Linux C 多文件工程、文件 I/O、MQTT 通信、日志系统、异常处理和 GitHub 项目文档整理。
```

## 10. 第一月总结

第一月完成了从 Linux C 基础到 Linux IoT Gateway 项目的阶段性闭环。

当前我已经具备：

```text
Linux C 多文件工程开发能力
基础网络通信能力
MQTT 物联网通信能力
日志与异常处理能力
项目文档整理能力
项目面试表达能力
```

第一月最重要的成果是：

```text
完成了一个可以运行、可以展示、可以写进简历的 Linux IoT Gateway 项目雏形。
```
