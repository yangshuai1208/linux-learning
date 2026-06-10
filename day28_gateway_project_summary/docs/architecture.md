# Linux IoT Gateway 项目架构说明

## 1. 项目定位

本项目是一个基于 Linux C 的物联网网关 Demo，主要模拟 STM32 设备上传传感器数据，Linux 网关负责解析、日志保存、MQTT 上传和运行统计。

项目目标：

- 模拟嵌入式设备数据上报
- Linux 网关解析传感器数据
- 生成 JSON 数据
- 通过 MQTT 发布到 Broker
- 保存传感器日志
- 保存系统运行日志
- 生成运行统计报告

## 2. 系统数据流

```text
STM32 / Mock Data
        ↓
data_source
        ↓
sensor_parser
        ↓
JSON payload
        ↓
log_writer  → sensor_log.txt
        ↓
mqtt_client → Mosquitto Broker
        ↓
mosquitto_sub
