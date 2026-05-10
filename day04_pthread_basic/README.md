# Day04 pthread 基础

## 一、今日目标

今天完成 Linux 多线程基础实验：

- 理解进程和线程的基本区别
- 学会使用 `pthread_create()` 创建子线程
- 学会使用 `pthread_join()` 等待子线程结束
- 观察主线程和子线程并发执行的现象

## 二、项目结构

```text
day04_pthread_basic/
├── pthread_demo.c
├── README.md
└── notes/
    └── day04_note.md
```

## 三、编译命令

```bash
gcc pthread_demo.c -o pthread_demo -pthread
```

注意：

```text
gcc 和 pthread_demo.c 中间有空格
pthread_demo.c 和 -o 中间有空格
-o 和 pthread_demo 中间有空格
pthread_demo 和 -pthread 中间有空格
```

`-pthread` 必须加，用于启用 pthread 支持并链接 pthread 相关库。

## 四、运行命令

```bash
./pthread_demo
```

## 五、实验现象

终端输出类似：

```text
main thread start
main thread running:0
child thread running:0
main thread running:1
child thread running:1
main thread running:2
child thread running:2
main thread running:3
child thread running:3
main thread running:4
child thread running:4
main thread end
```

说明主线程和子线程都在运行。

注意：多线程程序的输出顺序不一定每次完全一样，这是正常现象。

## 六、今日总结

今天完成了 pthread 基础实验，掌握了 Linux 下创建线程和等待线程结束的基本方法。

后续物联网网关项目中，会使用多线程实现：

```text
线程1：读取 STM32 串口数据
线程2：转发 TCP / MQTT 数据
线程3：写日志文件
```

因此 Day4 是后续网关多线程结构的基础。
