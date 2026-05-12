# Day07 第 1 周 Linux 基础总结

## 一、本周目标

第 1 周目标是打通嵌入式 Linux 应用开发基础，为后续物联网网关项目做准备。

本周完成内容：

```text
Day1：Ubuntu/WSL + gcc + hello_linux.c
Day2：多文件编译 + Makefile
Day3：gdb + 文件 I/O
Day4：pthread 基础
Day5：mutex 互斥锁
Day6：termios 串口通信
Day7：第 1 周总结
```

## 二、本周项目结构

```text
linux-learning-submit/
├── day01_hello_linux/
├── day02_makefile/
├── day03_file_io_gdb/
├── day04_pthread_basic/
├── day05_mutex/
├── day06_termios_serial/
└── day07_week01_summary/
```

## 三、核心知识串联

### 1. gcc

`gcc` 用于编译 C 语言程序。

示例：

```bash
gcc hello_linux.c -o hello_linux
```

在后续网关项目中，所有 Linux C 程序都需要通过 `gcc` 编译。

### 2. Makefile

Makefile 用来管理多文件工程编译。

示例：

```bash
make
make clean
```

后续网关项目会拆成多个模块：

```text
serial.c
parser.c
log.c
tcp_client.c
mqtt_client.c
main.c
```

所以必须使用 Makefile 管理编译。

### 3. gdb

`gdb` 用于调试 Linux C 程序。

常用命令：

```gdb
break main
run
next
print buffer
continue
quit
```

后续网关程序如果出现段错误、变量异常、串口数据解析错误，可以用 gdb 排查。

### 4. 文件 I/O

文件 I/O 用于读写本地文件。

核心函数：

```c
fopen();
fprintf();
fgets();
fclose();
```

后续网关日志模块会使用文件 I/O 保存运行日志。

### 5. pthread

`pthread` 用于创建 Linux 多线程程序。

核心函数：

```c
pthread_create();
pthread_join();
```

后续网关会使用多个线程分别处理串口、转发和日志。

### 6. mutex

互斥锁用于保护多线程共享数据。

核心函数：

```c
pthread_mutex_lock();
pthread_mutex_unlock();
```

后续网关多个线程访问同一份传感器数据时，需要互斥锁防止数据竞争。

### 7. termios

`termios` 用于配置 Linux 串口参数。

核心函数：

```c
tcgetattr();
cfsetispeed();
cfsetospeed();
tcsetattr();
read();
write();
```

Day6 已经实现 Linux 通过 `/dev/ttyUSB0` 接收 STM32 USART1 数据。

## 四、和物联网网关项目的关系

第 1 周内容最终会组成网关基础能力：

```text
gcc / Makefile：编译整个网关工程
gdb：调试网关程序
文件 I/O：保存日志
pthread：实现多线程架构
mutex：保护共享数据
termios：读取 STM32 串口数据
```

后续网关数据流：

```text
STM32 采集数据
↓
USART1 发送字符串
↓
Linux termios 读取串口
↓
解析温湿度数据
↓
写日志 / TCP 转发 / MQTT 发布
```

## 五、本周总结

第 1 周已经完成嵌入式 Linux 应用开发基础打通。

当前已经具备：

```text
1. 能在 Ubuntu/WSL 中编写和编译 C 程序
2. 能使用 Makefile 管理小型 C 工程
3. 能使用 gdb 基础调试
4. 能进行文件读写
5. 能创建线程
6. 能使用互斥锁保护共享变量
7. 能通过 termios 接收 STM32 串口数据
```

这些能力是后续物联网网关项目的基础。
