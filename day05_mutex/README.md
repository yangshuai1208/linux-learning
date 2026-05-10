# Day05 互斥锁 mutex

## 一、今日目标

今天完成 Linux 多线程互斥锁实验：

- 理解多线程访问共享变量时可能产生数据竞争
- 学会使用 `pthread_mutex_t` 定义互斥锁
- 学会使用 `pthread_mutex_init()` 初始化互斥锁
- 学会使用 `pthread_mutex_lock()` 和 `pthread_mutex_unlock()` 保护临界区
- 学会使用 `pthread_mutex_destroy()` 销毁互斥锁

## 二、项目结构

```text
day05_mutex/
├── mutex_demo.c
├── README.md
└── notes/
    └── day05_note.md
```

## 三、编译命令

```bash
gcc mutex_demo.c -o mutex_demo -pthread
```

注意：

```text
gcc 和 mutex_demo.c 中间有空格
mutex_demo.c 和 -o 中间有空格
-o 和 mutex_demo 中间有空格
mutex_demo 和 -pthread 中间有空格
```

## 四、运行命令

```bash
./mutex_demo
```

## 五、实验现象

加互斥锁后，多次运行结果稳定为：

```text
final counter=200000
```

原因：

```text
线程1：g_counter 加 100000 次
线程2：g_counter 加 100000 次
最终结果应该是 200000
```

去掉互斥锁后，多次运行可能出现：

```text
final counter=181555
```

说明多个线程同时执行 `g_counter++` 时发生了数据竞争，导致结果不稳定。

## 六、今日总结

今天完成了 pthread 互斥锁实验。

`g_counter++` 看起来只有一行，但底层不是原子操作，可能包含：

```text
读取 g_counter
加 1
写回 g_counter
```

多个线程同时执行时可能互相覆盖结果。

使用互斥锁后：

```c
pthread_mutex_lock(&g_mutex);

g_counter++;

pthread_mutex_unlock(&g_mutex);
```

可以保证同一时间只有一个线程修改共享变量，从而避免数据竞争。

后续物联网网关项目中，串口线程、转发线程、日志线程可能会访问同一份共享数据，因此需要互斥锁保证数据安全。
