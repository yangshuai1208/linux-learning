# Day08 TCP Client 基础

## 一、今日目标

今天完成 TCP client 基础实验：

- 理解 TCP client 的基本流程
- 学会使用 `socket()` 创建套接字
- 学会使用 `connect()` 连接服务器
- 学会使用 `send()` 发送数据
- 学会使用 `recv()` 接收数据
- 使用 `nc` 临时模拟 TCP server 进行测试

## 二、项目结构

```text
day08_tcp_client/
├── tcp_client.c
├── README.md
└── notes/
    └── day08_note.md
```

## 三、编译命令

```bash
gcc tcp_client.c -o tcp_client
```

注意：

```text
gcc 和 tcp_client.c 中间有空格
tcp_client.c 和 -o 中间有空格
-o 和 tcp_client 中间有空格
```

## 四、运行命令

```bash
./tcp_client
```

如果没有服务器监听，会出现：

```text
connect failed: Connection refused
```

这不是编译错误，而是因为 `127.0.0.1:8888` 没有 TCP server。

## 五、测试方式

第一个终端启动临时 server：

```bash
nc -l 8888
```

第二个终端运行 client：

```bash
./tcp_client
```

server 端可以看到 client 发来的数据：

```text
hello tcp server
```

## 六、今日总结

今天完成了 TCP client 基础实验。

TCP client 的核心流程是：

```text
socket()
connect()
send()
recv()
close()
```

后续物联网网关项目中，Linux 网关会把从 STM32 串口接收到的数据，通过 TCP client 发送给上位机或服务器。
