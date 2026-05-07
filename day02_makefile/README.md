# Day02 多文件编译与 Makefile

## 一、今日目标

今天完成 Linux C 多文件工程的基础练习：

- 学会将 C 程序拆分为多个文件
- 理解 `.c` 文件和 `.h` 文件的作用
- 使用 `gcc` 手动编译多个源文件
- 编写 Makefile，实现一键编译和清理

## 二、项目结构

```text
day02_makefile/
├── main.c
├── utils.c
├── utils.h
├── Makefile
└── README.md
```

## 三、文件说明

### 1. main.c

主程序入口，负责调用工具函数。

### 2. utils.h

头文件，存放函数声明。

```c
void print_welcome(void);
int add(int a, int b);
```

### 3. utils.c

源文件，存放函数具体实现。

### 4. Makefile

用于自动化编译，避免每次手动输入完整 gcc 命令。

## 四、手动编译命令

```bash
gcc main.c utils.c -o app
```

注意空格：

```text
gcc 和 main.c 中间有空格
main.c 和 utils.c 中间有空格
utils.c 和 -o 中间有空格
-o 和 app 中间有空格
```

## 五、运行命令

```bash
./app
```

## 六、实验现象

终端输出：

```text
Welcome to Linux Gateway Day02!
10+20=30
```

## 七、Makefile 编译

编译：

```bash
make
```

运行：

```bash
./app
```

清理：

```bash
make clean
```

## 八、今日总结

今天完成了 Linux C 多文件工程的基本练习，理解了 `.h` 文件放声明、`.c` 文件放实现，并使用 Makefile 实现了一键编译。

这个知识点是后续物联网网关项目的基础。后面项目中会出现多个模块，例如：

```text
serial.c
tcp_client.c
mqtt_client.c
log.c
main.c
```

这些文件都需要通过 Makefile 统一管理和编译。
