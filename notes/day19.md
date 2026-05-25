# Day19 笔记：Linux 串口读取 STM32 数据

## 1. 今日完成内容

今天开始实现 Linux 读取 STM32 串口数据。

完成内容：

```text
创建 day19_serial_stm32 目录
编写 serial_reader.c
使用 open() 打开串口设备
使用 termios 配置串口参数
使用 read() 读取串口数据
修复代码编译错误
完成 serial_reader.c 编译
```

当前状态：

```text
代码编译成功
真实串口读取暂未测试成功
原因是 WSL 中暂未出现 /dev/ttyUSB0
```

## 2. 今日目标链路

目标链路是：

```text
STM32 USART1
↓
CH340 / USB 转串口
↓
WSL /dev/ttyUSB0
↓
Linux serial_reader.c
↓
read() 读取串口字符串
```

STM32 后续建议发送格式：

```text
temp=25,humi=60,device_id=stm32_01
```

最好带换行符：

```text
\n
```

方便 Linux 端分行显示和后续解析。

## 3. 串口设备文件

Linux 中串口设备也是文件。

常见设备名：

```text
/dev/ttyUSB0
/dev/ttyUSB1
/dev/ttyACM0
```

本次程序默认使用：

```c
#define SERIAL_DEV "/dev/ttyUSB0"
```

如果设备实际是 `/dev/ttyUSB1` 或 `/dev/ttyACM0`，需要修改这个宏定义。

## 4. open()

代码：

```c
fd = open(SERIAL_DEV, O_RDWR | O_NOCTTY);
```

作用：

```text
打开串口设备文件。
```

参数解释：

```text
SERIAL_DEV：串口设备路径，例如 /dev/ttyUSB0
O_RDWR：可读可写
O_NOCTTY：不把该串口作为控制终端
```

返回值：

```text
成功：返回文件描述符 fd
失败：返回 -1
```

本次运行报错：

```text
open serial failed: No such file or directory
```

原因：

```text
系统中不存在 /dev/ttyUSB0，说明 WSL 还没有识别到串口设备。
```

## 5. termios

`termios` 是 Linux 下配置串口参数的结构体和接口。

代码：

```c
struct termios options;
```

用于配置：

```text
波特率
数据位
停止位
校验位
输入输出模式
读取超时
```

## 6. tcgetattr()

代码：

```c
tcgetattr(fd, &options);
```

作用：

```text
读取当前串口配置。
```

如果失败，说明当前 fd 不是有效串口，或者串口状态异常。

## 7. cfsetispeed() 和 cfsetospeed()

代码：

```c
cfsetispeed(&options, B115200);
cfsetospeed(&options, B115200);
```

作用：

```text
设置输入波特率和输出波特率。
```

当前设置为：

```text
115200
```

STM32 端也必须保持一致：

```text
BaudRate: 115200
WordLength: 8 Bits
Parity: None
StopBits: 1
```

## 8. CLOCAL 和 CREAD

代码：

```c
options.c_cflag |= CLOCAL;
options.c_cflag |= CREAD;
```

作用：

```text
CLOCAL：忽略调制解调器控制线
CREAD：允许接收数据
```

简单理解：

```text
打开串口接收功能。
```

## 9. 数据位、停止位、校验位

代码：

```c
options.c_cflag &= ~PARENB;
options.c_cflag &= ~CSTOPB;
options.c_cflag &= ~CSIZE;
options.c_cflag |= CS8;
```

含义：

```text
~PARENB：无校验位
~CSTOPB：1 位停止位
~CSIZE + CS8：8 位数据位
```

也就是常见串口配置：

```text
115200 8N1
```

其中：

```text
8：8 位数据位
N：无校验
1：1 位停止位
```

## 10. 关闭规范模式和回显

代码：

```c
options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
```

作用：

```text
关闭规范模式
关闭回显
关闭擦除字符回显
关闭终端信号处理
```

解释：

```text
ICANON：规范模式，关闭后可以按字节读取
ECHO：关闭输入回显
ECHOE：关闭擦除回显
ISIG：关闭终端信号处理
```

今天遇到的错误：

```c
options.c_lflag &= ~(|CANON | ECHO | ECHOE | ISIG);
```

错误原因：

```text
~ 后面不能直接接 |
CANON 写错，正确是 ICANON
```

正确写法：

```c
options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
```

## 11. 关闭软件流控

代码：

```c
options.c_iflag &= ~(IXON | IXOFF | IXANY);
```

作用：

```text
关闭软件流控。
```

避免 XON/XOFF 特殊字符影响普通串口数据读取。

## 12. VMIN 和 VTIME

代码：

```c
options.c_cc[VMIN] = 0;
options.c_cc[VTIME] = 10;
```

作用：

```text
设置 read() 的读取行为。
```

含义：

```text
VMIN = 0：没有最小读取字节数要求
VTIME = 10：等待时间为 1 秒
```

注意：

```text
VTIME 单位是 0.1 秒
所以 10 表示 1 秒
```

## 13. tcsetattr()

代码：

```c
tcsetattr(fd, TCSANOW, &options);
```

作用：

```text
把新的串口配置立即应用到设备。
```

其中：

```text
TCSANOW：立即生效
```

## 14. read()

代码：

```c
ret = read(fd, recv_buf, sizeof(recv_buf) - 1);
```

作用：

```text
从串口读取 STM32 发来的数据。
```

返回值：

```text
ret > 0：读到数据
ret == 0：暂时没有数据
ret < 0：读取失败
```

当前程序逻辑：

```c
if (ret > 0)
{
    printf("recv from stm32: %s", recv_buf);
}
else if (ret == 0)
{
    continue;
}
else
{
    perror("read failed");
    break;
}
```

## 15. close()

代码：

```c
close(fd);
```

作用：

```text
关闭串口设备文件。
```

程序退出前应该关闭 fd，避免资源泄露。

## 16. 今日遇到的问题

### 问题 1：ICANON 写错

错误：

```c
options.c_lflag &= ~(|CANON | ECHO | ECHOE | ISIG);
```

正确：

```c
options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
```

原因：

```text
CANON 拼写错误，正确宏是 ICANON。
~ 后面要跟一个完整的括号表达式。
```

### 问题 2：找不到 /dev/ttyUSB0

错误：

```text
open serial failed: No such file or directory
```

原因：

```text
WSL 中没有 /dev/ttyUSB0。
串口设备还没有 attach 到 WSL。
```

解决思路：

```text
1. 在 Ubuntu 中执行 lsusb
2. 执行 ls /dev/ttyUSB*
3. 如果没有设备，在 Windows PowerShell 管理员中使用 usbipd attach
4. 回到 Ubuntu 再确认 /dev/ttyUSB0 是否出现
```

## 17. 今日命令总结

进入目录：

```bash
cd ~/linux-learning-submit/day19_serial_stm32
```

编辑代码：

```bash
nano -l serial_reader.c
```

编译：

```bash
gcc serial_reader.c -o serial_reader
```

运行：

```bash
sudo ./serial_reader
```

检查串口设备：

```bash
lsusb
ls /dev/ttyUSB*
ls /dev/ttyACM*
```

## 18. 下一步计划

下一步不是改代码，而是解决硬件链路：

```text
1. 确认 STM32 / CH340 插入电脑
2. Windows PowerShell 管理员执行 usbipd list
3. 找到 CH340 对应 BUSID
4. 执行 usbipd bind 和 usbipd attach
5. Ubuntu 中确认 /dev/ttyUSB0 出现
6. 再运行 sudo ./serial_reader
```

## 19. 面试考点

### 问：Linux 下串口设备是什么？

答：

```text
Linux 下串口设备也是文件，常见路径是 /dev/ttyUSB0、/dev/ttyACM0，程序可以通过 open/read/write 操作串口。
```

### 问：termios 的作用是什么？

答：

```text
termios 用于配置 Linux 串口参数，例如波特率、数据位、停止位、校验位、输入输出模式和读取超时。
```

### 问：115200 8N1 是什么意思？

答：

```text
115200 表示波特率，8 表示 8 位数据位，N 表示无校验，1 表示 1 位停止位。
```

### 问：read() 返回值如何判断？

答：

```text
ret > 0 表示读到数据，ret == 0 表示暂时没有数据，ret < 0 表示读取失败。
```

### 问：为什么当前程序打不开串口？

答：

```text
因为系统中不存在 /dev/ttyUSB0，说明 WSL 还没有识别或挂载 USB 转串口设备。
```

## 20. 和物联网网关项目的关系

Day17 和 Day18 完成了 MQTT：

```text
MQTT publish 上传数据
MQTT subscribe 接收命令
```

Day19 开始接入 STM32 串口数据：

```text
STM32 串口发送数据
↓
Linux 读取串口
```

后续完整网关流程：

```text
STM32 采集温湿度
↓
USART 发送字符串
↓
Linux serial_reader 读取
↓
解析 temp/humi/device_id
↓
生成 JSON
↓
MQTT publish 上传
```

## 21. 今日总结

今天完成了 Linux 串口读取程序的代码框架。

当前结果：

```text
代码编译成功
程序可以运行
但 /dev/ttyUSB0 不存在，因此暂未完成真实串口数据读取
```

最关键一句话：

```text
Day19 已完成 Linux 串口读取代码，下一步重点是把 CH340 串口设备正确挂载到 WSL。
```
