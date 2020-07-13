# DYROS TOCABI JOYSTICK!!

* This is a DYROS Tocabi Humanoid's joystick controller Repository
* using Microsoft Xbox 360 Wired Controller for Linux
(http://wiki.ros.org/joy#Microsoft_Xbox_360_Wired_Controller_for_Linux)
* in Ubuntu 16.04.6 LTS with ROS
<br></br>
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

* test joystick
```sh
sudo jstest /dev/input/jsX
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

![xbox 360 wire joy value key](https://user-images.githubusercontent.com/68094299/87122977-d7e77100-c2c0-11ea-9015-16e452b2c174.png)
<br></br>
## How do I run dyros_tocabi_joystick ? ##
* Launch DaegyuLim's Custom-Controller (https://github.com/DaegyuLim/Custom-Controller)
* Enable mujoco
* in tocabi_gui > COMMAND > Mode > Select "13.CustomController 4" > Click "SEND" <br>

~~run Joystick<br>
rosrun dyros_tocabi_joystick dyros_tocabi_joystick<br>
run Joy Node<br>
rosrun joy joy_node~~ (launch file made)

* roslaunch
```ch
roslaunch dyros_tocabi_joystick joystick.launch
```
  also, you can check topic with `rostopic list` and `rostopic echo /tocabi/...`
  <br></br>
  ## Todo list ##
  1. ~~Joystick setup.~~ (2020/07/08)
  2. ~~Generate dyros_tocabi_joystick Node that publish WalkingCommand to DaegyuLim's Custom-Controller.~~ (2020/07/09)
  3. ~~Control walking speed with joystick.~~ (2020/07/10)
  4. ~~Make launch file (joy + dyros_tocabi_joystick)~~ (2020/07/13)
  5. Processing Joy message <br>
&nbsp; ~~add message(Angle velocity) publishing code~~ (2020/07/13) <br>
&nbsp; ~~implement moving average filter(with filling array[100] <= using Circular Queue)~~(2020/07/13) <br>
&nbsp; _add message(Duration,  Knee Angle, Foot Height) publishing code <br>
&nbsp; convert continuous value into discrete value <br>
&nbsp; joystick key setting(need to discuss)_(2020/07/13)
  6. Make Integrate launch file {(mujoco + Custum-Controller + tocabi_gui} + {joy + dyros_tocabi_joystick})
  7. Processing Publish Timing
&nbsp; _if value change, run publish code <br>
&nbsp; Queues in moving average filters must be updated at the same time interval_<br>
  8. etc., more discussion and problem will arise.
