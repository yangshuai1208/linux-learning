# Day20 串口字符串解析成结构体和 JSON

## 一、今日目标

今天完成 STM32 串口字符串解析实验。

目标是把模拟 STM32 发来的字符串：

```text
temp=25,humi=60,device_id=stm32_01
```

解析成结构体：

```c
typedef struct
{
    int temp;
    int humi;
    char device_id[32];
} SensorData;
```

再转换成 JSON 字符串：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
```

## 二、项目结构

```text
day20_sensor_parse/
├── sensor_parse.c
├── README.md
└── notes/
    └── day20_note.md
```

## 三、编译命令

```bash
gcc sensor_parse.c -o sensor_parse
```

注意：

```text
gcc 和 sensor_parse.c 中间有空格
sensor_parse.c 和 -o 中间有空格
-o 和 sensor_parse 中间有空格
```

## 四、运行命令

```bash
./sensor_parse
```

注意：

```text
./ 和 sensor_parse 中间没有空格
```

## 五、实验现象

运行结果：

```text
raw data: temp=25,humi=60,device_id=stm32_01
parse success
temp: 25
humi: 60
device_id: stm32_01
json: {"temp":25,"humi":60,"device_id":"stm32_01"}
```

说明：

```text
原始字符串读取成功
sscanf() 解析成功
SensorData 结构体保存成功
snprintf() 生成 JSON 成功
```

## 六、核心功能

本实验实现了两个核心函数：

```text
parse_sensor_data()：解析原始字符串
build_json()：根据结构体生成 JSON
```

数据流：

```text
模拟 STM32 串口字符串
↓
parse_sensor_data()
↓
SensorData 结构体
↓
build_json()
↓
JSON 字符串
```

## 七、和物联网网关项目的关系

Day19 完成的是 Linux 串口读取代码框架：

```text
STM32 串口数据
↓
Linux read() 读取
```

Day20 完成的是数据解析：

```text
串口字符串
↓
解析成结构体
↓
生成 JSON
```

后续网关完整流程：

```text
STM32 发送 temp=25,humi=60,device_id=stm32_01
↓
Linux 读取串口
↓
parse_sensor_data()
↓
build_json()
↓
mosquitto_publish()
↓
MQTT Broker
```

## 八、今日总结

今天完成了串口字符串解析和 JSON 拼接。

掌握内容：

```text
结构体封装传感器数据
sscanf() 解析字符串
snprintf() 生成 JSON
字符串解析错误判断
为 MQTT publish 准备数据格式
```

Day20 是后续 gateway_v1 的数据解析模块基础。
