# This project is designed for "Baidu AI" 
lastest update| 2019.6.11

![sample](/car-modules/car.jpg)

## Personal enviornment status
| 属性 | 参数 |
| --- | --- |
| Develope software| Visual Studio Code 1.35.0 |
| OS| Ubuntu 18.04 (kernel 4.18) |
| ROS version| ROS melodic |
| language| C++, Python, NodeJS |
| hardware| Arduino, jetson tx2(ARM) or Surface Book(X86) |
| sensors| Intel Realsense D435i |

## Brief introduction
- A autonomous car which can arrive at an indicated location in a scanned space.
- Transmitting signals via WI-FI.


## folder introduction.

* car
    * base-controller : 小车轮子PID控制， ROS接收cmd_vel,广播tf数据
    * car-ros : 小车RVIZ可视化
    * teleop-twist-keyboard : 终端控制小车，用于见图（SLAM）
* car-modules 
    * 小车的3D模型，班子CAD图等文件
* parking-sensor-arduino
    * 停车场传感器代码
* parking-sensor-web
    * 主机接收传感器信息，处理，向小车指令的代码
* wifi-communication-nodejs
    * 小车接接受主机指令并返广播到ROS节点
