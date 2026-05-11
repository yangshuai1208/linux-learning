# Day05 笔记：pthread 互斥锁 mutex

## 1. 今日完成内容

今天完成了 Linux pthread 互斥锁基础实验：

- 创建两个线程
- 两个线程同时修改全局变量 `g_counter`
- 使用互斥锁保护 `g_counter++`
- 对比加锁和不加锁时的运行结果
- 理解多线程数据竞争问题

## 2. 为什么需要互斥锁

今天的共享变量是：

```c
int g_counter = 0;
```

两个线程都会执行：

```c
g_counter++;
```

理论上：

```text
线程1 加 100000 次
线程2 加 100000 次
最终应该是 200000
```

但是如果没有互斥锁，运行结果可能小于 200000，例如：

```text
final counter=181555
```

这说明多个线程同时修改共享变量时发生了数据竞争。

## 3. g_counter++ 为什么不安全

`g_counter++` 看起来是一行代码，但底层不是一步完成，通常可以理解为：

```text
1. 读取 g_counter 的值
2. 对值加 1
3. 把新值写回 g_counter
```

如果两个线程同时执行，可能出现：

```text
线程1 读取到 100
线程2 也读取到 100
线程1 写回 101
线程2 也写回 101
```

本来应该加两次变成 102，结果只变成了 101。

这就是数据竞争。

## 4. pthread_mutex_t

```c
pthread_mutex_t g_mutex;
```

作用：定义一个互斥锁变量。

可以理解为：

```text
g_mutex 是保护共享变量 g_counter 的锁
```

## 5. pthread_mutex_init()

```c
pthread_mutex_init(&g_mutex, NULL);
```

作用：初始化互斥锁。

参数说明：

```text
&g_mutex    要初始化的互斥锁
NULL        使用默认属性
```

注意：

```text
&g_mutex 前面的 & 不能丢
```

## 6. pthread_mutex_lock()

```c
pthread_mutex_lock(&g_mutex);
```

作用：加锁。

加锁后，其他线程如果也想进入同一段被锁保护的代码，就必须等待。

## 7. pthread_mutex_unlock()

```c
pthread_mutex_unlock(&g_mutex);
```

作用：解锁。

注意：

```text
lock 和 unlock 必须成对出现
加锁后一定要解锁
否则可能导致其他线程一直等待
```

## 8. 临界区

今天的临界区是：

```c
pthread_mutex_lock(&g_mutex);

g_counter++;

pthread_mutex_unlock(&g_mutex);
```

临界区指的是：

```text
多个线程都可能访问的共享资源操作区域
```

这里共享资源是：

```c
g_counter
```

所以需要用互斥锁保护。

## 9. pthread_mutex_destroy()

```c
pthread_mutex_destroy(&g_mutex);
```

作用：销毁互斥锁，释放相关资源。

注意拼写：

```text
destroy，不是 destory
```

今天遇到过错误写法：

```c
pthread_mutex_destory(&g_mutex);
```

正确写法：

```c
pthread_mutex_destroy(&g_mutex);
```

## 10. 编译命令

```bash
gcc mutex_demo.c -o mutex_demo -pthread
```

注意：

```text
必须是 -pthread
不要写成 -pyhread
```

`-pthread` 的作用是启用 pthread 支持并链接 pthread 相关库。

## 11. 今日遇到的问题

### 问题 1：pthread_mutex_destroy 拼写错误

错误写法：

```c
pthread_mutex_destory(&g_mutex);
```

报错类似：

```text
undefined reference to pthread_mutex_destory
```

原因是函数名拼错。

正确写法：

```c
pthread_mutex_destroy(&g_mutex);
```

### 问题 2：-pthread 拼写错误

错误写法：

```bash
gcc mutex_demo.c -o mutex_demo -pyhread
```

正确写法：

```bash
gcc mutex_demo.c -o mutex_demo -pthread
```

### 问题 3：去掉锁后结果不稳定

现象：

```text
final counter=181555
```

原因：

```text
多个线程同时执行 g_counter++，发生数据竞争
```

解决：

```c
pthread_mutex_lock(&g_mutex);

g_counter++;

pthread_mutex_unlock(&g_mutex);
```

## 12. 面试考点

### 问：互斥锁解决什么问题？

答：

```text
互斥锁用于解决多个线程同时访问共享资源导致的数据竞争问题。
```

### 问：为什么 g_counter++ 需要加锁？

答：

```text
因为 g_counter++ 不是原子操作，它包含读取、加一、写回多个步骤。
多个线程同时执行时可能互相覆盖结果。
```

### 问：pthread_mutex_lock 和 pthread_mutex_unlock 的作用是什么？

答：

```text
pthread_mutex_lock 用于进入临界区前加锁。
pthread_mutex_unlock 用于离开临界区后解锁。
```

### 问：lock 和 unlock 为什么要成对出现？

答：

```text
如果加锁后不解锁，其他线程会一直等待，可能造成死锁。
```

## 13. 和物联网网关项目的关系

后续网关项目中会有多个线程：

```text
串口线程：读取 STM32 数据
转发线程：发送 TCP / MQTT 数据
日志线程：写日志文件
```

这些线程可能会访问同一份共享数据，例如：

```text
最新温度
最新湿度
设备状态
发送队列
日志缓冲区
```

如果不加锁，可能出现数据错乱。

因此 Day5 的互斥锁是后续网关多线程安全的基础。
