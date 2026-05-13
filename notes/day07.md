# Day07 笔记：第 1 周总结

## 1. 本周知识主线

第 1 周不是单独学零散命令，而是在搭建一个 Linux 网关项目的基础能力。

主线关系：

```text
C 程序基础
↓
gcc 编译
↓
Makefile 管理多文件工程
↓
gdb 调试
↓
文件 I/O 写日志
↓
pthread 多线程
↓
mutex 保护共享数据
↓
termios 读取 STM32 串口数据
```

## 2. gcc

常用命令：

```bash
gcc xxx.c -o xxx
```

含义：

```text
gcc：调用编译器
xxx.c：源文件
-o xxx：指定输出的可执行文件名
```

注意：

```text
gcc 和 xxx.c 中间有空格
xxx.c 和 -o 中间有空格
-o 和 xxx 中间有空格
```

## 3. Makefile

Makefile 用来自动化编译。

常用命令：

```bash
make
make clean
```

注意：

```text
Makefile 的命令行前面必须是 Tab，不是空格
```

后续网关项目会有多个源文件，因此必须用 Makefile 管理。

## 4. gdb

调试版编译：

```bash
gcc -g file_io.c -o file_io
```

进入 gdb：

```bash
gdb ./file_io
```

常用命令：

```gdb
break main
run
next
print buffer
continue
quit
```

面试常问：

```text
gdb 如何打断点？break main
gdb 如何单步执行？next
gdb 如何查看变量？print 变量名
```

## 5. 文件 I/O

核心函数：

```c
fopen();
fprintf();
fgets();
fclose();
```

作用：

```text
fopen：打开文件
fprintf：向文件写入数据
fgets：从文件读取一行
fclose：关闭文件
```

后续用于网关日志模块。

## 6. pthread

核心代码：

```c
pthread_create(&tid, NULL, thread_func, NULL);
pthread_join(tid, NULL);
```

含义：

```text
pthread_create：创建子线程
pthread_join：等待子线程结束
```

编译时需要：

```bash
gcc pthread_demo.c -o pthread_demo -pthread
```

注意：

```text
-pthread 不要写错
```

## 7. mutex

核心代码：

```c
pthread_mutex_lock(&g_mutex);

g_counter++;

pthread_mutex_unlock(&g_mutex);
```

作用：

```text
保护共享变量，避免多个线程同时修改导致数据竞争。
```

本周实验中，不加锁时曾出现：

```text
final counter=181555
```

加锁后稳定：

```text
final counter=200000
```

## 8. termios

串口设备：

```text
/dev/ttyUSB0
```

核心函数：

```c
open();
tcgetattr();
cfsetispeed();
cfsetospeed();
tcsetattr();
read();
write();
close();
```

串口参数：

```text
115200 8N1
```

含义：

```text
115200：波特率
8：8 位数据位
N：无校验
1：1 位停止位
```

## 9. WSL2 串口问题

WSL2 默认不能直接访问 Windows USB 设备。

解决方法：

```powershell
usbipd list
usbipd bind --busid 4-1
usbipd attach --wsl --busid 4-1
```

Ubuntu 中确认：

```bash
lsusb
ls /dev/ttyUSB*
```

成功后看到：

```text
/dev/ttyUSB0
```

## 10. 第 1 周最终能力

完成第 1 周后，已经具备 Linux 网关项目基础能力：

```text
能编译
能组织工程
能调试
能写日志
能多线程
能保护共享变量
能读取 STM32 串口数据
```

## 11. 面试表达

可以这样说：

```text
我在第一周完成了嵌入式 Linux 应用开发基础训练，包括 gcc、Makefile、gdb、文件 I/O、pthread、mutex 和 termios。
其中 termios 实验已经实现 WSL2 Ubuntu 通过 /dev/ttyUSB0 接收 STM32F103C8T6 USART1 发送的温湿度字符串。
这些内容后续会整合成一个 Linux 物联网网关程序。
```
