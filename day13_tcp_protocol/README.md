# Day13 TCP 简单应用层协议

## 一、今日目标

今天完成 TCP 简单应用层协议实验：

- 理解应用层协议的基本概念
- 在 TCP server 中加入命令解析功能
- 支持 `ping`、`get`、`help` 等命令
- 根据 client 发送的不同命令返回不同内容
- 为后续物联网网关项目中的命令控制和数据查询打基础

## 二、项目结构

```text
day13_tcp_protocol/
├── tcp_protocol_server.c
├── README.md
└── notes/
    └── day13_note.md
```

## 三、编译命令

```bash
gcc tcp_protocol_server.c -o tcp_protocol_server -pthread
```

注意：

```text
gcc 和 tcp_protocol_server.c 中间有空格
tcp_protocol_server.c 和 -o 中间有空格
-o 和 tcp_protocol_server 中间有空格
tcp_protocol_server 和 -pthread 中间有空格
```

## 四、运行方式

第一个终端运行 server：

```bash
./tcp_protocol_server
```

第二个终端运行 Day10 的 client：

```bash
cd ~/linux-learning-submit/day10_tcp_echo
./tcp_echo_client
```

## 五、实验现象

client 端输入：

```text
ping
```

返回：

```text
echo from server:pong
```

client 端输入：

```text
get
```

返回：

```text
echo from server:temp=25,humi=60,device_id=stm32_01
```

client 端输入：

```text
help
```

返回：

```text
echo from server:commands: ping, get, help
```

client 端输入未知命令：

```text
abc
```

返回：

```text
echo from server:unknown command
```

server 端可以看到：

```text
tcp protocol server listening on port 8888...
new client connected, connfd=4
client thread start, connfd=4
recv from connfd 4:ping
recv from connfd 4:get
recv from connfd 4:help
recv from connfd 4:abc
```

## 六、核心功能

本实验定义了一个简单的应用层协议：

```text
ping  -> pong
get   -> temp=25,humi=60,device_id=stm32_01
help  -> commands: ping, get, help
其他命令 -> unknown command
```

## 七、今日总结

今天完成了基于 TCP 的简单命令处理 server。

和前几天相比：

```text
Day10：server 收到什么就回显什么
Day13：server 根据命令返回不同结果
```

这说明 TCP 通信已经从简单收发数据，进一步发展为“按协议处理命令”。

后续物联网网关中，可以把 `get` 命令替换为真实的 STM32 温湿度数据查询。
