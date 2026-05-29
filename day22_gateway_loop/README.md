# Day22 gateway_loop：循环采集与日志保存

## 一、今日目标

今天将 Day21 的 gateway_v1 升级为循环运行版本。

Day21 只处理一次数据：

```text
模拟数据
↓
解析
↓
JSON
↓
写日志
```

Day22 升级为：

```text
模拟周期数据
↓
循环解析
↓
生成 JSON
↓
持续写入 sensor_log.txt
```

## 二、项目结构

```text
day22_gateway_loop/
├── gateway_loop.c
├── README.md
├── sensor_log.txt
└── notes/
    └── day22_note.md
```

## 三、编译命令

```bash
gcc gateway_loop.c -o gateway_loop
```

## 四、运行命令

```bash
./gateway_loop
```

查看日志：

```bash
cat sensor_log.txt
```

实时查看日志：

```bash
tail -f sensor_log.txt
```

停止实时查看：

```text
Ctrl + C
```

## 五、实验现象

程序运行后，日志文件会持续追加传感器 JSON 数据。

示例：

```text
[2026-05-28 20:06:43] {"temp":25,"humi":60,"device_id":"stm32_01"}
[2026-05-28 20:06:45] {"temp":25,"humi":60,"device_id":"stm32_01"}
[2026-05-28 20:06:47] {"temp":25,"humi":60,"device_id":"stm32_01"}
[2026-05-28 20:06:49] {"temp":25,"humi":60,"device_id":"stm32_01"}
[2026-05-28 20:06:51] {"temp":25,"humi":60,"device_id":"stm32_01"}
```

说明：

```text
gateway_loop 可以循环处理数据
JSON 数据可以持续写入日志
tail -f 可以实时观察日志变化
```

## 六、核心功能

本实验实现了：

```text
1. 模拟周期性传感器数据
2. 循环解析传感器字符串
3. 生成 JSON 数据
4. 持续追加写入 sensor_log.txt
5. 使用 tail -f 实时观察日志
```

## 七、核心流程

```text
raw_data
↓
parse_sensor_data()
↓
SensorData
↓
build_json()
↓
json_buf
↓
write_log()
↓
sensor_log.txt
↓
sleep(2)
↓
继续下一轮
```

## 八、和真实网关的关系

当前版本使用模拟数据。

后续真实版本会替换为：

```text
STM32 USART1 发送数据
↓
Linux read() 读取串口
↓
parse_sensor_data()
↓
build_json()
↓
write_log()
↓
mosquitto_publish()
```

## 九、今日总结

Day22 完成了 gateway_v1 的循环版本。

当前网关已经具备：

```text
循环运行能力
持续处理数据能力
日志持续保存能力
实时查看日志能力
```

这一步让程序更接近真实物联网网关。
