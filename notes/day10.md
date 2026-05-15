# Day10 笔记：TCP Echo 回显程序

## 1. 今日完成内容

今天完成了 TCP 回显程序：

- 编写 `tcp_echo_server.c`
- 编写 `tcp_echo_client.c`
- server 监听 8888 端口
- client 连接 server
- client 从键盘输入字符串
- client 使用 `send()` 发送数据
- server 使用 `recv()` 接收数据
- server 使用 `send()` 原样返回数据
- client 使用 `recv()` 接收 server 回显数据

最终实验现象：

```text
input message:hello linux
echo from server:hello linux
```

说明 TCP Echo 回显程序成功。

## 2. TCP Echo 的核心思想

TCP Echo 的意思是：

```text
客户端发送什么，服务器就返回什么。
```

例如：

```text
client 发送：hello linux
server 接收：hello linux
server 返回：hello linux
client 接收：hello linux
```

这比 Day8 和 Day9 更进一步：

```text
Day8：只写 TCP client
Day9：只写 TCP server
Day10：client 和 server 持续交互
```

## 3. server 端核心流程

```text
socket()
bind()
listen()
accept()
while (1)
{
    recv()
    send()
}
close()
```

关键点：

```text
listenfd 用于监听连接
connfd 用于和客户端真正通信
```

## 4. client 端核心流程

```text
socket()
connect()
while (1)
{
    fgets()
    send()
    recv()
}
close()
```

client 端通过 `fgets()` 从键盘读取输入，然后发送给 server。

## 5. server 端 recv()

```c
ret = recv(connfd, recv_buf, sizeof(recv_buf) - 1, 0);
```

作用：

```text
从客户端接收数据。
```

参数解释：

```text
connfd：和客户端通信的 socket
recv_buf：接收缓冲区
sizeof(recv_buf) - 1：最多接收 127 字节，预留一个 '\0'
0：默认接收方式
```

返回值：

```text
ret > 0：接收到数据
ret == 0：客户端正常关闭连接
ret < 0：接收失败
```

## 6. server 端 send()

```c
send(connfd, recv_buf, strlen(recv_buf), 0);
```

作用：

```text
把刚收到的数据原样发回给客户端。
```

这就是回显程序的核心。

## 7. client 端 fgets()

```c
fgets(send_buf, sizeof(send_buf), stdin);
```

作用：

```text
从键盘读取一行输入。
```

例如输入：

```text
hello linux
```

会保存到 `send_buf` 中。

注意：

```text
fgets 会把回车换行符一起读进去。
```

## 8. client 端 quit 退出

```c
if (strncmp(send_buf, "quit", 4) == 0)
{
    break;
}
```

作用：

```text
如果用户输入 quit，就退出循环并关闭 socket。
```

## 9. 今日遇到的问题

### 问题 1：recv 使用了错误的 socket

错误现象：

```text
recv failed: Socket operation on non-socket
```

原因：

```text
recv() 的第一个参数不是 connfd。
```

正确写法：

```c
ret = recv(connfd, recv_buf, sizeof(recv_buf) - 1, 0);
```

注意：

```text
listenfd 只负责监听
connfd 才负责通信
```

### 问题 2：perror 拼写错误

错误写法：

```c
perro("accept failed");
```

正确写法：

```c
perror("accept failed");
```

### 问题 3：socket 判断条件写错

错误现象：

```text
socket failed: Success
```

原因：

```text
socket 创建成功时返回值 >= 0，只有 < 0 才表示失败。
```

正确写法：

```c
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0)
{
    perror("socket failed");
    return 1;
}
```

## 10. 今日测试命令

编译 server：

```bash
gcc tcp_echo_server.c -o tcp_echo_server
```

编译 client：

```bash
gcc tcp_echo_client.c -o tcp_echo_client
```

运行 server：

```bash
./tcp_echo_server
```

运行 client：

```bash
./tcp_echo_client
```

退出 client：

```text
quit
```

停止 server：

```text
Ctrl + C
```

## 11. 面试考点

### 问：TCP Echo 程序是什么？

答：

```text
TCP Echo 程序是客户端发送数据后，服务器原样返回该数据的程序。
```

### 问：server 为什么要使用 while 循环？

答：

```text
为了持续接收同一个客户端发送的多条消息。
```

### 问：recv 返回 0 表示什么？

答：

```text
表示对端正常关闭连接。
```

### 问：listenfd 和 connfd 的区别是什么？

答：

```text
listenfd 用于监听客户端连接。
connfd 用于和某一个客户端实际通信。
```

### 问：为什么 recv 和 send 要使用 connfd？

答：

```text
因为 accept 成功后返回的 connfd 才是和客户端通信的 socket。
```

## 12. 和物联网网关项目的关系

Day10 是后续网关 TCP 通信的基础。

后续网关可能实现：

```text
STM32 通过串口发送温湿度数据
Linux 通过 termios 读取串口
Linux 通过 TCP 把数据发给服务器
服务器回复 OK
Linux 根据服务器回复继续处理
```

Day10 的 Echo 程序可以理解为后续 TCP 数据交互的最小模型。
