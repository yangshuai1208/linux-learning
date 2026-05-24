# Day18 笔记：使用 C 程序订阅 MQTT 消息

## 1. 今日完成内容

今天完成了使用 C 语言订阅 MQTT 消息的实验。

完成内容：

```text
编写 mqtt_subscribe.c
连接本地 Mosquitto Broker
订阅 gateway/stm32_01/sensor
接收并打印 MQTT 消息
使用 mosquitto_pub 测试消息发布
```

最终实验现象：

```text
mqtt subscribe client started
connect broker success
subscribe topic: gateway/stm32_01/sensor
recv mqtt message
topic: gateway/stm32_01/sensor
payload: {"temp":25,"humi":60,"device_id":"stm32_01"}
```

说明 C 程序订阅 MQTT 消息成功。

## 2. Day18 和 Day17 的区别

Day17 是发布 MQTT 消息：

```text
C 程序 publish
↓
Broker
↓
订阅端收到
```

Day18 是订阅 MQTT 消息：

```text
发布端发送消息
↓
Broker
↓
C 程序 subscribe 收到消息
```

Day17 解决的是“网关上传数据”。

Day18 解决的是“网关接收命令”。

## 3. 头文件

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
```

作用：

```text
stdio.h：printf()
stdlib.h：通用库函数
string.h：字符串相关函数
mosquitto.h：Mosquitto MQTT API
```

## 4. 宏定义

```c
#define MQTT_HOST "localhost"
#define MQTT_PORT 1883
#define MQTT_KEEPALIVE 60
#define MQTT_TOPIC "gateway/stm32_01/sensor"
```

解释：

```text
MQTT_HOST：Broker 地址
MQTT_PORT：MQTT 默认端口 1883
MQTT_KEEPALIVE：心跳时间
MQTT_TOPIC：订阅的 topic
```

## 5. mosquitto_lib_init()

```c
mosquitto_lib_init();
```

作用：

```text
初始化 Mosquitto 库。
```

写 Mosquitto C 程序时，一般先调用它。

## 6. mosquitto_new()

```c
mosq = mosquitto_new("gateway_sub_client", true, NULL);
```

作用：

```text
创建一个 MQTT client 对象。
```

参数解释：

```text
"gateway_sub_client"：客户端 ID
true：断开后清除会话
NULL：用户自定义数据，这里不使用
```

## 7. mosquitto_connect_callback_set()

```c
mosquitto_connect_callback_set(mosq, on_connect);
```

作用：

```text
设置连接成功后的回调函数。
```

当程序成功连接 Broker 后，Mosquitto 库会自动调用：

```c
on_connect()
```

## 8. on_connect() 回调函数

```c
void on_connect(struct mosquitto *mosq, void *userdata, int rc)
{
    int ret;

    if (rc == 0)
    {
        printf("connect broker success\n");

        ret = mosquitto_subscribe(mosq, NULL, MQTT_TOPIC, 0);
        if (ret != MOSQ_ERR_SUCCESS)
        {
            printf("subscribe failed: %s\n", mosquitto_strerror(ret));
        }
        else
        {
            printf("subscribe topic: %s\n", MQTT_TOPIC);
        }
    }
    else
    {
        printf("connect broker failed, rc=%d\n", rc);
    }
}
```

作用：

```text
连接 Broker 成功后，订阅指定 topic。
```

其中：

```text
rc == 0 表示连接成功
rc != 0 表示连接失败
```

## 9. mosquitto_subscribe()

```c
mosquitto_subscribe(mosq, NULL, MQTT_TOPIC, 0);
```

作用：

```text
订阅指定 topic。
```

参数解释：

```text
mosq：MQTT client 对象
NULL：消息 ID，这里不使用
MQTT_TOPIC：订阅的 topic
0：QoS 等级为 0
```

## 10. mosquitto_message_callback_set()

```c
mosquitto_message_callback_set(mosq, on_message);
```

作用：

```text
设置收到消息后的回调函数。
```

只要订阅的 topic 收到消息，就会自动调用：

```c
on_message()
```

## 11. on_message() 回调函数

```c
void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg)
{
    printf("recv mqtt message\n");
    printf("topic: %s\n", msg->topic);
    printf("payload: %.*s\n", msg->payloadlen, (char *)msg->payload);
}
```

作用：

```text
接收并打印 MQTT 消息。
```

其中：

```text
msg->topic：收到消息的 topic
msg->payload：消息内容
msg->payloadlen：消息长度
```

重点代码：

```c
printf("payload: %.*s\n", msg->payloadlen, (char *)msg->payload);
```

含义：

```text
按照 payloadlen 指定的长度打印 payload。
```

这样比直接用 `%s` 更安全，因为 MQTT payload 不一定以 `\0` 结尾。

## 12. mosquitto_connect()

```c
ret = mosquitto_connect(mosq, MQTT_HOST, MQTT_PORT, MQTT_KEEPALIVE);
```

作用：

```text
连接 MQTT Broker。
```

本实验连接：

```text
localhost:1883
```

如果 Mosquitto 没启动，可能连接失败。

解决：

```bash
sudo service mosquitto start
```

## 13. mosquitto_loop_forever()

```c
ret = mosquitto_loop_forever(mosq, -1, 1);
```

作用：

```text
一直循环处理 MQTT 网络事件。
```

订阅程序需要长期运行等待消息，所以使用：

```c
mosquitto_loop_forever()
```

运行后终端一直不退出是正常现象。

停止程序：

```text
Ctrl + C
```

## 14. mosquitto_destroy()

```c
mosquitto_destroy(mosq);
```

作用：

```text
销毁 MQTT client 对象。
```

## 15. mosquitto_lib_cleanup()

```c
mosquitto_lib_cleanup();
```

作用：

```text
清理 Mosquitto 库资源。
```

## 16. 今日命令总结

进入目录：

```bash
cd ~/linux-learning-submit/day18_mqtt_subscribe
```

编译：

```bash
gcc mqtt_subscribe.c -o mqtt_subscribe -lmosquitto
```

运行订阅程序：

```bash
./mqtt_subscribe
```

使用命令行发布测试消息：

```bash
mosquitto_pub -h localhost -t gateway/stm32_01/sensor -m '{"temp":25,"humi":60,"device_id":"stm32_01"}'
```

或者使用 Day17 C 发布程序：

```bash
cd ~/linux-learning-submit/day17_mqtt_publish
./mqtt_publish
```

## 17. 今日遇到的问题

### 问题 1：mosquitto_pub 命令参数写错

错误现象：

```text
Error: Unknown option 'gateway/stm32_01/sensor'
```

常见原因：

```text
-h localhost 和 -t 之间少了空格
或者 -t 后面的 topic 写法不对
```

正确写法：

```bash
mosquitto_pub -h localhost -t gateway/stm32_01/sensor -m '{"temp":25,"humi":60,"device_id":"stm32_01"}'
```

注意：

```text
localhost 和 -t 中间必须有空格
topic 和 -m 中间必须有空格
JSON 外面用英文单引号
JSON 里面用英文双引号
```

### 问题 2：程序运行后不退出

原因：

```text
mosquitto_loop_forever() 会一直等待 MQTT 消息。
```

这不是卡死，是订阅程序的正常现象。

停止：

```text
Ctrl + C
```

### 问题 3：订阅端收不到消息

检查：

```text
1. Mosquitto Broker 是否启动
2. mqtt_subscribe 是否先运行
3. 发布端和订阅端 topic 是否完全一致
4. 发布命令是否写对
```

## 18. 面试考点

### 问：C 程序订阅 MQTT 消息的基本流程是什么？

答：

```text
初始化 Mosquitto 库，创建 MQTT client，设置连接回调和消息回调，连接 Broker，在连接成功回调中订阅 topic，然后通过事件循环持续接收消息。
```

### 问：on_connect() 的作用是什么？

答：

```text
on_connect() 是连接成功后的回调函数，通常在这里执行 mosquitto_subscribe() 订阅 topic。
```

### 问：on_message() 的作用是什么？

答：

```text
on_message() 是收到 MQTT 消息后的回调函数，用于处理 topic 和 payload。
```

### 问：mosquitto_loop_forever() 的作用是什么？

答：

```text
mosquitto_loop_forever() 用于持续处理 MQTT 网络事件，使订阅程序保持运行并接收消息。
```

### 问：为什么打印 payload 用 `%.*s`？

答：

```text
因为 MQTT payload 不一定以字符串结束符 \0 结尾，使用 payloadlen 指定长度打印更安全。
```

## 19. 和物联网网关项目的关系

Day17 完成了：

```text
Linux 网关通过 MQTT publish 上传传感器数据
```

Day18 完成了：

```text
Linux 网关通过 MQTT subscribe 接收远程控制命令
```

后续网关可以设计两个 topic：

```text
gateway/stm32_01/sensor  上传传感器数据
gateway/stm32_01/cmd     接收控制命令
```

未来控制流程：

```text
服务器 publish led_on
↓
Linux 网关 subscribe 收到 led_on
↓
Linux 网关通过串口发送命令给 STM32
↓
STM32 控制 LED 打开
```

## 20. 今日总结

今天掌握了 C 程序订阅 MQTT 消息。

最重要 API：

```text
mosquitto_connect_callback_set()
mosquitto_message_callback_set()
mosquitto_subscribe()
mosquitto_loop_forever()
```

最关键一句话：

```text
Day18 完成了 C 程序 MQTT subscribe，为后续网关接收远程控制命令打基础。
```
