# Day28 笔记：Linux IoT Gateway 阶段性项目整理

## 1. 今日目标

今天对 Day24 到 Day27 的 Linux IoT Gateway 项目进行阶段性整理。

今天不新增核心代码功能，重点是把已有网关项目整理成一个可以放到 GitHub、可以写进简历、可以在面试中讲清楚的项目版本。

今日目标：

```text
1. 复制 Day27 工程为 Day28
2. 新增 docs 文档目录
3. 编写项目架构说明
4. 编写项目运行说明
5. 编写项目测试记录
6. 完善 README.md
7. 整理 GitHub 展示材料
8. 提交 Day28 项目总结版本
```

## 2. 今日完成内容

今天完成了：

```text
复制 day27_gateway_stats 为 day28_gateway_project_summary
新增 docs/architecture.md
新增 docs/run_guide.md
新增 docs/test_record.md
完善 README.md
创建 notes/day28_note.md
整理 .gitignore
准备 GitHub 提交
```

Day28 的重点是：

```text
把能运行的代码整理成能展示的项目。
```

## 3. 当前项目结构

Day28 项目结构：

```text
day28_gateway_project_summary/
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
├── docs/
│   ├── architecture.md
│   ├── run_guide.md
│   └── test_record.md
└── notes/
    └── day28_note.md
```

## 4. docs 目录的作用

今天新增了 `docs` 目录。

作用：

```text
存放项目详细说明文档
避免 README.md 过长
让项目结构更规范
方便 GitHub 展示和面试复盘
```

当前 docs 目录包括：

```text
architecture.md：项目架构说明
run_guide.md：项目运行说明
test_record.md：项目测试记录
```

## 5. architecture.md 作用

`architecture.md` 用来说明项目整体架构。

核心数据流：

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

这个文件主要解决：

```text
项目整体怎么设计
数据怎么流动
每个模块负责什么
项目后续如何扩展
```

## 6. run_guide.md 作用

`run_guide.md` 用来说明如何运行项目。

主要包括：

```text
环境要求
依赖安装
项目编译
启动 Mosquitto
订阅 MQTT Topic
运行网关程序
查看日志和报告
正确实验现象
```

核心命令：

```bash
sudo apt install mosquitto mosquitto-clients libmosquitto-dev
make clean
make
sudo service mosquitto start
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
./gateway_stats
```

这个文档的意义是：

```text
别人拿到项目后，可以根据说明复现实验。
```

## 7. test_record.md 作用

`test_record.md` 用来记录项目测试过程和测试结果。

测试输入：

```text
temp=25,humi=60,device_id=stm32_01
temp=26,humi=61,device_id=stm32_01
bad_data_from_stm32
temp=28,humi=63,device_id=stm32_01
temp=29,humi=64,device_id=stm32_01
```

其中：

```text
bad_data_from_stm32
```

用于模拟异常数据。

预期结果：

```text
正常数据可以解析、写日志、发布 MQTT
异常数据解析失败，写入 app_log 的 WARN 日志
异常数据不写入 sensor_log
异常数据不发布 MQTT
程序继续运行，不崩溃
```

运行统计结果：

```text
total count        : 5
parse success count: 4
parse fail count   : 1
mqtt success count : 4
mqtt fail count    : 0
```

这个文件可以作为：

```text
项目测试记录
GitHub 展示材料
面试时证明项目可运行的依据
```

## 8. README.md 的作用

README.md 是项目首页说明文档。

今天 README.md 重点整理了：

```text
项目简介
项目功能
项目架构
项目结构
编译运行方式
实验现象
输出文件说明
异常数据处理
已掌握知识点
后续扩展方向
```

README 的作用：

```text
让别人快速知道这个项目是什么、怎么运行、有什么亮点。
```

一句话：

```text
README 是项目的门面。
```

## 9. 当前项目支持的功能

当前 Linux IoT Gateway 已经支持：

```text
data_source 数据源模块
sensor_parser 数据解析模块
log_writer 传感器日志模块
mqtt_client MQTT 发布模块
app_log 系统运行日志模块
gateway_stats 运行统计模块
Makefile 多文件编译
异常数据跳过处理
```

当前输出文件：

```text
sensor_log.txt
app_log.txt
gateway_report.txt
```

## 10. 三个输出文件区别

### sensor_log.txt

作用：

```text
记录传感器业务数据。
```

示例：

```text
[时间] {"temp":25,"humi":60,"device_id":"stm32_01"}
```

特点：

```text
只保存解析成功的正常数据。
```

### app_log.txt

作用：

```text
记录程序运行过程。
```

示例：

```text
[时间] [INFO] gateway stats start
[时间] [WARN] parse sensor data failed
```

特点：

```text
保存程序启动、初始化、错误、警告、结束等运行状态。
```

### gateway_report.txt

作用：

```text
记录本次运行统计结果。
```

示例：

```text
total count        : 5
parse success count: 4
parse fail count   : 1
mqtt success count : 4
mqtt fail count    : 0
```

一句话总结：

```text
sensor_log 看数据
app_log 看过程
gateway_report 看结果
```

## 11. 当前项目架构理解

当前项目采用模块化设计。

模块分工：

```text
main.c：主流程调度
data_source：提供数据源
sensor_parser：解析字符串并生成 JSON
log_writer：保存传感器数据日志
mqtt_client：发布 MQTT 消息
app_log：记录程序运行状态
gateway_stats：统计本次运行结果
Makefile：统一编译多文件工程
```

这种设计的好处：

```text
代码结构清晰
每个模块职责明确
后续方便维护和扩展
更接近真实 Linux C 项目
```

## 12. .gitignore 的作用

今天提交 GitHub 前，需要更新 `.gitignore`。

建议忽略：

```gitignore
*.o
a.out

gateway_module
gateway_datasource
gateway_applog
gateway_stats

sensor_log.txt
app_log.txt
gateway_report.txt
```

原因：

```text
.o 文件是编译中间文件
gateway_stats 是可执行文件
sensor_log.txt / app_log.txt / gateway_report.txt 是运行生成文件
这些不属于源码，不应该提交到 GitHub
```

GitHub 应该提交：

```text
.c 源文件
.h 头文件
Makefile
README.md
docs 文档
notes 笔记
```

## 13. GitHub 提交流程

Day28 提交命令：

```bash
cd ~/linux-learning-submit
git status
git add day28_gateway_project_summary .gitignore
git commit -m "docs: add day28 gateway project summary"
git pull --no-rebase origin main
git push
```

注意：

```text
git add 后面有空格
day28_gateway_project_summary 和 .gitignore 中间有空格
```

如果误提交了可执行文件或日志文件，可以取消暂存：

```bash
git restore --staged day28_gateway_project_summary/gateway_stats
git restore --staged day28_gateway_project_summary/*.o
git restore --staged day28_gateway_project_summary/sensor_log.txt
git restore --staged day28_gateway_project_summary/app_log.txt
git restore --staged day28_gateway_project_summary/gateway_report.txt
```

## 14. 今日涉及到的命令

复制工程：

```bash
cd ~/linux-learning-submit
cp -r day27_gateway_stats day28_gateway_project_summary
cd day28_gateway_project_summary
```

清理旧文件：

```bash
make clean
rm -f sensor_log.txt app_log.txt gateway_report.txt
```

创建 docs 目录：

```bash
mkdir -p docs
```

编辑文档：

```bash
nano README.md
nano docs/architecture.md
nano docs/run_guide.md
nano docs/test_record.md
nano notes/day28_note.md
```

编译运行：

```bash
make clean
make
sudo service mosquitto start
./gateway_stats
```

查看输出：

```bash
cat sensor_log.txt
cat app_log.txt
cat gateway_report.txt
```

## 15. 今日重点理解

Day28 的重点不是写新代码，而是项目整理能力。

真实求职中，只会写代码还不够，还需要能说明：

```text
这个项目是什么
为什么这样设计
每个模块做什么
怎么运行
测试结果是什么
项目有什么亮点
后续还能怎么扩展
```

这就是项目表达能力。

## 16. 当前项目亮点

当前项目可以总结为：

```text
基于 Linux C 的物联网网关 Demo
使用模块化设计
使用 Makefile 构建
使用 Mosquitto 实现 MQTT 发布
支持传感器数据解析和 JSON 生成
支持业务日志和系统日志分离
支持异常数据跳过处理
支持运行统计报告生成
文档结构完整，方便 GitHub 展示
```

## 17. 面试表达版本

面试中可以这样讲：

```text
我做了一个 Linux C 物联网网关项目，模拟 STM32 设备上传温湿度数据。网关通过 data_source 模块读取数据，由 sensor_parser 模块解析为结构体并生成 JSON，然后通过 log_writer 保存业务日志，通过 mqtt_client 使用 Mosquitto 发布到 MQTT Broker。同时我加入了 app_log 系统日志模块记录程序运行状态，加入 gateway_stats 模块统计本次运行结果并生成报告。项目采用 Makefile 管理多文件编译，并整理了 README、架构说明、运行说明和测试记录，方便复现和展示。
```

## 18. 今日面试考点

### 问：为什么要写项目文档？

答：

```text
项目文档可以说明项目背景、架构、运行方式和测试结果，方便别人复现项目，也方便面试时展示项目能力。
```

### 问：README 和 docs 有什么区别？

答：

```text
README 是项目入口文档，负责快速介绍项目；docs 存放更详细的架构说明、运行说明和测试记录。
```

### 问：为什么运行日志不提交 GitHub？

答：

```text
运行日志属于程序运行产生的临时文件，不是源码文件。一般通过 .gitignore 忽略，避免污染仓库。
```

### 问：当前项目的工程化体现在哪里？

答：

```text
模块化设计、Makefile 构建、MQTT 通信封装、日志分层、异常数据处理、运行统计报告、项目文档化整理。
```

### 问：后续这个项目怎么扩展？

答：

```text
可以接入真实 STM32 串口数据，增加 MQTT 断线重连，支持多设备 device_id，增加配置文件读取，增加 systemd 后台服务，或者增加 Qt 上位机显示。
```

## 19. 今日总结

Day28 完成了 Linux IoT Gateway 的阶段性项目整理。

今天掌握：

```text
GitHub 项目展示整理
README 编写
docs 文档目录设计
项目架构说明
运行说明编写
测试记录编写
.gitignore 使用
Git 提交流程
项目面试表达
```

最关键一句话：

```text
Day28 把 Linux IoT Gateway 从“能运行的代码”整理成了“能展示、能复现、能写进简历的项目版本”。
```
