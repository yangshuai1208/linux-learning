# Day22 笔记：gateway_loop 循环采集与日志保存

## 1. 今日完成内容

今天完成了 gateway_loop 循环版本。

完成内容：

```text
创建 day22_gateway_loop
编写 gateway_loop.c
模拟周期性传感器数据
循环解析数据
生成 JSON
写入 sensor_log.txt
使用 tail -f 实时查看日志
```

实验现象：

```text
sensor_log.txt 中持续追加 JSON 数据
每隔约 2 秒写入一条日志
```

## 2. Day21 和 Day22 的区别

Day21：

```text
只处理一次数据
```

Day22：

```text
循环处理多条数据
```

真实网关程序不能只运行一次，而应该长期运行，不断处理设备上报的数据。

所以 Day22 的核心思想是：

```text
网关程序需要具备持续运行能力。
```

## 3. raw_data_list 模拟数据

代码示例：

```c
const char *raw_data_list[] =
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
模拟 STM32 周期性发送的多条传感器数据。
```

后续真实串口接入后，这部分会替换成：

```c
read(fd, recv_buf, sizeof(recv_buf) - 1);
```

## 4. 计算数组元素个数

代码：

```c
int data_count = sizeof(raw_data_list) / sizeof(raw_data_list[0]);
```

作用：

```text
自动计算数组中有多少条模拟数据。
```

解释：

```text
sizeof(raw_data_list)：整个数组大小
sizeof(raw_data_list[0])：一个元素大小
两者相除就是数组元素个数
```

这样比手写数字更安全。

## 5. for 循环

代码：

```c
for (i = 0; i < data_count; i++)
{
    ...
}
```

作用：

```text
依次处理 raw_data_list 中的每一条模拟数据。
```

每轮循环做：

```text
解析字符串
生成 JSON
写入日志
等待 2 秒
```

## 6. continue

代码：

```c
if (ret != 0)
{
    printf("parse sensor data failed\n");
    continue;
}
```

作用：

```text
当前这条数据处理失败时，跳过本次循环，继续处理下一条数据。
```

为什么不用 return？

```text
真实网关不能因为一条坏数据就退出。
使用 continue 可以提高程序健壮性。
```

## 7. sleep()

代码：

```c
sleep(2);
```

作用：

```text
程序暂停 2 秒，模拟 STM32 周期性上报数据。
```

需要头文件：

```c
#include <unistd.h>
```

## 8. write_log()

作用：

```text
把每一条 JSON 数据追加写入 sensor_log.txt。
```

核心代码：

```c
fp = fopen("sensor_log.txt", "a");
fprintf(fp, "[%s] %s\n", time_buf, json_data);
fclose(fp);
```

其中 `"a"` 表示追加模式：

```text
不会覆盖旧日志
会在文件末尾继续写入
文件不存在时会自动创建
```

## 9. tail -f

命令：

```bash
tail -f sensor_log.txt
```

作用：

```text
实时查看日志文件末尾新增内容。
```

这在 Linux 开发和服务器调试中非常常用。

停止：

```text
Ctrl + C
```

## 10. 今日命令总结

进入目录：

```bash
cd ~/linux-learning-submit/day22_gateway_loop
```

编辑代码：

```bash
nano -l gateway_loop.c
```

编译：

```bash
gcc gateway_loop.c -o gateway_loop
```

运行：

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

## 11. 今日实验结论

今天实验成功的标志：

```text
程序可以循环运行
日志文件中出现多条带时间戳的 JSON 数据
tail -f 可以实时看到日志变化
```

当前截图中已经看到多条日志连续写入，说明循环写日志功能成功。

## 12. 和物联网网关项目的关系

当前 Day22：

```text
模拟数据
↓
循环处理
↓
写日志
```

后续真实网关：

```text
STM32 串口数据
↓
read()
↓
parse_sensor_data()
↓
build_json()
↓
write_log()
↓
mosquitto_publish()
```

Day22 解决的是：

```text
网关程序持续运行和持续处理数据的问题。
```

## 13. 面试考点

### 问：为什么网关程序要循环运行？

答：

```text
因为物联网设备会周期性上报数据，网关需要长期运行并持续处理数据，而不是只处理一次后退出。
```

### 问：continue 的作用是什么？

答：

```text
continue 用于跳过本次循环，继续下一轮循环。在网关程序中，一条数据异常时不应该直接退出程序。
```

### 问：tail -f 有什么作用？

答：

```text
tail -f 可以实时查看日志文件新增内容，常用于 Linux 程序调试和服务运行观察。
```

### 问：日志为什么使用追加模式？

答：

```text
追加模式可以保留历史数据，不会覆盖旧日志，适合记录连续产生的传感器数据。
```

## 14. 今日总结

今天完成了 gateway_loop 循环版本。

掌握内容：

```text
数组模拟多条数据
for 循环处理
continue 异常跳过
sleep 模拟周期上报
日志持续追加
tail -f 实时查看日志
```

最关键一句话：

```text
Day22 让网关程序从“一次性处理”升级为“循环持续处理”，更接近真实物联网网关。
```
