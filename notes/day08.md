# Day08 笔记：TCP Client 基础

## 1. 今日完成内容

今天完成了 TCP client 基础实验：

- 编写 `tcp_client.c`
- 使用 `socket()` 创建 TCP 套接字
- 使用 `connect()` 连接服务器
- 使用 `send()` 发送字符串
- 使用 `recv()` 接收服务器回复
- 使用 `nc -l 8888` 临时模拟 TCP server

## 2. TCP client 基本流程

TCP client 的基本流程：

```text
1. socket() 创建套接字
2. 设置服务器 IP 和端口
3. connect() 连接服务器
4. send() 发送数据
5. recv() 接收数据
6. close() 关闭套接字
```

## 3. socket()

```c
sockfd = socket(AF_INET, SOCK_STREAM, 0);
```

作用：创建 TCP socket。

参数解释：

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

## 4. struct sockaddr_in

```c
struct sockaddr_in server_addr;
```

作用：保存服务器地址信息。

常用成员：

```c
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(SERVER_PORT);
server_addr.sin_addr;
```

## 5. htons()

```c
server_addr.sin_port = htons(SERVER_PORT);
```

作用：把主机字节序转换成网络字节序。

网络通信中端口号要使用网络字节序。

## 6. inet_pton()

```c
ret = inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
```

作用：把字符串形式的 IP 地址转换成网络地址结构。

例如：

```text
"127.0.0.1"
```

转换后存入：

```c
server_addr.sin_addr
```

注意：

```text
inet_pton 只有 3 个参数
第三个参数是 &server_addr.sin_addr
不要写成 &server_addr, sin_addr
```

## 7. connect()

```c
ret = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
```

作用：连接 TCP server。

如果没有 server 正在监听，会出现：

```text
connect failed: Connection refused
```

这说明 client 已经尝试连接，但目标端口没有服务。

## 8. send()

```c
ret = send(sockfd, send_buf, strlen(send_buf), 0);
```

作用：发送数据到 TCP server。

本次发送：

```text
hello tcp server
```

## 9. recv()

```c
ret = recv(sockfd, recv_buf, sizeof(recv_buf) - 1, 0);
```

作用：接收服务器返回的数据。

如果 `ret > 0`，说明收到数据。

## 10. close()

```c
close(sockfd);
```

作用：关闭 socket。

socket 本质上也是文件描述符，用完需要关闭。

## 11. 今日遇到的问题

### 问题 1：数组初始化写错

错误写法：

```c
char send_buf[] += "hello tcp server\n";
```

正确写法：

```c
char send_buf[] = "hello tcp server\n";
```

### 问题 2：变量名写错

错误写法：

```c
socked = socket(AF_INET, SOCK_STREAM, 0);
```

正确写法：

```c
sockfd = socket(AF_INET, SOCK_STREAM, 0);
```

注意：

```text
sockfd 是 socket file descriptor 的缩写
```

### 问题 3：宏名拼错

错误写法：

```c
#define SEVER_PORT 8888
```

正确写法：

```c
#define SERVER_PORT 8888
```

### 问题 4：inet_pton 参数写错

错误写法：

```c
inet_pton(AF_INET, SERVER_IP, &server_addr, sin_addr);
```

正确写法：

```c
inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
```

### 问题 5：运行文件名拼错

错误写法：

```bash
./tcp_clinet
```

正确写法：

```bash
./tcp_client
```

## 12. 测试命令

安装 netcat：

```bash
sudo apt install netcat-openbsd -y
```

启动临时 server：

```bash
nc -l 8888
```

运行 client：

```bash
./tcp_client
```

## 13. 面试考点

### 问：TCP client 的基本流程是什么？

答：

```text
socket -> connect -> send/recv -> close
```

### 问：socket(AF_INET, SOCK_STREAM, 0) 表示什么？

答：

```text
创建一个 IPv4 TCP socket。
```

### 问：connect failed: Connection refused 是什么原因？

答：

```text
目标 IP 可达，但目标端口没有服务在监听。
```

### 问：htons 的作用是什么？

答：

```text
把端口号从主机字节序转换为网络字节序。
```

## 14. 和物联网网关项目的关系

后续网关会把 STM32 串口数据转发出去。

数据链路：

```text
STM32 USART1
↓
Linux termios 读取串口
↓
解析温湿度字符串
↓
TCP client 发送给服务器
```

Day8 学的 TCP client，就是后续网关 TCP 转发模块的基础。
