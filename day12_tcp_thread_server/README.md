# Day12 多线程 TCP Server

## 一、今日目标

今天完成多线程 TCP server 实验：

- 理解 Day11 单线程循环 server 的局限
- 学会使用 `pthread_create()` 为每个客户端创建线程
- 实现多个 client 同时连接 server
- 理解 `listenfd`、`connfd` 和线程函数之间的关系
- 掌握 TCP server 多客户端并发处理的基本模型

## 二、项目结构

```text
day12_tcp_thread_server/
├── tcp_thread_server.c
├── README.md
└── notes/
    └── day12_note.md
```

## 三、编译命令

```bash
gcc tcp_thread_server.c -o tcp_thread_server -pthread
```

注意：

```text
gcc 和 tcp_thread_server.c 中间有空格
tcp_thread_server.c 和 -o 中间有空格
-o 和 tcp_thread_server 中间有空格
tcp_thread_server 和 -pthread 中间有空格
-pthread 不要写成 -pyhread
```

## 四、运行方式

第一个终端运行 Day12 server：

```bash
./tcp_thread_server
```

第二个终端运行 Day10 client：

```bash
cd ~/linux-learning-submit/day10_tcp_echo
./tcp_echo_client
```

第三个终端再运行一个 Day10 client：

```bash
cd ~/linux-learning-submit/day10_tcp_echo
./tcp_echo_client
```

## 五、实验现象

server 端输出：

```text
tcp thread server listening on port 8888...
new client connected, connfd=4
client thread start, connfd=4
new client connected, connfd=5
client thread start, connfd=5
recv from connfd 5:hello client1
recv from connfd 4:hello client2
```

client 端可以分别输入：

```text
hello client1
hello client2
```

并收到 server 回显。

## 六、核心流程

多线程 TCP server 的核心流程：

```text
socket()
bind()
listen()
while 循环
    accept()
    pthread_create()
        在线程中 recv()
        在线程中 send()
        close(connfd)
```

## 七、今日总结

Day11 的 server 虽然可以连续服务多个 client，但本质上仍然是一个一个处理。

Day12 通过多线程改进：

```text
主线程负责 accept 新客户端
子线程负责处理具体客户端的数据收发
```

这样多个 client 可以同时在线。

本实验是后续物联网网关多客户端通信和并发处理的基础。
