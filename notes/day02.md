# Day02 笔记：多文件编译与 Makefile

## 1. 为什么要多文件编译

Day1 只有一个文件：

```text
hello_linux.c
```

所以可以直接编译：

```bash
gcc hello_linux.c -o hello_linux
```

但真实项目不会只有一个文件。后续网关项目会拆成多个模块，例如：

```text
main.c
serial.c
tcp_client.c
mqtt_client.c
log.c
```

所以今天开始学习多文件编译。

## 2. .h 文件和 .c 文件的区别

### .h 文件

`.h` 文件一般放：

```text
函数声明
宏定义
结构体声明
类型定义
```

例如：

```c
void print_welcome(void);
int add(int a, int b);
```

它的作用是告诉其他 `.c` 文件：

```text
这些函数存在，可以调用。
```

### .c 文件

`.c` 文件一般放函数的具体实现。

例如：

```c
int add(int a, int b)
{
    return a + b;
}
```

## 3. 本次代码结构

```text
main.c      主函数
utils.h     函数声明
utils.c     函数实现
Makefile    自动化编译规则
```

## 4. 手动编译

命令：

```bash
gcc main.c utils.c -o app
```

注意：

```text
不能只编译 main.c
因为 add() 和 print_welcome() 的实现写在 utils.c 里
```

如果只执行：

```bash
gcc main.c -o app
```

可能会出现链接错误：

```text
undefined reference to add
undefined reference to print_welcome
```

原因是编译器找不到函数实现。

## 5. Makefile 基础

本次 Makefile 内容：

```makefile
CC = gcc
TARGET = app
SRCS = main.c utils.c

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
```

注意：

```text
$(CC) 那一行前面必须是 Tab 键，不是空格
rm -f 那一行前面也必须是 Tab 键，不是空格
$(CC) 和 $(SRCS) 中间必须有空格
$(SRCS) 和 -o 中间必须有空格
-o 和 $(TARGET) 中间必须有空格
```

## 6. make 和 make clean

编译：

```bash
make
```

运行：

```bash
./app
```

清理生成文件：

```bash
make clean
```

`make clean` 会删除编译生成的可执行文件 `app`。

## 7. 今日遇到的问题

### 问题 1：utils.h 找不到

报错：

```text
fatal error: utils.h: No such file or directory
```

原因：

```text
文件名写错，例如把 utils 写成 utlis
```

解决：

```bash
mv utlis.h utils.h
mv utlis.c utils.c
```

### 问题 2：return 拼写错误

错误写法：

```c
retunr a + b;
```

正确写法：

```c
return a + b;
```

### 问题 3：Makefile 少空格

错误现象：

```text
gccmain.c: No such file or directory
```

原因是 Makefile 中写成了：

```makefile
$(CC)$(SRCS) -o $(TARGET)
```

正确写法：

```makefile
$(CC) $(SRCS) -o $(TARGET)
```

`$(CC)` 和 `$(SRCS)` 中间必须有空格。

## 8. 面试考点

### 问：.h 文件和 .c 文件分别放什么？

答：

```text
.h 文件放函数声明、宏定义、结构体声明。
.c 文件放函数的具体实现。
```

### 问：Makefile 有什么作用？

答：

```text
Makefile 用来管理编译规则，实现自动化编译，避免每次手动输入复杂 gcc 命令。
```

### 问：为什么多文件工程不能只编译 main.c？

答：

```text
因为 main.c 中调用的函数可能定义在其他 .c 文件中。
如果不一起编译，链接阶段会找不到函数实现。
```

## 9. 和网关项目的关系

后续物联网网关项目会拆分成多个模块：

```text
main.c          主程序
serial.c        串口读取 STM32 数据
tcp_client.c    TCP 转发
mqtt_client.c   MQTT 发布
log.c           日志模块
```

这些模块最后都需要通过 Makefile 统一编译。

因此 Day2 的 Makefile 是后续网关项目工程化的基础。
