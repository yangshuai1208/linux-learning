# Day24 笔记：gateway_mqtt 模块化拆分与 Makefile

## 1. 今日完成内容

今天完成了 Day23 网关程序的模块化拆分。

完成内容：

```text
创建 day24_gateway_module
拆分 sensor_parser 模块
拆分 log_writer 模块
拆分 mqtt_client 模块
编写 main.c 主流程
编写 Makefile
使用 make 编译项目
运行 gateway_module
验证 MQTT 发布和日志写入
```

## 2. 为什么要模块化

Day23 是一个大文件：

```text
gateway_mqtt.c
```

所有功能都写在一个文件中：

```text
解析
JSON 生成
日志
MQTT
main 主流程
```

问题是：

```text
代码越来越长
不方便维护
不方便复用
不方便调试
不像真实工程
```

Day24 拆成多个模块后：

```text
main.c：主流程
sensor_parser.c：解析和 JSON
log_writer.c：日志
mqtt_client.c：MQTT
Makefile：统一编译
```

这样更符合真实 Linux C 项目结构。

## 3. main.c

`main.c` 只负责主流程。

主要流程：

```text
1. 定义模拟 STM32 数据
2. 初始化 MQTT
3. 循环处理每条数据
4. 调用 parse_sensor_data()
5. 调用 build_json()
6. 调用 write_log()
7. 调用 mqtt_publish_json()
8. 释放 MQTT 资源
```

核心代码：

```c
ret = mqtt_client_init(&mosq);

ret = parse_sensor_data(raw_data_list[i], &sensor);

ret = build_json(&sensor, json_buf, sizeof(json_buf));

ret = write_log(json_buf);

ret = mqtt_publish_json(mosq, json_buf);

mqtt_client_cleanup(mosq);
```

## 4. sensor_parser.h

头文件内容：

```c
#ifndef SENSOR_PARSER_H
#define SENSOR_PARSER_H

typedef struct
{
    int temp;
    int humi;
    char device_id[32];
} SensorData;

int parse_sensor_data(const char *raw, SensorData *data);
int build_json(const SensorData *data, char *json_buf, int buf_size);

#endif
```

作用：

```text
声明 SensorData 结构体
声明 parse_sensor_data()
声明 build_json()
让其他 .c 文件可以使用这些函数
```

## 5. sensor_parser.c

负责实现：

```text
parse_sensor_data()
build_json()
```

### parse_sensor_data()

作用：

```text
把 STM32 字符串解析成 SensorData 结构体。
```

输入：

```text
temp=25,humi=60,device_id=stm32_01
```

输出：

```text
sensor.temp = 25
sensor.humi = 60
sensor.device_id = stm32_01
```

核心代码：

```c
ret = sscanf(raw,
             "temp=%d,humi=%d,device_id=%31s",
             &data->temp,
             &data->humi,
             data->device_id);
```

### build_json()

作用：

```text
把 SensorData 结构体转换成 JSON 字符串。
```

核心代码：

```c
snprintf(json_buf,
         buf_size,
         "{\"temp\":%d,\"humi\":%d,\"device_id\":\"%s\"}",
         data->temp,
         data->humi,
         data->device_id);
```

生成结果：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
```

## 6. log_writer.h

内容：

```c
#ifndef LOG_WRITER_H
#define LOG_WRITER_H

int write_log(const char *json_data);

#endif
```

作用：

```text
声明 write_log() 函数
```

## 7. log_writer.c

负责实现日志写入。

核心函数：

```c
int write_log(const char *json_data)
```

核心代码：

```c
fp = fopen("sensor_log.txt", "a");

now = time(NULL);
time_info = localtime(&now);

strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", time_info);

fprintf(fp, "[%s] %s\n", time_buf, json_data);

fclose(fp);
```

作用：

```text
给每条 JSON 数据加时间戳
追加写入 sensor_log.txt
```

## 8. mqtt_client.h

内容：

```c
#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <mosquitto.h>

#define MQTT_HOST "localhost"
#define MQTT_PORT 1883
#define MQTT_KEEPALIVE 60
#define MQTT_TOPIC "gateway/stm32_01/sensor"

int mqtt_client_init(struct mosquitto **mosq);
int mqtt_publish_json(struct mosquitto *mosq, const char *json_data);
void mqtt_client_cleanup(struct mosquitto *mosq);

#endif
```

作用：

```text
定义 MQTT 参数
声明 MQTT 初始化函数
声明 MQTT 发布函数
声明 MQTT 清理函数
```

注意：

```text
#define MQTT_HOST "localhost"
```

宏名和字符串之间必须有空格。

错误写法：

```c
#define MQTT_HOST"localhost"
```

## 9. mqtt_client.c

负责 MQTT 相关功能。

### mqtt_client_init()

作用：

```text
初始化 Mosquitto 库
创建 MQTT client
连接 MQTT Broker
```

核心流程：

```text
mosquitto_lib_init()
mosquitto_new()
mosquitto_connect()
```

### mqtt_publish_json()

作用：

```text
把 JSON 数据发布到 MQTT topic。
```

核心代码：

```c
mosquitto_publish(
    mosq,
    NULL,
    MQTT_TOPIC,
    strlen(json_data),
    json_data,
    0,
    false
);
```

### mqtt_client_cleanup()

作用：

```text
断开 MQTT 连接
销毁 client 对象
清理 Mosquitto 库
```

核心流程：

```text
mosquitto_disconnect()
mosquitto_destroy()
mosquitto_lib_cleanup()
```

## 10. Makefile

Makefile 内容：

```makefile
CC = gcc
TARGET = gateway_module
OBJS = main.o sensor_parser.o log_writer.o mqtt_client.o
LIBS = -lmosquitto

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

main.o: main.c sensor_parser.h log_writer.h mqtt_client.h
	$(CC) -c main.c

sensor_parser.o: sensor_parser.c sensor_parser.h
	$(CC) -c sensor_parser.c

log_writer.o: log_writer.c log_writer.h
	$(CC) -c log_writer.c

mqtt_client.o: mqtt_client.c mqtt_client.h
	$(CC) -c mqtt_client.c

clean:
	rm -f $(OBJS) $(TARGET)
```

作用：

```text
自动编译多文件项目
```

执行：

```bash
make
```

等价于手动执行：

```bash
gcc -c main.c
gcc -c sensor_parser.c
gcc -c log_writer.c
gcc -c mqtt_client.c
gcc main.o sensor_parser.o log_writer.o mqtt_client.o -o gateway_module -lmosquitto
```

## 11. Makefile 注意事项

### 命令前必须是 Tab

例如：

```makefile
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)
```

前面必须是 Tab，不是空格。

### 链接命令必须有空格

正确：

```makefile
$(CC) $(OBJS) -o $(TARGET) $(LIBS)
```

错误：

```makefile
$(CC) $(OBJS)-o$(TARGET)-lmosquitto
```

否则会报类似：

```text
cannot find mqtt_client.o-ogateway_module-lmosquitto
```

原因是系统把它当成了一个文件名。

## 12. 今日遇到的问题

### 问题 1：文件名不一致

错误：

```text
sensor_parse.c
```

正确：

```text
sensor_parser.c
```

Makefile 中写的是 `sensor_parser.c`，文件名必须一致。

### 问题 2：宏定义缺少空格

错误：

```c
#define MQTT_HOST"localhost"
```

正确：

```c
#define MQTT_HOST "localhost"
```

### 问题 3：函数名拼写错误

错误：

```c
bubild_json()
```

正确：

```c
build_json()
```

### 问题 4：Makefile 链接命令缺少空格

错误现象：

```text
cannot find mqtt_client.o-ogateway_module-lmosquitto
```

原因：

```text
-o 和 -lmosquitto 前后缺少空格
```

正确：

```makefile
$(CC) $(OBJS) -o $(TARGET) $(LIBS)
```

## 13. 今日命令总结

进入目录：

```bash
cd ~/linux-learning-submit/day24_gateway_module
```

编译：

```bash
make
```

清理：

```bash
make clean
```

运行：

```bash
./gateway_module
```

订阅 MQTT：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

查看日志：

```bash
cat sensor_log.txt
```

## 14. 和前几天内容的关系

Day23：

```text
gateway_mqtt.c 单文件实现日志 + MQTT 上传
```

Day24：

```text
将单文件拆分成多个模块
使用 Makefile 统一构建
```

也就是：

```text
Day23 重点是功能跑通
Day24 重点是工程化组织
```

## 15. 面试考点

### 问：为什么要模块化？

答：

```text
模块化可以让代码结构更清晰，方便维护、调试、复用和扩展，更接近真实项目开发方式。
```

### 问：.h 文件和 .c 文件分别做什么？

答：

```text
.h 文件通常放结构体定义、宏定义和函数声明。
.c 文件通常放函数的具体实现。
```

### 问：Makefile 的作用是什么？

答：

```text
Makefile 用于自动化编译多文件 C 项目，避免每次手动输入复杂 gcc 命令。
```

### 问：为什么链接 Mosquitto 要加 -lmosquitto？

答：

```text
因为程序使用了 Mosquitto C 库函数，链接阶段需要链接 libmosquitto 库。
```

### 问：main.c 在模块化项目中的作用是什么？

答：

```text
main.c 只负责主流程调度，不直接关心每个模块内部实现。
```

## 16. 今日总结

今天完成了 gateway_mqtt 的模块化拆分。

掌握内容：

```text
多文件 C 工程
头文件声明
源文件实现
Makefile 编译
模块化设计
MQTT 模块封装
日志模块封装
传感器解析模块封装
```

最关键一句话：

```text
Day24 将网关项目从单文件 demo 升级为模块化 Linux C 工程，更接近真实项目结构。
```
