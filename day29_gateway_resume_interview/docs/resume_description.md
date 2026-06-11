，# Linux IoT Gateway 简历项目描述

## 项目名称

Linux IoT Gateway 物联网网关项目

## 项目描述

基于 Linux C 实现物联网网关 Demo，模拟 STM32 设备上报温湿度数据，网关端完成数据源读取、字符串解析、JSON 构建、传感器日志保存、MQTT 发布、系统运行日志记录和运行统计报告生成。

项目采用模块化设计，通过 Makefile 管理多文件编译，使用 Mosquitto C API 实现 MQTT 通信，并支持异常数据跳过处理，为后续接入真实 STM32 串口数据、多设备管理和 MQTT 断线重连预留扩展空间。

## 技术栈

- Linux C
- Makefile
- 文件 I/O
- 结构体
- 模块化设计
- MQTT
- Mosquitto
- JSON 字符串构建
- 日志系统
- 异常数据处理
- Git / GitHub

## 项目职责

1. 设计并实现 data_source 数据源模块，支持 mock 数据读取，并预留真实串口数据接入。
2. 设计并实现 sensor_parser 解析模块，将 STM32 风格字符串解析为结构体，并构建 JSON 数据。
3. 使用 Mosquitto C API 实现 MQTT 发布功能，将传感器数据上传到指定 Topic。
4. 实现 sensor_log 业务日志和 app_log 系统运行日志，区分传感器数据和程序运行状态。
5. 实现 gateway_stats 运行统计模块，统计总数据数、解析成功数、解析失败数、MQTT 发布成功数和失败数。
6. 使用 Makefile 管理多文件工程编译，并整理 README、架构说明、运行说明和测试记录。

## 项目亮点

- 模块化结构清晰，便于扩展和维护。
- 支持 MQTT 数据发布，符合物联网网关常见通信方式。
- 区分业务日志和系统日志，方便调试和问题定位。
- 支持异常数据跳过处理，提升程序健壮性。
- 支持运行统计报告，方便展示系统运行结果。
- GitHub 文档完整，具备可复现运行步骤。
