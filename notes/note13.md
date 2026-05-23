# Day13 笔记：TCP 简单应用层协议

## 1. 今日完成内容

今天完成了 TCP 简单应用层协议实验：

- 编写 `tcp_protocol_server.c`
- 基于 Day12 多线程 TCP server 继续扩展
- 增加 `handle_command()` 命令处理函数
- 实现不同命令返回不同内容
- 使用 Day10 的 `tcp_echo_client` 进行测试

最终实现效果：

```text
ping -> pong
get  -> temp=25,humi=60,device_id=stm32_01
help -> commands: ping, get, help
abc  -> unknown command
```

## 2. 什么是应用层协议

TCP 只负责可靠传输数据，但不会规定数据内容是什么意思。

应用层协议就是我们自己规定：

```text
客户端发什么
服务器怎么理解
服务器返回什么
```

今天定义的协议是：

```text
ping：测试连接
get：获取模拟传感器数据
help：查看支持的命令
其他命令：返回 unknown command
```

## 3. handle_command() 函数

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

作用：

```text
根据 client 发来的命令，生成 server 要回复的内容。
```

参数解释：

```text
cmd：客户端发来的命令
reply：服务器要返回的内容
reply_size：reply 缓冲区大小，防止写越界
```

## 4. strncmp()

示例：

```c
strncmp(cmd, "ping", 4)
```

作用：

```text
比较 cmd 前 4 个字符是否等于 ping。
```

为什么不用 `strcmp()`？

因为 client 使用 `fgets()` 输入时，字符串后面通常带有换行符：

```text
ping\n
```

如果用：

```c
strcmp(cmd, "ping")
```

可能匹配失败。

所以这里使用：

```c
strncmp(cmd, "ping", 4)
```

只比较前 4 个字符。

## 5. snprintf()

示例：

```c
snprintf(reply, reply_size, "pong\n");
```

作用：

```text
把要回复的内容写入 reply 缓冲区。
```

相比 `sprintf()`，`snprintf()` 更安全，因为它限制了最大写入长度。

## 6. 多线程处理 client

Day13 继续沿用 Day12 的多线程模型：

```text
主线程：accept 新客户端
子线程：处理某个 client 的 recv/send
```

主线程接收到 client 后：

```c
pthread_create(&tid, NULL, client_thread, pconnfd);
pthread_detach(tid);
```

作用：

```text
为每个客户端创建一个独立线程。
```

## 7. pthread_detach()

```c
pthread_detach(tid);
```

作用：

```text
让子线程结束后自动释放资源。
```

服务器程序中通常不会对每个 client 线程 `pthread_join()`，所以使用 `pthread_detach()` 更合适。

## 8. malloc 传递 connfd

代码中使用：

```c
int *pconnfd = malloc(sizeof(int));
*pconnfd = connfd;
pthread_create(&tid, NULL, client_thread, pconnfd);
```

原因：

```text
不能直接长期把 &connfd 传给线程。
因为 connfd 是主线程变量，下一次 accept 可能会修改它。
```

更安全的做法是：

```text
给每个线程单独分配一份 connfd。
```

在线程函数中：

```c
int connfd = *(int *)arg;
free(arg);
```

这样每个线程都有自己独立的 connfd。

## 9. recv() 和 send()

线程中接收数据：

```c
ret = recv(connfd, recv_buf, sizeof(recv_buf) - 1, 0);
```

根据命令生成回复：

```c
handle_command(recv_buf, send_buf, sizeof(send_buf));
```

发送回复：

```c
send(connfd, send_buf, strlen(send_buf), 0);
```

流程是：

```text
recv 收到命令
handle_command 解析命令
send 返回结果
```

## 10. 今日遇到的问题

### 问题 1：perror 参数忘记加双引号

错误写法：

```c
perror(listen failed);
```

正确写法：

```c
perror("listen failed");
```

说明：

```text
perror 的参数必须是字符串。
字符串要用英文双引号包起来。
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

解决方法：

```text
检查每一个 { 是否都有对应的 }。
```

### 问题 3：编译多线程程序要加 -pthread

正确编译命令：

```bash
gcc tcp_protocol_server.c -o tcp_protocol_server -pthread
```

注意：

```text
-pthread 不能漏
```

## 11. 今日测试命令

进入 Day13 目录：

```bash
cd ~/linux-learning-submit/day13_tcp_protocol
```

编译：

```bash
gcc tcp_protocol_server.c -o tcp_protocol_server -pthread
```

运行 server：

```bash
./tcp_protocol_server
```

另开终端运行 Day10 client：

```bash
cd ~/linux-learning-submit/day10_tcp_echo
./tcp_echo_client
```

测试命令：

```text
ping
get
help
abc
quit
```

## 12. 面试考点

### 问：什么是应用层协议？

答：

```text
应用层协议是应用程序自己约定的数据格式和命令规则，用来规定客户端发送什么、服务器如何解析、服务器返回什么。
```

### 问：为什么 TCP 还需要应用层协议？

答：

```text
TCP 只负责可靠传输字节流，不关心数据含义。应用层协议负责定义这些字节代表什么。
```

### 问：为什么这里用 strncmp 而不是 strcmp？

答：

```text
因为 fgets 读取的字符串可能带有换行符，例如 ping\n。
strncmp 只比较前几个有效字符，更适合简单命令判断。
```

### 问：snprintf 比 sprintf 好在哪里？

答：

```text
snprintf 可以限制最大写入长度，避免缓冲区溢出风险。
```

## 13. 和物联网网关项目的关系

Day13 是后续网关“命令控制”的基础。

现在是模拟协议：

```text
get -> temp=25,humi=60,device_id=stm32_01
```

后续可以升级为真实网关协议：

```text
get_temp  -> 返回 STM32 实时温度
get_humi  -> 返回 STM32 实时湿度
get_all   -> 返回完整传感器数据
led_on    -> 控制 STM32 LED 打开
led_off   -> 控制 STM32 LED 关闭
```

最终网关数据流：

```text
上位机 TCP client
↓
发送 get 命令
↓
Linux 网关 TCP server
↓
读取或返回 STM32 最新数据
↓
回复给上位机
```

Day13 完成的是 TCP 应用层命令协议的基础能力。
