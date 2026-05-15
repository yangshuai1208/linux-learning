# Day09 TCP Server 基础

## 一、今日目标

今天完成 TCP server 基础实验：

- 理解 TCP server 的基本流程
- 学会使用 `socket()` 创建监听套接字
- 学会使用 `bind()` 绑定本机端口
- 学会使用 `listen()` 进入监听状态
- 学会使用 `accept()` 接收客户端连接
- 学会使用 `recv()` 接收客户端数据
- 学会使用 `send()` 回复客户端数据

## 二、项目结构

```text
day09_tcp_server/
├── tcp_server.c
├── README.md
└── notes/
    └── day09_note.md
```

## 三、编译命令

```bash
gcc tcp_server.c -o tcp_server
```

注意：

```text
gcc 和 tcp_server.c 中间有空格
tcp_server.c 和 -o 中间有空格
-o 和 tcp_server 中间有空格
```

## 四、运行方式

第一个终端运行 server：

```bash
./tcp_server
```

第二个终端运行 Day8 的 client：

```bash
cd ~/linux-learning-submit/day08_tcp_client
./tcp_client
```

## 五、实验现象

server 端输出：

```text
tcp server listening on port 8888...
client connected
recv from client:hello tcp server
```

client 端输出：

```text
connect server success
send:hello tcp server
recv:server received
```

说明 TCP server 成功监听 8888 端口，并成功接收 client 发来的数据，同时向 client 返回回复。

## 六、核心流程

TCP server 的基本流程：

```text
socket()
bind()
listen()
accept()
recv()
send()
close()
```

## 七、今日总结

今天完成了 TCP server 基础实验。

Day8 学的是 TCP client：

```text
socket -> connect -> send/recv -> close
```

Day9 学的是 TCP server：

```text
socket -> bind -> listen -> accept -> recv/send -> close
```

后续物联网网关项目中，Linux 网关既可以作为 TCP client 主动上传数据，也可以作为 TCP server 等待上位机连接。
