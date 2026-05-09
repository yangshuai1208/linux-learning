# Day04 笔记：pthread 基础

## 1. 今日完成内容

今天完成了 Linux pthread 基础实验：

- 编写 `pthread_demo.c`
- 使用 `pthread_create()` 创建子线程
- 使用 `pthread_join()` 等待子线程结束
- 使用 `sleep()` 观察主线程和子线程交替运行
- 使用 `-pthread` 编译多线程程序

## 2. pthread 是什么

`pthread` 是 Linux 下常用的 POSIX 线程库。

线程可以理解为：

```text
同一个程序内部同时执行的多个任务流
```

在今天的程序中：

```text
main 函数所在的是主线程
thread_func 函数运行在子线程
```

## 3. 今日核心代码

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func(void *arg)
{
    int i = 0;

    for (i = 0; i < 5; i++)
    {
        printf("child thread running:%d\n", i);
        sleep(1);
    }

    return NULL;
}

int main(void)
{
    pthread_t tid;

    printf("main thread start\n");

    pthread_create(&tid, NULL, thread_func, NULL);

    for (int i = 0; i < 5; i++)
    {
        printf("main thread running:%d\n", i);
        sleep(1);
    }

    pthread_join(tid, NULL);

    printf("main thread end\n");

    return 0;
}
```

## 4. 头文件说明

### pthread.h

```c
#include <pthread.h>
```

用于使用 pthread 相关 API，例如：

```c
pthread_create();
pthread_join();
pthread_t;
```

### unistd.h

```c
#include <unistd.h>
```

用于使用 `sleep()` 函数。

## 5. pthread_t

```c
pthread_t tid;
```

`pthread_t` 用来保存线程 ID。

可以理解为：

```text
tid 是子线程的编号
```

注意：

```text
pthread_t 是类型
tid 是变量名
id 和 tid 不是同一个变量
```

今天遇到过这个错误：

```c
pthread_create(&id, NULL, thread_func, NULL);
```

错误原因：

```text
代码里定义的是 tid，不是 id
```

正确写法：

```c
pthread_create(&tid, NULL, thread_func, NULL);
```

## 6. pthread_create()

```c
pthread_create(&tid, NULL, thread_func, NULL);
```

作用：创建一个新线程。

参数解释：

```text
&tid          保存新线程 ID
NULL          使用默认线程属性
thread_func   子线程执行的函数
NULL          传给线程函数的参数
```

注意：

```text
&tid 前面的 & 不能少
thread_func 后面不要加括号
```

错误写法：

```c
pthread_create(&tid, NULL, thread_func(), NULL);
```

原因：

```text
thread_func() 表示直接调用函数
thread_func 表示把函数地址传给 pthread_create
```

正确写法：

```c
pthread_create(&tid, NULL, thread_func, NULL);
```

## 7. 线程函数格式

pthread 线程函数必须是这种格式：

```c
void *thread_func(void *arg)
```

含义：

```text
返回值类型：void *
参数类型：void *
```

不要写成：

```c
void thread_func(void)
```

因为 `pthread_create()` 要求线程函数类型匹配。

## 8. pthread_join()

```c
pthread_join(tid, NULL);
```

作用：等待子线程结束。

如果没有 `pthread_join()`，主线程可能提前结束，导致子线程还没运行完，整个程序就退出了。

## 9. sleep()

```c
sleep(1);
```

作用：让当前线程暂停 1 秒。

今天用它是为了更明显观察：

```text
主线程和子线程交替输出
```

## 10. 编译命令

```bash
gcc pthread_demo.c -o pthread_demo -pthread
```

注意：

```text
必须加 -pthread
```

如果不加，可能出现 pthread 相关链接错误。

## 11. 今日遇到的问题

### 问题 1：id undeclared

错误：

```text
'id' undeclared
```

原因：

```c
pthread_create(&id, NULL, thread_func, NULL);
```

但实际定义的是：

```c
pthread_t tid;
```

正确：

```c
pthread_create(&tid, NULL, thread_func, NULL);
```

### 问题 2：可执行文件名写错

错误现象：

```text
./pthread_demo: No such file or directory
```

原因可能是编译时写成了：

```bash
gcc pthread_demo.c -o pthread_dmeo -pthread
```

但是运行时执行：

```bash
./pthread_demo
```

`pthread_dmeo` 和 `pthread_demo` 不是同一个名字。

正确：

```bash
gcc pthread_demo.c -o pthread_demo -pthread
./pthread_demo
```

### 问题 3：多线程输出顺序不固定

多线程是并发执行的，所以输出可能不是完全固定顺序。

只要能看到：

```text
main thread running
child thread running
main thread end
```

说明线程基本运行正常。

## 12. 面试考点

### 问：Linux 下如何创建线程？

答：

```c
pthread_create(&tid, NULL, thread_func, NULL);
```

### 问：pthread_join 的作用是什么？

答：

```text
等待指定子线程结束，防止主线程提前退出。
```

### 问：编译 pthread 程序为什么要加 -pthread？

答：

```text
告诉 gcc 启用 pthread 支持，并链接 pthread 相关库。
```

### 问：线程函数的标准格式是什么？

答：

```c
void *thread_func(void *arg)
```

## 13. 和物联网网关项目的关系

后续网关项目会使用多线程结构：

```text
串口线程：负责读取 STM32 发来的数据
转发线程：负责 TCP / MQTT 发送数据
日志线程：负责写入日志文件
```

今天的 pthread 基础，是后续实现网关多线程架构的基础。

Day5 会继续学习互斥锁，用来解决多个线程同时访问共享变量导致的数据错误问题。
