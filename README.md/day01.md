cd ~/linux-learning/day01_hello_linux
nano README.md

写入：

# Day01 Hello Linux

## 今日目标

- 安装并启动 Ubuntu / WSL
- 安装 gcc 编译环境
- 编写并运行第一个 Linux C 程序

## 编译命令

```bash
gcc hello_linux.c -o hello_linux
运行命令
./hello_linux
实验现象

终端输出：

Hello, Embedded Linux!
今日总结

今天完成了嵌入式 Linux 第一天环境打通，确认 WSL Ubuntu 可以正常使用 gcc 编译并运行 C 程序。

后续的串口通信、TCP 通信、MQTT 发布程序，都会基于当前 Linux C 开发环境继续完成。


保存：

```text
Ctrl + O
回车
Ctrl + X
