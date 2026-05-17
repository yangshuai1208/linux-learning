# Day11 TCP Server 连续服务多个客户端

## 一、今日目标

今天完成 TCP server 连续服务多个客户端实验：

- 理解 Day10 TCP Echo server 的局限
- 学会使用外层 `while(1)` 持续等待新客户端连接
- 学会使用内层 `while(1)` 持续接收当前客户端数据
- 理解 `accept()`、`recv()`、`send()` 的配合关系
- 实现 client 断开后，server 不退出，继续等待下一个 client

## 二、项目结构

```text
day11_tcp_loop_server/
├── tcp_loop_server.c
├── README.md
└── notes/
    └── day11_note.md
```

## 三、编译命令

```bash
gcc tcp_loop_server.c -o tcp_loop_server
```

注意：

```text
gcc 和 tcp_loop_server.c 中间有空格
tcp_loop_server.c 和 -o 中间有空格
-o 和 tcp_loop_server 中间有空格
```

## 四、运行方式

第一个终端运行 Day11 server：

```bash
./tcp_loop_server
```

第二个终端运行 Day10 client：

```bash
cd ~/linux-learning-submit/day10_tcp_echo
./tcp_echo_client
```

## 五、实验现象

server 端输出：

```text
tcp loop server listening on port:8888...
client connected
recv:hello day11
client disconnected
client connected
```

client 端输出：

```text
connect server success
input message:hello day11
echo from server:hello day11
input message:quit

connect server success
input message:
```

说明：

```text
第一个 client 断开后，server 没有退出；
第二个 client 仍然可以重新连接 server。
```

## 六、核心流程

Day10 server 流程：

```text
accept()
while 循环 recv/send
close()
程序结束
```

Day11 server 流程：

```text
while 循环
    accept()
    while 循环
        recv()
        send()
    close(connfd)
    继续等待下一个 client
```

## 七、今日总结

今天实现了可以连续服务多个客户端的 TCP server。

核心思想是：

```text
外层 while 负责持续 accept 新客户端
内层 while 负责持续处理当前客户端的数据
当前客户端断开后，关闭 connfd，然后继续 accept 下一个客户端
```

这比 Day10 更接近真实服务器程序的结构。
