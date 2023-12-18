# 关于orangepi的有线连接
在有网络的主机上将wifi共享，在orangepi上要配置静态ip。
修改文件 /etc/network/interfaces，加入

    auto eth0
    iface eth0 inet static
    address <这里填分配的ip>
    netmask 255.255.255.0
    gateway <这里填主机的端口ip，在主机cmd用ifconfig查>

# Game
整个游戏实体，负责控制游戏流程，切换场景等。

# Object
object 是场景中的某些贴图，包含位置信息和一个Item。

# Item
包含动画信息。

# Config
配置类，用于配置一些动画信息。

使用json配置文件，安装jsoncpp库支持json解析。
## jsoncpp
安装

    sudo apt update
    sudo apt-get install libjsoncpp-dev

