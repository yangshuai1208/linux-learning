# Day06 笔记：Linux 串口 termios

## 1. 今日完成内容

今天完成了 Linux 串口通信基础实验：

- 使用 usbipd 将 CH340 USB 转串口挂载到 WSL2
- Ubuntu 中识别到 `/dev/ttyUSB0`
- 使用 `open()` 打开串口设备
- 使用 `termios` 配置串口参数
- 使用 `write()` 向串口发送数据
- 使用 `read()` 接收 STM32 USART1 发来的数据
- Linux 成功接收到 STM32 发送的字符串

最终接收到的数据：

```text
recv:temp=25,humi=60,device_id=stm32_01
```

## 2. WSL2 识别 USB 串口

Windows PowerShell 管理员中使用：

```powershell
usbipd list
```

可以看到 CH340：

```text
USB-SERIAL CH340 (COM4)
```

绑定设备：

```powershell
usbipd bind --busid 4-1
```

挂载到 WSL：

```powershell
usbipd attach --wsl --busid 4-1
```

注意：

```text
usbipd 和 bind 中间有空格
bind 和 --busid 中间有空格
--busid 和 4-1 中间有空格

usbipd 和 attach 中间有空格
attach 和 --wsl 中间有空格
--wsl 和 --busid 中间有空格
--busid 和 4-1 中间有空格
```

Ubuntu 中检查：

```bash
lsusb
```

可以看到：

```text
QinHeng Electronics CH340 serial converter
```

查看串口设备：

```bash
ls /dev/ttyUSB*
```

输出：

```text
/dev/ttyUSB0
```

## 3. 串口设备文件

Linux 中硬件设备通常以文件形式存在。

本次串口设备是：

```text
/dev/ttyUSB0
```

代码中定义为：

```c
#define SERIAL_DEV "/dev/ttyUSB0"
```

注意：

```text
USB 是大写
最后是数字 0，不是字母 O
路径前后必须有英文双引号
```

## 4. open()

```c
fd = open(SERIAL_DEV, O_RDWR | O_NOCTTY);
```

作用：打开串口设备。

参数说明：

```text
SERIAL_DEV：串口设备路径
O_RDWR：可读可写
O_NOCTTY：不把该串口作为控制终端
```

如果打开失败：

```c
perror("open serial failed");
```

常见原因：

```text
1. /dev/ttyUSB0 不存在
2. 没有 sudo 权限
3. 串口被 Windows 串口助手占用
4. usbipd 没有挂载成功
```

## 5. termios 是什么

`termios` 是 Linux 中配置终端和串口参数的标准接口。

本次用它配置：

```text
波特率：115200
数据位：8 位
校验位：无
停止位：1 位
工作模式：非规范模式
```

也就是：

```text
115200 8N1
```

## 6. tcgetattr()

```c
tcgetattr(fd, &options);
```

作用：获取当前串口配置。

如果失败，会输出：

```text
tcgetattr failed
```

今天之前在 `/dev/ttyS4` 上出现过：

```text
tcgetattr failed: Input/output error
```

原因是 WSL2 没有正确接管 Windows COM 串口。

解决方法是通过 usbipd 挂载 CH340，使其在 Ubuntu 中变成：

```text
/dev/ttyUSB0
```

## 7. cfsetispeed() 和 cfsetospeed()

```c
cfsetispeed(&options, B115200);
cfsetospeed(&options, B115200);
```

作用：

```text
cfsetispeed：设置输入波特率
cfsetospeed：设置输出波特率
```

注意：

```text
第二个必须是 cfsetospeed
不要两个都写成 cfsetispeed
```

## 8. 配置 8N1

### 8 位数据位

```c
options.c_cflag &= ~CSIZE;
options.c_cflag |= CS8;
```

### 无校验

```c
options.c_cflag &= ~PARENB;
```

### 1 位停止位

```c
options.c_cflag &= ~CSTOPB;
```

## 9. 非规范模式

```c
options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
options.c_oflag &= ~OPOST;
options.c_iflag &= ~(IXON | IXOFF | IXANY);
```

作用：

```text
关闭终端回显
关闭规范输入模式
关闭软件流控
让串口按原始数据方式收发
```

## 10. VMIN 和 VTIME

```c
options.c_cc[VMIN] = 0;
options.c_cc[VTIME] = 10;
```

含义：

```text
VMIN = 0：read 不要求必须读到指定字节数
VTIME = 10：等待超时时间，单位是 0.1 秒，所以 10 表示约 1 秒
```

## 11. tcsetattr()

```c
tcsetattr(fd, TCSANOW, &options);
```

作用：把配置立即应用到串口。

```text
TCSANOW 表示立即生效
```

## 12. write()

```c
ret = write(fd, send_buf, strlen(send_buf));
```

作用：向串口发送数据。

本次发送：

```text
hello stm32
```

## 13. read()

```c
ret = read(fd, recv_buf, sizeof(recv_buf) - 1);
```

作用：从串口读取数据。

如果 `ret > 0`，说明读到了数据：

```c
printf("recv:%s\n", recv_buf);
```

## 14. 今日遇到的问题

### 问题 1：文件名拼错

错误：

```text
serila_demo.c: No such file or directory
```

原因：

```text
serial 拼成了 serila
```

正确：

```bash
gcc serial_demo.c -o serial_demo
```

### 问题 2：普通用户权限不足

错误：

```text
open serial failed: Permission denied
```

解决：

```bash
sudo ./serial_demo
```

### 问题 3：/dev/ttyS4 能 open，但 tcgetattr 失败

现象：

```text
open serial success:/dev/ttyS4
tcgetattr failed: Input/output error
```

原因：

```text
/dev/ttyS4 不是 WSL2 中真正可用的 USB 串口设备
```

解决：

```text
使用 usbipd 将 CH340 挂载到 WSL，使其显示为 /dev/ttyUSB0
```

### 问题 4：/dev/ttyUSB0 消失

现象：

```text
ls /dev/ttyUSB*
No such file or directory
```

原因：

```text
烧录 STM32、插拔 USB、重启 WSL 后，USB 串口可能从 WSL 中断开
```

解决：

```powershell
usbipd attach --wsl --busid 4-1
```

## 15. 和网关项目的关系

Day6 是后续物联网网关最关键的基础之一。

后续网关数据链路：

```text
STM32 采集温湿度
STM32 USART1 发送字符串
Linux 网关通过 /dev/ttyUSB0 接收数据
Linux 解析字符串
Linux 写日志 / TCP 转发 / MQTT 发布
```

今天完成的是其中的第一段：

```text
STM32 → Linux 串口接收
```

## 16. 面试考点

### 问：Linux 下串口设备是什么形式？

答：

```text
Linux 下串口表现为设备文件，例如 /dev/ttyUSB0、/dev/ttyS0。
```

### 问：termios 用来做什么？

答：

```text
termios 用于配置 Linux 串口参数，例如波特率、数据位、校验位、停止位和读写模式。
```

### 问：115200 8N1 是什么意思？

答：

```text
波特率 115200，8 位数据位，无校验，1 位停止位。
```

### 问：open、read、write 在串口通信中分别做什么？

答：

```text
open 打开串口设备
read 从串口读取数据
write 向串口发送数据
```

### 问：为什么 WSL2 中要用 usbipd？

答：

```text
WSL2 默认不能直接访问 Windows USB 设备，需要通过 usbipd 将 USB 串口设备挂载到 WSL。
```
