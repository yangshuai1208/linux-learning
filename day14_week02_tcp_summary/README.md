# Day14 第 2 周 TCP Socket 总结

## 一、本周目标

第 2 周主要学习 Linux TCP Socket 网络编程，为后续物联网网关项目中的网络通信模块打基础。

本周完成内容：

```text
Day8：TCP client
Day9：TCP server
Day10：TCP echo 回显程序
Day11：TCP server 连续服务多个客户端
Day12：多线程 TCP server
Day13：TCP 简单应用层协议
Day14：第 2 周总结
```

## 二、本周知识主线

第 2 周的学习路线是：

```text
能连接
↓
能收发
↓
能回显
↓
能连续服务多个客户端
↓
能多线程并发处理客户端
↓
能根据命令返回不同结果
```

## 三、TCP Client 流程

TCP client 的基本流程：

```text
socket()
connect()
send()
recv()
close()
```

client 是主动连接 server 的一方。

核心作用：

```text
socket：创建 TCP socket
connect：连接服务器
send：发送数据
recv：接收服务器回复
close：关闭 socket
```

## 四、TCP Server 流程

TCP server 的基本流程：

```text
socket()
bind()
listen()
accept()
recv()
send()
close()
```

server 是监听端口、等待 client 连接的一方。

核心作用：

```text
socket：创建监听 socket
bind：绑定 IP 和端口
listen：进入监听状态
accept：接收客户端连接
recv：接收客户端数据
send：向客户端发送数据
close：关闭 socket
```

## 五、listenfd 和 connfd

这是 TCP server 中的重要概念：

```text
listenfd：监听 socket，只负责等待客户端连接
connfd：通信 socket，负责和某一个客户端进行 recv/send
```

一句话理解：

```text
listenfd 负责接客，connfd 负责服务客人。
```

## 六、Day10 TCP Echo

TCP Echo 的含义：

```text
client 发什么
server 就原样返回什么
```

核心代码逻辑：

```text
server recv 数据
server send 同一份数据
client 接收回显结果
```

## 七、Day11 循环 server

Day11 解决的问题：

```text
Day10 server 只能服务一个 client，client 断开后 server 就退出。
```

Day11 改进为：

```text
外层 while：不断 accept 新客户端
内层 while：持续处理当前客户端数据
```

核心结构：

```text
while (1)
{
    accept();

    while (1)
    {
        recv();
        send();
    }

    close(connfd);
}
```

## 八、Day12 多线程 server

Day12 解决的问题：

```text
Day11 虽然能连续服务多个 client，但一次主要处理一个 client。
```

Day12 改进为：

```text
主线程负责 accept 新客户端
子线程负责处理具体客户端的 recv/send
```

核心结构：

```text
while (1)
{
    accept();
    pthread_create();
}
```

编译时需要：

```bash
gcc tcp_thread_server.c -o tcp_thread_server -pthread
```

## 九、Day13 应用层协议

TCP 只负责传输字节流，不理解数据含义。

应用层协议由应用程序自己规定：

```text
client 发什么命令
server 如何解析
server 返回什么结果
```

Day13 定义的简单协议：

```text
ping -> pong
get  -> temp=25,humi=60,device_id=stm32_01
help -> commands: ping, get, help
其他命令 -> unknown command
```

## 十、和物联网网关项目的关系

第 2 周内容最终服务于物联网网关项目。

后续网关通信链路：

```text
STM32 采集传感器数据
↓
串口发送给 Linux
↓
Linux 通过 termios 读取数据
↓
Linux 通过 TCP server 等待上位机连接
↓
上位机发送 get 命令
↓
Linux 返回传感器数据
```

也可以是：

```text
STM32 采集数据
↓
Linux 读取串口
↓
Linux 作为 TCP client 主动上传到服务器
```

## 十一、本周总结

第 2 周已经完成 Linux TCP Socket 网络编程基础。

当前已经具备：

```text
1. 能编写 TCP client
2. 能编写 TCP server
3. 能实现 TCP 双向收发
4. 能实现 TCP 回显程序
5. 能让 server 连续服务多个客户端
6. 能使用多线程处理多个客户端
7. 能设计简单应用层协议
```

这些能力是后续 Linux 物联网网关网络通信模块的基础。
