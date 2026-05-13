# Day06 Linux 串口 termios

## 一、今日目标

今天完成 Linux 串口通信基础实验：

- 理解 Linux 下串口设备文件
- 学会使用 `open()` 打开串口
- 学会使用 `termios` 配置串口参数
- 学会使用 `read()` 接收 STM32 串口数据
- 学会在 WSL2 中通过 usbipd 挂载 CH340 USB 转串口

## 二、项目结构

```text
day06_termios_serial/
├── serial_demo.c
├── README.md
└── notes/
    └── day06_note.md
```

## 三、实验环境

```text
开发环境：Windows + WSL2 Ubuntu 22.04
开发板：野火 STM32F103C8T6
串口芯片：CH340 USB 转串口
Linux 串口设备：/dev/ttyUSB0
STM32 串口：USART1
串口参数：115200 8N1
```

## 四、编译命令

```bash
gcc serial_demo.c -o serial_demo
```

注意：

```text
gcc 和 serial_demo.c 中间有空格
serial_demo.c 和 -o 中间有空格
-o 和 serial_demo 中间有空格
```

## 五、运行命令

```bash
sudo ./serial_demo
```

注意：

```text
sudo 和 ./serial_demo 中间有空格
```

普通用户直接运行可能会出现：

```text
open serial failed: Permission denied
```

所以当前使用 `sudo` 运行。

## 六、实验现象

WSL 中识别到 CH340：

```bash
lsusb
```

输出中可以看到：

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

运行程序后，终端输出：

```text
open serial success:/dev/ttyUSB0
send:hello stm32
waiting data...
recv:temp=25,humi=60,device_id=stm32_01
recv:temp=25,humi=60,device_id=stm32_01
recv:temp=25,humi=60,device_id=stm32_01
```

说明 Linux 端已经成功接收到 STM32 USART1 发送的数据。

## 七、今日总结

今天完成了 Linux 串口 termios 基础实验。

Linux 端通过 `open()` 打开 `/dev/ttyUSB0`，使用 `termios` 配置串口参数为 `115200 8N1`，然后通过 `read()` 循环接收 STM32 发来的字符串。

本实验是后续物联网网关项目的基础。后续网关程序会通过串口接收 STM32 发来的温湿度数据，再进行解析、日志保存、TCP 转发或 MQTT 发布。
