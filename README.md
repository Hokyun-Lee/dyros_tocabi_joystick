# dyros_tocabi_joystick

* This is a DYROS Tocabi Humanoid's joystick controller Repository
* using Microsoft Xbox 360 Wired Controller for Linux
(http://wiki.ros.org/joy#Microsoft_Xbox_360_Wired_Controller_for_Linux)
* in Ubuntu 16.04.6 LTS


## Configuring Joystick ##
1. 조이스틱 패키지 설치
```sh
sudo apt-get install ros-kinetic-joy
```

2. 조이스틱 인식 확인
* Connect your joystick to your computer.
```sh
ls /dev/input/
```

* listing the permissions of the joystick.(in my case, js0)
```sh
ls -1 /dev/input/jsX
sudo chmod a+rw /dev/input/jsX
```

* Starting the joy Node
```sh
roscore
rosparam set joy_node/dev "/dev/input/jsX"
rosrun joy joy_node
```
* Check published topic
```sh
rostopic echo joy
```


## How do I run dyros_tocabi_joystick ? ##
* Launch DaegyuLim's Custom-Controller (https://github.com/DaegyuLim/Custom-Controller)
* Enable mujoco
* in tocabi_gui > COMMAND > Mode > Select '13.CustomController 4 > Click 'SEND'
* run Joystick
```ch
rosrun dyros_tocabi_joystick dyros_tocabi_joystick
```
* run Joy Node
```ch
rosrun joy joy_node
```
  also, you can check topic with `rostopic list` and `rostopic echo /tocabi/...`
