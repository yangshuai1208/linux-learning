# Day11 笔记：TCP Server 连续服务多个客户端

## 1. 今日完成内容

今天完成了 TCP server 连续服务多个客户端实验：

- 编写 `tcp_loop_server.c`
- 在 Day10 Echo server 基础上增加外层 `while(1)`
- 实现 server 持续 `accept()` 新客户端
- 实现当前 client 断开后，server 不退出
- 使用 Day10 的 `tcp_echo_client` 测试多次连接

最终实验现象：

```text
client 第一次连接成功
发送 hello day11
server 回显 hello day11
client 输入 quit 断开
server 继续等待
client 第二次仍然可以连接成功
```

说明 Day11 实验成功。

## 2. Day10 的问题

Day10 的 TCP Echo server 只能处理一个客户端。

大致流程是：

```text
socket()
bind()
listen()
accept()
recv/send
close()
return
```

问题：

```text
一个 client 断开后，server 程序也结束了。
```

真实 server 不应该这样，应该继续等待新的 client。

## 3. Day11 的改进

Day11 增加了外层循环：

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

核心：

```text
外层 while：持续等待新客户端
内层 while：持续处理当前客户端
```

## 4. 外层 while 的作用

```c
while (1)
{
    connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
}
```

作用：

```text
让 server 不断等待新的客户端连接。
```

如果没有外层 while：

```text
server 只 accept 一次，处理完一个 client 后就退出。
```

## 5. 内层 while 的作用

```c
while (1)
{
    ret = recv(connfd, recv_buf, sizeof(recv_buf) - 1, 0);
    send(connfd, recv_buf, strlen(recv_buf), 0);
}
```

作用：

```text
持续接收当前客户端发送的多条消息。
```

也就是：

```text
同一个 client 可以连续发送 hello1、hello2、hello3
server 都能接收并回显。
```

## 6. accept()

```c
connfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
```

作用：

```text
等待并接收一个客户端连接。
```

注意：

```text
accept 默认是阻塞函数。
没有 client 连接时，程序会停在 accept 这里等待。
```

返回值：

```text
成功：返回 connfd
失败：返回 -1
```

## 7. listenfd 和 connfd 的区别

```text
listenfd：监听 socket，只负责等待客户端连接。
connfd：通信 socket，负责和某一个客户端进行 recv/send。
```

这是高频面试点。

错误理解：

```text
用 listenfd 去 recv/send。
```

正确做法：

```text
accept 返回 connfd 后，用 connfd 和客户端通信。
```

## 8. recv() 返回值

```c
ret = recv(connfd, recv_buf, sizeof(recv_buf) - 1, 0);
```

返回值含义：

```text
ret > 0：接收到数据
ret == 0：客户端正常断开连接
ret < 0：接收失败
```

Day11 中：

```c
else if (ret == 0)
{
    printf("client disconnected\n");
    break;
}
```

这里的 `break` 只跳出内层 while。

随后执行：

```c
close(connfd);
```

然后回到外层 while，继续等待下一个 client。

## 9. close(connfd)

```c
close(connfd);
```

作用：

```text
关闭当前客户端连接。
```

注意：

```text
不能在每个 client 断开后关闭 listenfd。
```

如果关闭了 listenfd，server 就不能继续 accept 新客户端了。

## 10. 今日实验命令

进入 Day11 目录：

```bash
cd ~/linux-learning-submit/day11_tcp_loop_server
```

编译：

```bash
gcc tcp_loop_server.c -o tcp_loop_server
```

运行 server：

```bash
./tcp_loop_server
```

另开终端运行 Day10 client：

```bash
cd ~/linux-learning-submit/day10_tcp_echo
./tcp_echo_client
```

停止 server：

```text
Ctrl + C
```

退出 client：

```text
quit
```

## 11. 今日遇到的问题

### 问题 1：大括号不匹配

编译时出现：

```text
expected declaration or statement at end of input
```

原因：

```text
代码最后缺少右大括号，或者 while / if 的大括号数量不匹配。
```

解决：

```text
检查每一个 { 是否都有对应的 }。
```

### 问题 2：测试时 server 端输出粘连

例如：

```text
tcp loop server listening on port:8888...
```

这是 printf 格式问题，不影响核心实验。

可以优化为：

```c
printf("tcp loop server listening on port:%d...\n", SERVER_PORT);
```

## 12. 面试考点

### 问：如何让 TCP server 处理完一个 client 后不退出？

答：

```text
在 accept 外面增加外层 while 循环。
每个 client 断开后关闭 connfd，然后继续 accept 下一个 client。
```

### 问：外层 while 和内层 while 分别负责什么？

答：

```text
外层 while 负责持续接收新客户端连接。
内层 while 负责持续处理当前客户端的数据收发。
```

### 问：recv 返回 0 表示什么？

答：

```text
表示客户端正常关闭连接。
```

### 问：为什么不能关闭 listenfd？

答：

```text
listenfd 是监听 socket，关闭后 server 就不能继续等待新的 client。
每个客户端断开时只需要关闭 connfd。
```

## 13. 和物联网网关项目的关系

Day11 更接近真实服务器模型。

后续网关可能作为 TCP server：

```text
上位机 client 连接 Linux 网关
Linux 网关发送当前 STM32 传感器数据
client 断开
Linux 网关继续等待下一个 client
```

Day11 完成的是：

```text
server 持续运行、连续服务多个客户端的基础能力。
```
