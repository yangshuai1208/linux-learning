# Day23 笔记：gateway_loop + MQTT publish 整合

## 1. 今日完成内容

今天完成了 gateway_loop 和 MQTT publish 的整合。

完成内容：

```text
编写 gateway_mqtt.c
复用 Day22 的循环数据处理逻辑
复用 parse_sensor_data()
复用 build_json()
复用 write_log()
新增 mqtt_publish_json()
使用 Mosquitto C API 发布 JSON
使用 mosquitto_sub 验证消息接收
```

实验现象：

```text
gateway_mqtt 可以循环生成 JSON
每条 JSON 可以写入 sensor_log.txt
每条 JSON 可以通过 MQTT 发布
mosquitto_sub 可以接收到多条消息
```

## 2. Day22 和 Day23 的区别

Day22：

```text
模拟数据
↓
解析
↓
JSON
↓
写日志
```

Day23：

```text
模拟数据
↓
解析
↓
JSON
↓
写日志
↓
MQTT publish
```

Day23 的意义是：

```text
网关不再只是本地处理数据，而是可以把数据发布到 MQTT Broker。
```

## 3. MQTT 相关宏定义

代码：

```c
#define MQTT_HOST "localhost"
#define MQTT_PORT 1883
#define MQTT_KEEPALIVE 60
#define MQTT_TOPIC "gateway/stm32_01/sensor"
```

解释：

```text
MQTT_HOST：Broker 地址，本地测试用 localhost
MQTT_PORT：MQTT 默认端口 1883
MQTT_KEEPALIVE：心跳时间
MQTT_TOPIC：发布传感器数据的主题
```

## 4. mqtt_publish_json() 函数

代码：

```c
int mqtt_publish_json(struct mosquitto *mosq, const char *json_data)
{
    int ret;

    if (mosq == NULL || json_data == NULL)
    {
        return -1;
    }

    ret = mosquitto_publish(
        mosq,
        NULL,
        MQTT_TOPIC,
        strlen(json_data),
        json_data,
        0,
        false
    );

    if (ret != MOSQ_ERR_SUCCESS)
    {
        printf("mqtt publish failed: %s\n", mosquitto_strerror(ret));
        return -1;
    }

    mosquitto_loop(mosq, 1000, 1);

    return 0;
}
```

作用：

```text
把 JSON 字符串发布到 MQTT Broker。
```

## 5. mosquitto_publish()

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

参数解释：

```text
mosq：MQTT client 对象
NULL：消息 ID，这里不使用
MQTT_TOPIC：发布到哪个 topic
strlen(json_data)：消息长度
json_data：消息内容
0：QoS 等级为 0
false：不是 retained 消息
```

## 6. mosquitto_loop()

代码：

```c
mosquitto_loop(mosq, 1000, 1);
```

作用：

```text
让 Mosquitto 库处理网络收发，确保 publish 消息真正发出去。
```

如果 publish 后程序马上继续执行或退出，调用 `mosquitto_loop()` 可以提高发送稳定性。

## 7. MQTT 初始化流程

代码流程：

```text
mosquitto_lib_init()
↓
mosquitto_new()
↓
mosquitto_connect()
↓
循环 mosquitto_publish()
↓
mosquitto_disconnect()
↓
mosquitto_destroy()
↓
mosquitto_lib_cleanup()
```

## 8. 为什么 MQTT 连接放在循环外面

正确做法：

```text
程序启动时连接一次 MQTT Broker
循环中只负责 publish 数据
程序结束时断开连接
```

不推荐每条数据都：

```text
connect
publish
disconnect
```

原因：

```text
频繁连接和断开效率低
真实网关一般是长期连接 MQTT Broker
```

## 9. 主循环流程

每轮循环做：

```text
1. 取出一条模拟 raw_data
2. parse_sensor_data() 解析
3. build_json() 生成 JSON
4. write_log() 写入日志
5. mqtt_publish_json() 发布 MQTT
6. sleep(2) 模拟周期上报
```

## 10. 实验测试方法

终端 1：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

终端 2：

```bash
cd ~/linux-learning-submit/day23_gateway_mqtt
./gateway_mqtt
```

订阅端收到：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
{"temp":26,"humi":61,"device_id":"stm32_01"}
{"temp":27,"humi":62,"device_id":"stm32_01"}
{"temp":28,"humi":63,"device_id":"stm32_01"}
```

说明 MQTT 发布成功。

## 11. 编译命令

```bash
gcc gateway_mqtt.c -o gateway_mqtt -lmosquitto
```

说明：

```text
-lmosquitto 用于链接 Mosquitto C 库。
```

如果漏掉，可能报：

```text
undefined reference to `mosquitto_xxx'
```

## 12. 常见问题

### 问题 1：MQTT 连接失败

现象：

```text
mqtt connect failed: Connection refused
```

原因：

```text
Mosquitto Broker 没启动。
```

解决：

```bash
sudo service mosquitto start
```

### 问题 2：订阅端收不到消息

检查：

```text
1. mosquitto_sub 是否先运行
2. topic 是否完全一致
3. gateway_mqtt 是否显示 mqtt publish success
4. Mosquitto 服务是否启动
```

topic 必须一致：

```text
gateway/stm32_01/sensor
```

### 问题 3：编译失败 undefined reference

原因：

```text
编译时漏了 -lmosquitto。
```

正确：

```bash
gcc gateway_mqtt.c -o gateway_mqtt -lmosquitto
```

## 13. 今日命令总结

启动 Mosquitto：

```bash
sudo service mosquitto start
```

进入目录：

```bash
cd ~/linux-learning-submit/day23_gateway_mqtt
```

编辑代码：

```bash
nano -l gateway_mqtt.c
```

编译：

```bash
gcc gateway_mqtt.c -o gateway_mqtt -lmosquitto
```

订阅：

```bash
mosquitto_sub -h localhost -t gateway/stm32_01/sensor
```

运行网关：

```bash
./gateway_mqtt
```

查看日志：

```bash
cat sensor_log.txt
```

## 14. 和物联网网关项目的关系

Day19：

```text
Linux 串口读取 STM32 数据
```

Day20：

```text
字符串解析成结构体和 JSON
```

Day21：

```text
gateway_v1：解析 + JSON + 日志
```

Day22：

```text
gateway_loop：循环处理 + 持续日志
```

Day23：

```text
gateway_mqtt：循环处理 + 日志 + MQTT 上传
```

也就是说，Day23 已经完成了网关核心主链路：

```text
数据输入
↓
数据解析
↓
JSON 封装
↓
日志保存
↓
MQTT 上传
```

## 15. 面试考点

### 问：gateway_mqtt 实现了什么？

答：

```text
gateway_mqtt 实现了模拟传感器数据的循环处理、JSON 封装、日志保存和 MQTT 发布，是 Linux 物联网网关的核心雏形。
```

### 问：为什么 MQTT 连接要放在循环外？

答：

```text
真实网关通常长期连接 Broker，循环中只发布数据。频繁连接和断开会降低效率，也不符合长期运行程序的设计。
```

### 问：mosquitto_publish() 的作用是什么？

答：

```text
mosquitto_publish() 用于向指定 MQTT topic 发布消息。
```

### 问：QoS 0 表示什么？

答：

```text
QoS 0 表示最多发送一次，开销最低，但不保证消息一定送达。
```

### 问：为什么要同时写日志和 MQTT 发布？

答：

```text
日志用于本地调试和历史追踪，MQTT 用于把数据上传到服务器或云平台。两者结合更接近真实网关项目。
```

## 16. 今日总结

今天完成了 gateway_loop 与 MQTT publish 的整合。

掌握内容：

```text
Mosquitto C API
mosquitto_publish()
MQTT topic / payload
循环发布 JSON
本地日志 + MQTT 上传
网关主流程整合
```

最关键一句话：

```text
Day23 已经完成 Linux 网关的核心上传链路：模拟数据 -> 解析 -> JSON -> 日志 -> MQTT publish。
```
