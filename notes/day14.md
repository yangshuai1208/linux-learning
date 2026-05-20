# Day14 笔记：第 2 周 TCP Socket 总结

## 1. 本周学习内容

第 2 周主要学习 Linux TCP Socket 网络编程。

学习顺序：

```text
Day8：TCP client
Day9：TCP server
Day10：TCP echo 回显程序
Day11：循环 server
Day12：多线程 server
Day13：简单应用层协议
Day14：总结
```

本周主线：

```text
基础连接
↓
数据收发
↓
持续服务
↓
并发处理
↓
协议解析
```

## 2. TCP Client

TCP client 是主动连接服务器的一方。

基本流程：

```text
socket()
connect()
send()
recv()
close()
```

核心代码：

```c
sockfd = socket(AF_INET, SOCK_STREAM, 0);

connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

send(sockfd, send_buf, strlen(send_buf), 0);

recv(sockfd, recv_buf, sizeof(recv_buf) - 1, 0);

close(sockfd);
```

常用场景：

```text
Linux 网关主动把 STM32 数据上传给服务器。
```

## 3. TCP Server

TCP server 是监听端口、等待客户端连接的一方。

基本流程：

```text
socket()
bind()
listen()
accept()
recv()
send()
close()
```

核心代码：

```c
listenfd = socket(AF_INET, SOCK_STREAM, 0);

bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

listen(listenfd, BACKLOG);

connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);

recv(connfd, recv_buf, sizeof(recv_buf) - 1, 0);

send(connfd, send_buf, strlen(send_buf), 0);
```

常用场景：

```text
Linux 网关作为 server，等待上位机连接并查询数据。
```

## 4. socket()

```c
socket(AF_INET, SOCK_STREAM, 0);
```

作用：

```text
创建一个 TCP socket。
```

参数解释：

```text
AF_INET：IPv4
SOCK_STREAM：TCP 字节流
0：默认协议
```

返回值：

```text
成功：返回 socket 文件描述符
失败：返回 -1
```

## 5. bind()

```c
bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
```

作用：

```text
把 socket 绑定到本机 IP 和端口。
```

server 端必须使用。

client 端一般不需要手动 bind。

## 6. listen()

```c
listen(listenfd, BACKLOG);
```

作用：

```text
让 socket 进入监听状态，等待客户端连接。
```

`BACKLOG` 表示连接等待队列长度。

## 7. accept()

```c
connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
```

作用：

```text
接收客户端连接。
```

注意：

```text
accept 默认是阻塞函数。
如果没有 client 连接，程序会停在 accept 这里等待。
```

返回值：

```text
成功：返回 connfd
失败：返回 -1
```

## 8. listenfd 和 connfd

这是本周最重要的面试点。

```text
listenfd：监听 socket，只负责 accept 新连接。
connfd：通信 socket，负责和某一个客户端 recv/send。
```

错误理解：

```text
用 listenfd 去 recv/send。
```

正确理解：

```text
listenfd 负责监听。
accept 返回的 connfd 负责通信。
```

## 9. recv()

```c
ret = recv(connfd, recv_buf, sizeof(recv_buf) - 1, 0);
```

作用：

```text
接收客户端发来的数据。
```

返回值：

```text
ret > 0：收到数据
ret == 0：对端正常关闭连接
ret < 0：接收失败
```

`ret == 0` 很重要，表示 client 正常断开。

## 10. send()

```c
send(connfd, send_buf, strlen(send_buf), 0);
```

作用：

```text
向对端发送数据。
```

在 Echo 程序中：

```c
send(connfd, recv_buf, strlen(recv_buf), 0);
```

表示收到什么就返回什么。

## 11. Day10：TCP Echo

TCP Echo 的核心：

```text
client 发送数据
server 接收数据
server 原样返回数据
client 收到回显数据
```

核心代码：

```c
ret = recv(connfd, recv_buf, sizeof(recv_buf) - 1, 0);

send(connfd, recv_buf, strlen(recv_buf), 0);
```

Echo 程序是网络通信调试中非常基础的模型。

## 12. Day11：循环 Server

Day11 的核心是双层 while：

```c
while (1)
{
    connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);

    while (1)
    {
        ret = recv(connfd, recv_buf, sizeof(recv_buf) - 1, 0);

        if (ret > 0)
        {
            send(connfd, recv_buf, strlen(recv_buf), 0);
        }
        else if (ret == 0)
        {
            break;
        }
        else
        {
            break;
        }
    }

    close(connfd);
}
```

外层 while：

```text
持续等待新的客户端。
```

内层 while：

```text
持续处理当前客户端的数据。
```

## 13. Day12：多线程 Server

Day12 的核心思想：

```text
主线程负责 accept
子线程负责 recv/send
```

核心代码：

```c
pthread_create(&tid, NULL, client_thread, pconnfd);
pthread_detach(tid);
```

编译命令：

```bash
gcc tcp_thread_server.c -o tcp_thread_server -pthread
```

注意：

```text
-pthread 不能漏。
```

多线程 server 的好处：

```text
多个客户端可以同时连接和通信。
```

## 14. malloc 传递 connfd

推荐写法：

```c
int *pconnfd = malloc(sizeof(int));
*pconnfd = connfd;

pthread_create(&tid, NULL, client_thread, pconnfd);
```

线程函数中：

```c
int connfd = *(int *)arg;
free(arg);
```

原因：

```text
不能长期直接把 &connfd 传给线程。
因为 connfd 是主线程变量，下一次 accept 可能会修改它。
```

## 15. Day13：应用层协议

TCP 只负责可靠传输字节流，不规定数据含义。

应用层协议负责定义：

```text
客户端发什么
服务器如何解析
服务器返回什么
```

Day13 定义的协议：

```text
ping -> pong
get  -> temp=25,humi=60,device_id=stm32_01
help -> commands: ping, get, help
其他命令 -> unknown command
```

核心函数：

```c
void handle_command(char *cmd, char *reply, int reply_size)
{
    if (strncmp(cmd, "ping", 4) == 0)
    {
        snprintf(reply, reply_size, "pong\n");
    }
    else if (strncmp(cmd, "get", 3) == 0)
    {
        snprintf(reply, reply_size, "temp=25,humi=60,device_id=stm32_01\n");
    }
    else if (strncmp(cmd, "help", 4) == 0)
    {
        snprintf(reply, reply_size, "commands: ping, get, help\n");
    }
    else
    {
        snprintf(reply, reply_size, "unknown command\n");
    }
}
```

## 16. strncmp()

```c
strncmp(cmd, "ping", 4)
```

作用：

```text
比较 cmd 前 4 个字符是否等于 ping。
```

为什么不用 strcmp：

```text
因为 fgets 读取的字符串可能带有换行符，例如 ping\n。
strncmp 只比较前几个有效字符，更适合简单命令判断。
```

## 17. snprintf()

```c
snprintf(reply, reply_size, "pong\n");
```

作用：

```text
安全地把字符串写入 reply 缓冲区。
```

相比 `sprintf()`：

```text
snprintf 可以限制最大写入长度，减少缓冲区溢出风险。
```

## 18. 本周常见错误

### 1. 拼写错误

例如：

```text
SERVRE_PORT
perro
tcp_clinet
```

正确：

```text
SERVER_PORT
perror
tcp_client
```

### 2. 大括号不匹配

错误：

```text
expected declaration or statement at end of input
```

原因：

```text
少了右大括号 }。
```

解决：

```text
检查 if / while / 函数的大括号是否成对。
```

### 3. 端口被占用

错误：

```text
bind failed: Address already in use
```

原因：

```text
之前的 server 或 nc 还在占用 8888 端口。
```

解决：

```text
关闭旧 server，或者按 Ctrl + C。
```

### 4. server 没启动

错误：

```text
connect failed: Connection refused
```

原因：

```text
client 连接的端口没有 server 在监听。
```

解决：

```text
先启动 server，再运行 client。
```

## 19. 面试重点

### 问：TCP client 的流程是什么？

答：

```text
socket -> connect -> send/recv -> close
```

### 问：TCP server 的流程是什么？

答：

```text
socket -> bind -> listen -> accept -> recv/send -> close
```

### 问：listenfd 和 connfd 的区别是什么？

答：

```text
listenfd 用于监听客户端连接。
connfd 是 accept 返回的通信 socket，用于和具体客户端收发数据。
```

### 问：如何让 server 连续服务多个 client？

答：

```text
在 accept 外面套外层 while。
当前 client 断开后 close(connfd)，然后继续 accept 下一个 client。
```

### 问：如何让 server 同时服务多个 client？

答：

```text
主线程负责 accept。
每接收一个 client，就创建一个子线程处理该 client 的 recv/send。
```

### 问：什么是应用层协议？

答：

```text
应用层协议是应用程序自己定义的数据格式和命令规则，用来规定客户端发送什么、服务器如何解析、服务器返回什么。
```

## 20. 和物联网网关项目的关系

第 2 周 TCP Socket 内容是后续网关网络通信的基础。

后续网关可能结构：

```text
线程1：读取 STM32 串口数据
线程2：保存日志
线程3：TCP server 等待上位机连接
线程4：处理 client 命令
线程5：MQTT 上传数据
```

TCP 协议命令可以扩展为：

```text
get_temp -> 获取温度
get_humi -> 获取湿度
get_all  -> 获取完整数据
led_on   -> 控制 STM32 LED
led_off  -> 控制 STM32 LED
```

最终目标：

```text
Linux 网关能接收 STM32 数据，并通过 TCP/MQTT 对外提供数据服务。
```

## 21. 第 2 周总结

第 2 周完成了 TCP Socket 网络编程基础。

已经掌握：

```text
1. TCP client 编写
2. TCP server 编写
3. TCP 双向收发
4. TCP echo 回显
5. 循环 server
6. 多线程 server
7. 简单应用层协议
```

这些内容是后续物联网网关项目的网络通信基础。
