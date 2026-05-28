# Day20 笔记：串口字符串解析成结构体和 JSON

## 1. 今日完成内容

今天完成了传感器字符串解析实验。

完成内容：

```text
定义 SensorData 结构体
编写 parse_sensor_data() 函数
使用 sscanf() 解析 temp、humi、device_id
编写 build_json() 函数
使用 snprintf() 生成 JSON 字符串
完成编译和运行测试
```

最终实验结果：

```text
raw data: temp=25,humi=60,device_id=stm32_01
parse success
temp: 25
humi: 60
device_id: stm32_01
json: {"temp":25,"humi":60,"device_id":"stm32_01"}
```

说明 Day20 实验成功。

## 2. 今日数据流

今天的数据流是：

```text
原始字符串
↓
parse_sensor_data()
↓
SensorData 结构体
↓
build_json()
↓
JSON 字符串
```

原始字符串：

```text
temp=25,humi=60,device_id=stm32_01
```

生成 JSON：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
```

## 3. SensorData 结构体

代码：

```c
typedef struct
{
    int temp;
    int humi;
    char device_id[32];
} SensorData;
```

作用：

```text
用一个结构体统一保存一条传感器数据。
```

字段解释：

```text
temp：温度
humi：湿度
device_id：设备编号
```

为什么要用结构体：

```text
比零散变量更清晰
方便函数传参
方便后续扩展更多传感器字段
适合网关项目模块化设计
```

## 4. parse_sensor_data() 函数

代码：

```c
int parse_sensor_data(const char *raw, SensorData *data)
{
    int ret;

    if (raw == NULL || data == NULL)
    {
        return -1;
    }

    memset(data, 0, sizeof(SensorData));

    ret = sscanf(raw,
                 "temp=%d,humi=%d,device_id=%31s",
                 &data->temp,
                 &data->humi,
                 data->device_id);

    if (ret != 3)
    {
        return -1;
    }

    return 0;
}
```

作用：

```text
把原始字符串解析成 SensorData 结构体。
```

输入：

```text
temp=25,humi=60,device_id=stm32_01
```

输出：

```text
data->temp = 25
data->humi = 60
data->device_id = stm32_01
```

## 5. 参数检查

代码：

```c
if (raw == NULL || data == NULL)
{
    return -1;
}
```

作用：

```text
防止传入空指针。
```

如果 raw 或 data 是 NULL，函数直接返回失败。

这是写 C 程序时的基本防御性写法。

## 6. memset()

代码：

```c
memset(data, 0, sizeof(SensorData));
```

作用：

```text
清空结构体内容，避免残留脏数据。
```

如果不清空，device_id 数组里可能有旧数据。

## 7. sscanf()

代码：

```c
ret = sscanf(raw,
             "temp=%d,humi=%d,device_id=%31s",
             &data->temp,
             &data->humi,
             data->device_id);
```

作用：

```text
按照指定格式从字符串中提取数据。
```

格式解释：

```text
temp=%d：提取 temp 后面的整数
humi=%d：提取 humi 后面的整数
device_id=%31s：提取 device_id 后面的字符串，最多 31 个字符
```

为什么是 `%31s`：

```text
device_id 数组大小是 32
最多读取 31 个字符
最后预留 1 个位置给字符串结束符 '\0'
防止数组越界
```

## 8. sscanf() 返回值

代码：

```c
if (ret != 3)
{
    return -1;
}
```

解释：

```text
sscanf() 返回成功匹配的字段数量。
```

这里期望解析 3 个字段：

```text
temp
humi
device_id
```

所以：

```text
ret == 3：解析成功
ret != 3：解析失败
```

## 9. build_json() 函数

代码：

```c
int build_json(const SensorData *data, char *json_buf, int buf_size)
{
    if (data == NULL || json_buf == NULL || buf_size <= 0)
    {
        return -1;
    }

    snprintf(json_buf,
             buf_size,
             "{\"temp\":%d,\"humi\":%d,\"device_id\":\"%s\"}",
             data->temp,
             data->humi,
             data->device_id);

    return 0;
}
```

作用：

```text
把 SensorData 结构体转换成 JSON 字符串。
```

输入结构体：

```text
temp = 25
humi = 60
device_id = stm32_01
```

输出 JSON：

```json
{"temp":25,"humi":60,"device_id":"stm32_01"}
```

## 10. snprintf()

代码：

```c
snprintf(json_buf,
         buf_size,
         "{\"temp\":%d,\"humi\":%d,\"device_id\":\"%s\"}",
         data->temp,
         data->humi,
         data->device_id);
```

作用：

```text
格式化生成字符串，并限制最大写入长度。
```

为什么不用 sprintf：

```text
sprintf 不限制写入长度，容易缓冲区溢出。
snprintf 可以指定 buf_size，更安全。
```

## 11. JSON 双引号转义

在 C 字符串里生成 JSON 时，JSON 的双引号要写成：

```text
\"
```

例如：

```c
"{\"temp\":25}"
```

原因：

```text
C 字符串本身用双引号包裹。
如果 JSON 内部也要双引号，就必须转义。
```

## 12. main() 函数流程

代码流程：

```text
1. 定义模拟原始字符串 raw_data
2. 定义 SensorData sensor
3. 定义 json_buf
4. 调用 parse_sensor_data()
5. 打印解析后的结构体字段
6. 调用 build_json()
7. 打印 JSON 字符串
```

核心代码：

```c
const char *raw_data = "temp=25,humi=60,device_id=stm32_01";
SensorData sensor;
char json_buf[128];
```

说明：

```text
今天先用模拟字符串代替真实串口输入。
后续会把 raw_data 换成 Day19 read() 读取到的数据。
```

## 13. 今日命令总结

进入目录：

```bash
cd ~/linux-learning-submit/day20_sensor_parse
```

编辑代码：

```bash
nano -l sensor_parse.c
```

编译：

```bash
gcc sensor_parse.c -o sensor_parse
```

运行：

```bash
./sensor_parse
```

## 14. 今日遇到的问题

编译过程中出现过变量名或标识符相关错误。

常见原因：

```text
变量名写错
结构体变量名和代码中使用的不一致
函数参数名写错
英文符号输错
```

解决方法：

```text
根据 gcc 报错的行号定位
检查变量名是否一致
重新编译
```

最终代码已经成功运行。

## 15. 和物联网网关项目的关系

Day19 是串口读取：

```text
STM32 串口数据
↓
Linux read()
```

Day20 是数据解析：

```text
read() 读到的字符串
↓
parse_sensor_data()
↓
SensorData
↓
build_json()
↓
JSON
```

后续 Day21 gateway_v1 会整合：

```text
串口读取
↓
字符串解析
↓
JSON 生成
↓
日志保存
```

再往后会加入：

```text
MQTT publish 上传
```

## 16. 面试考点

### 问：为什么要把字符串解析成结构体？

答：

```text
结构体可以把一条传感器数据统一封装起来，方便函数传参、模块化管理和后续扩展。
```

### 问：sscanf() 的作用是什么？

答：

```text
sscanf() 用于从字符串中按照指定格式提取数据。
```

### 问：sscanf() 返回值表示什么？

答：

```text
返回成功匹配的字段数量。
```

### 问：为什么 device_id 用 %31s？

答：

```text
因为 device_id 数组大小是 32，最多读取 31 个字符，给字符串结束符 '\0' 留一个位置，防止越界。
```

### 问：snprintf() 和 sprintf() 有什么区别？

答：

```text
snprintf() 可以限制写入缓冲区的最大长度，比 sprintf() 更安全。
```

### 问：为什么 JSON 字符串里的双引号要写成 \"？

答：

```text
因为 C 字符串本身使用双引号包裹，内部 JSON 的双引号需要转义。
```

## 17. 今日总结

今天掌握了：

```text
结构体封装传感器数据
sscanf() 解析字符串
snprintf() 拼接 JSON
字符串解析错误判断
JSON 数据格式生成
```

最关键一句话：

```text
Day20 完成了从 STM32 串口字符串到 JSON 数据的转换，为后续 MQTT 上传打基础。
```
