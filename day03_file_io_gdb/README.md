# Day03 文件 I/O 与 gdb 调试

## 一、今日目标

今天完成两个核心任务：

- 学习 Linux C 文件读写
- 学习使用 gdb 进行断点调试、单步执行、查看变量

## 二、项目文件

```text
day03_file_io_gdb/
├── file_io.c
├── README.md
└── notes/
    └── day03_note.md
```

## 三、编译命令

```bash
gcc file_io.c -o file_io
```

带调试信息编译：

```bash
gcc -g file_io.c -o file_io
```

注意：

```text
gcc 和 -g 中间有空格
-g 和 file_io.c 中间有空格
file_io.c 和 -o 中间有空格
-o 和 file_io 中间有空格
```

## 四、运行命令

```bash
./file_io
```

## 五、实验现象

程序运行后，会创建 `sensor_log.txt` 文件，并写入一行模拟传感器数据：

```text
device_id=stm32_01,temp=25,humi=60
```

终端输出：

```text
read from file:device_id=stm32_01,temp=25,humi=60
```

## 六、gdb 调试命令

```bash
gdb ./file_io
```

进入 gdb 后使用：

```gdb
break main
run
next
print buffer
continue
quit
```

## 七、今日总结

今天完成了 Linux C 文件 I/O 和 gdb 基础调试。

文件 I/O 是后续日志模块的基础，后面网关程序会把串口接收到的数据写入日志文件。

gdb 是后续排查程序崩溃、变量异常、解析错误的重要工具。
