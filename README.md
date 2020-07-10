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
  
  
  ## Todo list ##
  1. Joystick setup. (2020/07/08)
  2. Generate dyros_tocabi_joystick Node that publish WalkingCommand to DaegyuLim's Custom-Controller. (2020/07/09)
  3. Control walking speed with joystick. (2020/07/10)
  4. Make launch file (mujoco + Custum-Controller + tocabi_gui + joy + dyros_tocabi_joystick)
  5. Processing Joy message
   - add message(Duration, Angle velocity, Knee Angle, Foot Height) publishing code
   - convert continuous value into discrete value
   - if value change, run publish code
   - implement moving average filter(using with filling array[100])
  6. etc., more discussion and problem will arise.
