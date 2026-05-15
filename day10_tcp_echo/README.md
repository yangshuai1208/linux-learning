# Day10 TCP Echo 回显程序

## 一、今日目标

今天完成 TCP 回显程序实验：

- 理解 TCP client 和 TCP server 的基本通信流程
- 在 Day8 TCP client 和 Day9 TCP server 的基础上实现双向通信
- 实现 client 发送消息，server 原样返回消息
- 理解 `recv()` 和 `send()` 在循环中的使用方式
- 理解 `recv()` 返回值为 0 时表示对端关闭连接

## 二、项目结构

```text
day10_tcp_echo/
├── tcp_echo_server.c
├── tcp_echo_client.c
├── README.md
└── notes/
    └── day10_note.md
```

## 三、编译命令

```bash
gcc tcp_echo_server.c -o tcp_echo_server
gcc tcp_echo_client.c -o tcp_echo_client
```

注意：

```text
gcc 和 tcp_echo_server.c 中间有空格
tcp_echo_server.c 和 -o 中间有空格
-o 和 tcp_echo_server 中间有空格

gcc 和 tcp_echo_client.c 中间有空格
tcp_echo_client.c 和 -o 中间有空格
-o 和 tcp_echo_client 中间有空格
```

## 四、运行方式

第一个终端运行 server：

```bash
./tcp_echo_server
```

第二个终端运行 client：

```bash
./tcp_echo_client
```

client 端输入：

```text
hello linux
```

退出 client：

```text
quit
```

## 五、实验现象

server 端输出：

```text
tcp echo server listening on port 8888...
client connected
recv:hello linux
recv:hello linux
recv:hello linux
```

client 端输出：

```text
connect server success
input message:hello linux
echo from server:hello linux
```

说明 TCP 回显程序运行成功。

## 六、核心流程

server 端流程：

```text
socket()
bind()
listen()
accept()
while 循环
    recv()
    send()
close()
```

client 端流程：

```text
socket()
connect()
while 循环
    fgets()
    send()
    recv()
close()
```

## 七、今日总结

今天完成了 TCP Echo 回显程序。

本实验的核心是：

```text
client 发什么
server 收什么
server 再原样发回去
client 接收回显结果
```

后续物联网网关项目中，可以在此基础上实现：

```text
STM32 串口数据
↓
Linux 网关接收
↓
TCP 发送给服务器
↓
服务器回复 OK 或控制命令
```
