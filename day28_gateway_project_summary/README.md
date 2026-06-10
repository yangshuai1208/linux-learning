# Day28 Linux IoT Gateway 项目阶段性总结

## 一、项目简介

本项目是一个基于 Linux C 的物联网网关 Demo。

项目模拟 STM32 设备上传传感器数据，Linux 网关负责完成数据读取、数据解析、JSON 生成、日志保存、MQTT 发布和运行统计。

项目适合作为嵌入式 Linux、物联网网关、智能硬件方向的阶段性项目。

## 二、项目功能

当前已实现：

- data_source 数据源模块
- sensor_parser 数据解析模块
- log_writer 传感器日志模块
- mqtt_client MQTT 发布模块
- app_log 系统运行日志模块
- gateway_stats 运行统计模块
- Makefile 多文件编译
- 异常数据跳过处理

## 三、项目架构

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
