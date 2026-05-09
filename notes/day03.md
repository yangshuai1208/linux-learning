# Day03 笔记：文件 I/O 与 gdb 调试

## 1. 今日完成内容

今天完成了：

- 使用 `fopen()` 打开文件
- 使用 `fprintf()` 向文件写入数据
- 使用 `fgets()` 从文件读取数据
- 使用 `fclose()` 关闭文件
- 使用 `gcc -g` 编译带调试信息的程序
- 使用 gdb 进行断点调试、单步执行、查看变量

## 2. 文件 I/O 核心函数

### fopen()

```c
fp = fopen("sensor_log.txt", "w");
```

作用：打开文件。

常用模式：

```text
"r"：只读，文件必须存在
"w"：写入，文件不存在则创建，文件存在则清空
"a"：追加，文件不存在则创建，文件存在则写到末尾
```

注意：

```text
sensor_log.txt 文件名前后必须一致
.txt 不要写成 .text
"r" 和 "w" 都是小写
```

### fprintf()

```c
fprintf(fp, "device_id=stm32_01,temp=25,humi=60\n");
```

作用：向文件写入格式化数据。

和 `printf()` 的区别：

```text
printf() 输出到终端
fprintf() 输出到文件
```

### fgets()

```c
fgets(buffer, sizeof(buffer), fp);
```

作用：从文件中读取一行内容到 `buffer`。

注意：

```text
fgets 执行之前，buffer 还是空的
gdb 中要先 next 执行完 fgets，再 print buffer
```

### fclose()

```c
fclose(fp);
```

作用：关闭文件。

文件打开后必须关闭，否则可能出现数据未写入或资源泄漏。

## 3. 今日代码核心流程

```text
打开 sensor_log.txt
写入一行模拟 STM32 传感器数据
关闭文件
重新以只读方式打开文件
读取一行内容到 buffer
打印 buffer
关闭文件
```

## 4. gdb 基础命令

### 编译时加 -g

```bash
gcc -g file_io.c -o file_io
```

`-g` 的作用是加入调试信息，让 gdb 可以看到源码、行号和变量。

### 进入 gdb

```bash
gdb ./file_io
```

注意：

```text
gdb 调试的是可执行文件 file_io
不是 file_io.c
```

### 常用 gdb 命令

```gdb
break main
run
next
print buffer
continue
quit
```

含义：

```text
break main     在 main 函数打断点
run            运行程序
next           单步执行，不进入函数内部
print buffer   查看 buffer 变量
continue       继续运行
quit           退出 gdb
```

## 5. 今日遇到的问题

### 问题 1：读文件失败

错误现象：

```text
open file for read failed
```

原因：

```c
fp = fopen("sensor_log.text", "r");
```

把 `.txt` 写成了 `.text`。

正确写法：

```c
fp = fopen("sensor_log.txt", "r");
```

### 问题 2：print buffer 看到全是 0

现象：

```text
'\000' <repeats 127 times>
```

原因：

```text
fgets(buffer, sizeof(buffer), fp);
```

这一行还没有真正执行，buffer 还没有读入文件内容。

正确操作：

```gdb
next
print buffer
```

必须先 `next` 执行完 `fgets()`，再查看 `buffer`。

### 问题 3：退出 gdb 后输入 break main 报错

原因：

```text
break main 是 gdb 命令
只能在 (gdb) 环境里输入
不能在普通 Linux 终端输入
```

正确流程：

```bash
gdb ./file_io
```

看到：

```text
(gdb)
```

之后再输入：

```gdb
break main
run
```

## 6. 面试考点

### 问：fopen 的 r、w、a 有什么区别？

答：

```text
r：只读，文件必须存在
w：写入，文件不存在则创建，存在则清空
a：追加，文件不存在则创建，存在则写到文件末尾
```

### 问：gdb 如何打断点？

答：

```gdb
break main
```

### 问：gdb 如何单步执行？

答：

```gdb
next
```

### 问：gdb 如何查看变量？

答：

```gdb
print buffer
```

### 问：为什么调试时要加 -g？

答：

```text
-g 会生成调试信息，使 gdb 能够查看源码、行号和变量。
```

## 7. 和网关项目的关系

Day3 的文件 I/O 后面会用于网关日志模块。

后续网关运行时，需要记录：

```text
串口收到的数据
解析后的温湿度
TCP 转发状态
MQTT 发布状态
错误信息
```

这些信息都可以通过文件 I/O 写入日志文件。

gdb 后面用于调试：

```text
串口读取异常
字符串解析错误
线程死锁
程序崩溃
指针问题
```

因此 Day3 是后续网关项目调试能力和日志能力的基础。
