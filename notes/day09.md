# Day09 笔记：TCP Server 基础

## 1. 今日完成内容

今天完成了 TCP server 基础实验：

- 编写 `tcp_server.c`
- 使用 `socket()` 创建 TCP socket
- 使用 `bind()` 绑定本机端口
- 使用 `listen()` 监听连接
- 使用 `accept()` 接收客户端连接
- 使用 `recv()` 接收客户端数据
- 使用 `send()` 回复客户端数据
- 使用 Day8 的 `tcp_client` 测试 server

## 2. TCP server 基本流程

TCP server 的标准流程：

```text
1. socket() 创建监听 socket
2. bind() 绑定 IP 和端口
3. listen() 开始监听
4. accept() 接收客户端连接
5. recv() 接收客户端数据
6. send() 回复客户端
7. close() 关闭 socket
```

和 TCP client 对比：

```text
TCP client：socket -> connect -> send/recv -> close
TCP server：socket -> bind -> listen -> accept -> recv/send -> close
```

## 3. socket()

```c
listenfd = socket(AF_INET, SOCK_STREAM, 0);
```

作用：创建一个 TCP socket。

参数说明：

```text
AF_INET：IPv4
SOCK_STREAM：TCP 字节流
0：使用默认协议
```

返回值：

```text
成功：返回 socket 文件描述符
失败：返回 -1
```

## 4. setsockopt()

```c
setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```

作用：允许端口复用。

这样程序退出后，再次启动 server 时，不容易出现：

```text
bind failed: Address already in use
```

## 5. bind()

```c
bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
```

作用：把 socket 绑定到本机 IP 和端口。

本次绑定端口：

```text
8888
```

关键配置：

```c
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(SERVER_PORT);
server_addr.sin_addr.s_addr = INADDR_ANY;
```

含义：

```text
AF_INET：IPv4
htons(SERVER_PORT)：把端口转为网络字节序
INADDR_ANY：监听本机所有网卡地址
```

## 6. listen()

```c
listen(listenfd, BACKLOG);
```

作用：让 socket 进入监听状态，等待客户端连接。

`BACKLOG` 表示连接等待队列长度。

```c
#define BACKLOG 5
```

## 7. accept()

```c
connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
```

作用：接收一个客户端连接。

注意：

```text
listenfd：监听 socket，只负责等待连接
connfd：通信 socket，负责和具体客户端收发数据
```

这是面试常问点。

## 8. recv()

```c
ret = recv(connfd, recv_buf, sizeof(recv_buf) - 1, 0);
```

作用：接收客户端发来的数据。

如果：

```c
ret > 0
```

说明接收到数据。

本次收到：

```text
hello tcp server
```

## 9. send()

```c
send(connfd, send_buf, strlen(send_buf), 0);
```

作用：向客户端回复数据。

本次回复：

```text
server received
```

## 10. close()

```c
close(connfd);
close(listenfd);
```

作用：关闭 socket 文件描述符。

注意：

```text
connfd 是和客户端通信的 socket
listenfd 是监听连接的 socket
两个都要关闭
```

## 11. 今日测试流程

第一个终端启动 server：

```bash
cd ~/linux-learning-submit/day09_tcp_server
./tcp_server
```

第二个终端运行 Day8 client：

```bash
cd ~/linux-learning-submit/day08_tcp_client
./tcp_client
```

测试成功现象：

```text
server 端：
tcp server listening on port 8888...
client connected
recv from client:hello tcp server

client 端：
connect server success
send:hello tcp server
recv:server received
```

## 12. 今日遇到的问题

### 问题 1：目录名输错

错误示例：

```bash
cd day00_tcp_server
```

正确：

```bash
cd day09_tcp_server
```

注意：

```text
今天是 Day9，不是 Day0
```

### 问题 2：端口可能被占用

如果出现：

```text
bind failed: Address already in use
```

原因可能是：

```text
1. 上一个 tcp_server 还在运行
2. nc -l 8888 还没关闭
3. 端口处于短暂占用状态
```

解决：

```text
关闭旧 server 或 nc 终端，等待几秒后重试
```

## 13. 面试考点

### 问：TCP server 的基本流程是什么？

答：

```text
socket -> bind -> listen -> accept -> recv/send -> close
```

### 问：bind 的作用是什么？

答：

```text
把 socket 绑定到本机 IP 和端口。
```

### 问：listen 的作用是什么？

答：

```text
让 socket 进入监听状态，等待客户端连接。
```

### 问：accept 的作用是什么？

答：

```text
接收客户端连接，并返回一个新的通信 socket。
```

### 问：listenfd 和 connfd 的区别是什么？

答：

```text
listenfd 用于监听连接，connfd 用于和某一个客户端实际通信。
```

### 问：为什么端口要用 htons？

答：

```text
因为网络通信要求端口号使用网络字节序。
```

## 14. 和物联网网关项目的关系

Day9 是网关 TCP 通信能力的基础。

后续可能有两种模式：

```text
模式1：Linux 网关作为 TCP client，主动把 STM32 数据上传给服务器
模式2：Linux 网关作为 TCP server，等待上位机连接并读取数据
```

后续数据链路：

```text
STM32 USART1
↓
Linux termios 读取串口
↓
解析传感器数据
↓
TCP server / TCP client 进行网络通信
```

Day9 完成的是 TCP server 基础能力。
