# Day25 笔记：gateway_datasource 数据源模块封装

## 1. 今日完成内容

今天在 Day24 模块化网关基础上，新增了 `data_source` 数据源模块。

完成内容：

```text
复制 Day24 工程为 Day25
新增 config.h
新增 data_source.h
新增 data_source.c
修改 main.c
修改 Makefile
编译生成 gateway_datasource
运行 mock 数据模式
验证 MQTT 发布成功
```

## 2. 今日核心思想

Day24 中，模拟数据直接写在 `main.c` 里：

```c
raw_data_list[i]
```

这样的问题是：

```text
main.c 和数据来源耦合太紧
后续切换真实串口时，需要大改 main.c
```

Day25 改为：

```c
data_source_read(raw_buf, sizeof(raw_buf));
```

这样 `main.c` 不再关心数据来自哪里。

数据可以来自：

```text
模拟数组
真实串口
网络
文件
其他设备
```

这就是解耦思想。

## 3. config.h

代码：

```c
#ifndef CONFIG_H
#define CONFIG_H

#define USE_MOCK_DATA 1

#define LOOP_COUNT 5

#define SERIAL_DEV "/dev/ttyUSB0"

#endif
```

作用：

```text
统一管理项目配置。
```

字段说明：

```text
USE_MOCK_DATA：选择数据源模式
LOOP_COUNT：主循环次数
SERIAL_DEV：串口设备路径
```

当前：

```c
#define USE_MOCK_DATA 1
```

表示使用模拟数据。

后续真实串口模式：

```c
#define USE_MOCK_DATA 0
```

## 4. data_source.h

代码：

```c
#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H

int data_source_init(void);
int data_source_read(char *buf, int buf_size);
void data_source_cleanup(void);

#endif
```

作用：

```text
声明数据源模块对外提供的接口。
```

三个函数：

```text
data_source_init()：初始化数据源
data_source_read()：读取一条数据
data_source_cleanup()：清理数据源资源
```

## 5. data_source.c 的两种模式

`data_source.c` 使用条件编译：

```c
#if USE_MOCK_DATA == 1

// mock 模式

#else

// serial 模式

#endif
```

含义：

```text
USE_MOCK_DATA == 1 时，编译模拟数据代码
USE_MOCK_DATA == 0 时，编译真实串口代码
```

今天遇到过：

```text
error: unterminated #else
```

原因：

```text
#if 和 #else 后面缺少 #endif
```

正确结构：

```c
#if USE_MOCK_DATA == 1

// 模拟数据模式

#else

// 串口数据模式

#endif
```

## 6. mock 模式数据

代码：

```c
static const char *mock_data_list[] =
{
    "temp=25,humi=60,device_id=stm32_01",
    "temp=26,humi=61,device_id=stm32_01",
    "temp=27,humi=62,device_id=stm32_01",
    "temp=28,humi=63,device_id=stm32_01",
    "temp=29,humi=64,device_id=stm32_01"
};
```

作用：

```text
模拟 STM32 周期上报的传感器数据。
```

## 7. mock_index

代码：

```c
static int mock_index = 0;
```

作用：

```text
记录当前读取到第几条模拟数据。
```

每次读取后：

```c
mock_index++;
```

如果超过数组数量：

```c
mock_index = 0;
```

这样可以循环读取模拟数据。

## 8. data_source_init()

mock 模式代码：

```c
int data_source_init(void)
{
    printf("data source: mock mode\n");
    return 0;
}
```

作用：

```text
初始化数据源。
```

当前 mock 模式不需要打开硬件设备，所以只打印提示并返回成功。

真实串口模式下，这个函数后续会负责：

```text
open() 打开 /dev/ttyUSB0
termios 配置串口
```

## 9. data_source_read()

mock 模式核心代码：

```c
int data_source_read(char *buf, int buf_size)
{
    int count;

    if (buf == NULL || buf_size <= 0)
    {
        return -1;
    }

    count = sizeof(mock_data_list) / sizeof(mock_data_list[0]);

    snprintf(buf, buf_size, "%s", mock_data_list[mock_index]);

    mock_index++;
    if (mock_index >= count)
    {
        mock_index = 0;
    }

    sleep(2);

    return 0;
}
```

作用：

```text
读取一条数据，并保存到 buf 中。
```

参数：

```text
buf：保存读取结果的缓冲区
buf_size：缓冲区大小
```

为什么使用 `snprintf()`：

```text
可以限制写入长度，防止缓冲区溢出。
```

## 10. data_source_cleanup()

mock 模式代码：

```c
void data_source_cleanup(void)
{
    printf("data source cleanup\n");
}
```

作用：

```text
清理数据源资源。
```

当前 mock 模式没有硬件资源，所以只打印提示。

真实串口模式后续会在这里：

```text
close(serial_fd)
```

## 11. serial 模式预留

Day25 的 `data_source.c` 中已经预留了真实串口模式。

核心逻辑：

```text
open(SERIAL_DEV, O_RDWR | O_NOCTTY)
tcgetattr()
cfsetispeed()
cfsetospeed()
tcsetattr()
read()
close()
```

后续只要 WSL 能识别 `/dev/ttyUSB0`，就可以继续测试真实串口读取。

## 12. main.c 的变化

Day24 中：

```c
raw_data_list[i]
```

Day25 中：

```c
ret = data_source_read(raw_buf, sizeof(raw_buf));
```

主流程变成：

```text
data_source_init()
↓
mqtt_client_init()
↓
data_source_read()
↓
parse_sensor_data()
↓
build_json()
↓
write_log()
↓
mqtt_publish_json()
↓
mqtt_client_cleanup()
↓
data_source_cleanup()
```

这让 `main.c` 更像真实项目中的调度层。

## 13. Makefile 修改

Day25 新增了：

```text
data_source.c
data_source.h
config.h
```

所以 Makefile 中需要加入：

```makefile
OBJS = main.o data_source.o sensor_parser.o log_writer.o mqtt_client.o
```

并新增规则：

```makefile
data_source.o: data_source.c data_source.h config.h
	$(CC) -c data_source.c
```

注意：

```text
Makefile 命令前必须是 Tab，不是空格。
```

## 14. 今日遇到的问题

### 问题 1：把 C 文件当成命令运行

错误操作：

```bash
data_source.c
```

结果：

```text
data_source.c: command not found
```

原因：

```text
.c 文件不是可执行命令。
```

正确编辑：

```bash
nano data_source.c
```

正确编译：

```bash
make
```

### 问题 2：条件编译缺少 #endif

错误：

```text
error: unterminated #else
```

原因：

```text
#if / #else 没有用 #endif 闭合。
```

正确：

```c
#if USE_MOCK_DATA == 1
...
#else
...
#endif
```

### 问题 3：编译生成成功后还需要运行测试

`make` 成功只说明代码编译通过。

还需要执行：

```bash
./gateway_datasource
```

并用：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

验证 MQTT 是否收到消息。

## 15. 今日命令总结

进入目录：

```bash
cd ~/linux-learning-submit/day25_gateway_datasource
```

编辑配置：

```bash
nano config.h
```

编辑数据源模块：

```bash
nano data_source.h
nano data_source.c
```

编辑主函数：

```bash
nano main.c
```

编辑 Makefile：

```bash
nano Makefile
```

编译：

```bash
make clean
make
```

启动 MQTT：

```bash
sudo service mosquitto start
```

订阅：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

运行：

```bash
./gateway_datasource
```

查看日志：

```bash
cat sensor_log.txt
```

## 16. 和前几天的关系

Day24：

```text
完成模块化拆分
main.c + sensor_parser + log_writer + mqtt_client
```

Day25：

```text
新增 data_source 模块
让 main.c 不再直接依赖模拟数据数组
```

项目结构进一步升级：

```text
数据源层
↓
解析层
↓
日志层
↓
MQTT 通信层
↓
主流程调度层
```

## 17. 面试考点

### 问：为什么要新增 data_source 模块？

答：

```text
为了让 main.c 和具体数据来源解耦。后续无论数据来自模拟数组还是真实串口，主流程都不需要大改。
```

### 问：config.h 的作用是什么？

答：

```text
config.h 用来集中管理项目配置，例如是否使用模拟数据、循环次数和串口设备路径。
```

### 问：条件编译有什么作用？

答：

```text
条件编译可以根据宏定义选择编译不同代码，例如 USE_MOCK_DATA 为 1 时编译模拟数据逻辑，为 0 时编译串口逻辑。
```

### 问：data_source_read() 的作用是什么？

答：

```text
data_source_read() 对外提供统一的数据读取接口，隐藏底层数据来源细节。
```

### 问：Day25 相比 Day24 的提升是什么？

答：

```text
Day24 完成模块化，Day25 进一步抽象数据源，使项目更容易切换到真实串口输入，更接近真实网关架构。
```

## 18. 今日总结

今天完成了 `data_source` 数据源模块封装。

掌握内容：

```text
数据源抽象
条件编译
config.h 配置管理
mock 数据模式
serial 模式预留
main.c 与数据来源解耦
Makefile 新增模块
```

最关键一句话：

```text
Day25 让网关项目从“模块化工程”进一步升级为“可切换数据源的网关架构”，为后续接入真实 STM32 串口数据做准备。
```
