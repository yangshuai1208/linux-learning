# Day12 笔记：多线程 TCP Server

## 1. 今日完成内容

今天完成了多线程 TCP server 实验：

- 编写 `tcp_thread_server.c`
- 使用 `socket()` 创建监听 socket
- 使用 `bind()` 绑定 8888 端口
- 使用 `listen()` 监听客户端连接
- 使用 `accept()` 接收客户端连接
- 使用 `pthread_create()` 为每个客户端创建独立线程
- 在线程函数中使用 `recv()` 和 `send()` 处理客户端数据
- 实现多个客户端同时连接 server

最终实验现象：

```text
new client connected, connfd=4
client thread start, connfd=4
new client connected, connfd=5
client thread start, connfd=5
recv from connfd 5:hello client1
recv from connfd 4:hello client2
```

说明多个 client 可以同时连接并通信。

## 2. Day11 的问题

Day11 的 server 可以连续服务多个客户端，但仍然是单线程模型。

特点：

```text
client1 连接
server 处理 client1
client1 断开
server 再处理 client2
```

问题：

```text
如果 client1 一直不退出，client2 就只能等待。
```

## 3. Day12 的改进

Day12 使用多线程模型：

```text
主线程：负责 accept 新客户端
子线程：负责处理某一个客户端的 recv/send
```

这样：

```text
client1 连接后交给线程1处理
client2 连接后交给线程2处理
两个客户端可以同时通信
```

## 4. 多线程 TCP server 基本流程

```text
socket()
bind()
listen()
while (1)
{
    connfd = accept();
    pthread_create();
}
```

线程函数中：

```text
while (1)
{
    recv();
    send();
}
close(connfd);
```

## 5. socket()

```c
listenfd = socket(AF_INET, SOCK_STREAM, 0);
```

作用：

```text
创建 TCP 监听 socket。
```

参数：

```text
AF_INET：IPv4
SOCK_STREAM：TCP
0：默认协议
```

## 6. bind()

```c
bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
```

作用：

```text
把 listenfd 绑定到本机 8888 端口。
```

## 7. listen()

```c
listen(listenfd, BACKLOG);
```

作用：

```text
让 socket 进入监听状态，等待客户端连接。
```

## 8. accept()

```c
connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
```

作用：

```text
接收一个客户端连接，返回用于通信的 connfd。
```

注意：

```text
listenfd 负责监听
connfd 负责和具体 client 通信
```

## 9. pthread_create()

```c
pthread_create(&tid, NULL, client_thread, &connfd);
```

作用：

```text
创建一个子线程，让子线程处理当前客户端。
```

参数含义：

```text
&tid：线程 ID
NULL：使用默认线程属性
client_thread：线程函数
&connfd：传给线程函数的参数
```

编译时必须加：

```bash
-pthread
```

完整编译命令：

```bash
gcc tcp_thread_server.c -o tcp_thread_server -pthread
```

## 10. 线程函数 client_thread

线程函数一般写成：

```c
void *client_thread(void *arg)
{
    int connfd = *(int *)arg;

    while (1)
    {
        recv();
        send();
    }

    close(connfd);
    return NULL;
}
```

作用：

```text
每个线程独立处理一个 client。
```

## 11. recv()

```c
ret = recv(connfd, recv_buf, sizeof(recv_buf) - 1, 0);
```

返回值：

```text
ret > 0：收到数据
ret == 0：客户端正常断开
ret < 0：接收失败
```

## 12. send()

```c
send(connfd, recv_buf, strlen(recv_buf), 0);
```

作用：

```text
把收到的数据回发给 client。
```

## 13. close(connfd)

```c
close(connfd);
```

作用：

```text
关闭当前客户端连接。
```

注意：

```text
子线程中只能关闭 connfd
不能关闭 listenfd
```

因为 listenfd 是主线程用来继续 accept 新客户端的。

## 14. 今日遇到的问题

### 问题 1：宏名拼错

错误写法：

```c
SERVRE_PORT
```

正确写法：

```c
SERVER_PORT
```

注意：

```text
SERVER 是服务器
不要写成 SERVRE
```

### 问题 2：大括号不匹配

错误现象：

```text
expected declaration or statement at end of input
```

原因：

```text
少了右大括号 }
或者 if / while / 函数的大括号没有配对。
```

解决：

```text
检查每一个 { 是否都有对应的 }。
```

### 问题 3：忘记加 -pthread

如果编译 pthread 程序，必须加：

```bash
-pthread
```

否则可能出现 pthread 相关链接错误。

## 15. Day11 和 Day12 对比

Day11：

```text
单线程 server
一个 client 断开后，才能处理下一个 client
```

Day12：

```text
多线程 server
每个 client 一个线程
多个 client 可以同时通信
```

## 16. 面试考点

### 问：多线程 TCP server 的基本思路是什么？

答：

```text
主线程负责 accept 客户端连接，每接收一个客户端连接，就创建一个子线程处理该客户端的 recv/send。
```

### 问：为什么要使用多线程？

答：

```text
为了让多个客户端可以同时连接和通信，避免一个客户端阻塞整个 server。
```

### 问：listenfd 和 connfd 的区别？

答：

```text
listenfd 用于监听连接，connfd 用于和某个具体客户端通信。
```

### 问：pthread_create 编译时为什么要加 -pthread？

答：

```text
因为 pthread 属于 POSIX 线程库，编译链接时需要显式启用线程支持。
```

## 17. 和物联网网关项目的关系

Day12 是后续网关并发通信的基础。

后续可能的网关模型：

```text
主线程：接收 TCP client 连接
线程1：处理上位机 client1
线程2：处理上位机 client2
串口线程：读取 STM32 数据
日志线程：保存数据日志
```

Day12 训练的是：

```text
多线程 + TCP 网络通信
```

这是后续网关整合阶段的重要基础。
