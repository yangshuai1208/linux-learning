# Day19 Linux 串口读取 STM32 数据

## 一、今日目标

今天开始实现 Linux 网关读取 STM32 串口数据。

目标链路：

```text
STM32 USART1 发送数据
↓
USB 转串口 / CH340
↓
WSL 中识别为 /dev/ttyUSB0
↓
Linux C 程序打开串口
↓
read() 读取并打印 STM32 数据
```

## 二、项目结构

```text
day19_serial_stm32/
├── serial_reader.c
├── README.md
└── notes/
    └── day19_note.md
```

## 三、今日完成情况

已完成：

```text
1. 创建 day19_serial_stm32 目录
2. 编写 serial_reader.c
3. 使用 open() 打开串口设备
4. 使用 termios 配置串口参数
5. 使用 read() 读取串口数据
6. 修复 ICANON 拼写和位运算错误
7. serial_reader.c 编译成功
```

当前未完成：

```text
真实 STM32 串口数据读取测试
```

原因：

```text
当前 WSL 中暂未识别 /dev/ttyUSB0
运行程序时报错：open serial failed: No such file or directory
```

下一步：

```text
将 CH340 / USB 转串口设备重新 attach 到 WSL
确认 /dev/ttyUSB0 出现
再运行 sudo ./serial_reader 测试 STM32 数据读取
```

## 四、编译命令

```bash
gcc serial_reader.c -o serial_reader
```

注意：

```text
gcc 和 serial_reader.c 中间有空格
serial_reader.c 和 -o 中间有空格
-o 和 serial_reader 中间有空格
```

## 五、运行命令

```bash
sudo ./serial_reader
```

如果串口设备存在，正常应该看到：

```text
open serial success: /dev/ttyUSB0
serial init success, waiting stm32 data...
recv from stm32: temp=25,humi=60,device_id=stm32_01
```

## 六、当前实验现象

当前运行结果：

```text
open serial failed: No such file or directory
```

说明：

```text
程序已经正常执行到 open()，但系统中不存在 /dev/ttyUSB0。
这不是 C 代码逻辑错误，而是 WSL 串口设备还没有挂载成功。
```

## 七、下一步测试步骤

检查串口设备：

```bash
lsusb
ls /dev/ttyUSB*
ls /dev/ttyACM*
```

如果没有 `/dev/ttyUSB0`，需要在 Windows PowerShell 管理员中执行：

```powershell
usbipd list
usbipd bind --busid 对应BUSID
usbipd attach --wsl --busid 对应BUSID
```

回到 Ubuntu 后再次检查：

```bash
lsusb
ls /dev/ttyUSB*
```

如果出现：

```text
/dev/ttyUSB0
```

再运行：

```bash
sudo ./serial_reader
```

## 八、今日总结

Day19 主要完成了 Linux 串口读取程序的代码框架。

当前完成的是：

```text
Linux 串口读取程序编写
termios 串口参数配置
编译通过
问题定位到 WSL 串口设备未挂载
```

Day19 是后续物联网网关串口输入模块的基础。

后续目标：

```text
STM32 串口数据
↓
Linux read() 读取
↓
解析 temp/humi/device_id
↓
MQTT publish 上传
```
