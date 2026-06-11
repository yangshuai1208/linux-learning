# Linux IoT Gateway 面试讲解稿

## 1. 项目一句话介绍

我做了一个基于 Linux C 的物联网网关项目，模拟 STM32 设备上传温湿度数据，Linux 网关负责解析数据、生成 JSON、写日志、通过 MQTT 上传，并生成运行统计报告。

## 2. 项目背景

这个项目是为了练习嵌入式 Linux、物联网网关和 MQTT 通信相关能力。

在真实物联网场景中，STM32 或其他 MCU 采集传感器数据后，通常需要通过串口、网络或其他方式传给 Linux 网关，再由网关进行解析、存储、上传云端或转发给上位机。

所以我设计了一个 Linux IoT Gateway Demo，用 mock 数据模拟 STM32 上报，后续可以切换为真实串口数据。

## 3. 项目整体架构

项目采用模块化设计，主要分为：

- data_source：数据源模块
- sensor_parser：传感器数据解析模块
- log_writer：传感器业务日志模块
- mqtt_client：MQTT 发布模块
- app_log：系统运行日志模块
- gateway_stats：运行统计模块
- main.c：主流程调度

数据流是：

```text
Mock STM32 Data
       ↓
data_source
       ↓
sensor_parser
       ↓
JSON
       ↓
log_writer → sensor_log.txt
       ↓
mqtt_client → Mosquitto Broker
       ↓
gateway_stats → gateway_report.txt
