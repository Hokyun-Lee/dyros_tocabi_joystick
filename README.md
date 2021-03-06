# DYROS TOCABI JOYSTICK!!!

* This is a DYROS Tocabi Humanoid's joystick control interface Repository
* using Microsoft Xbox 360 Wired Controller for Linux
(http://wiki.ros.org/joy#Microsoft_Xbox_360_Wired_Controller_for_Linux)
* in Ubuntu 16.04.6 LTS with ROS <br>
* using Joy(ROS driver for a generic Linux joystick).
<p align="center"><img src="https://user-images.githubusercontent.com/68094299/89493237-ac3ba600-d7ee-11ea-98a2-6833478e9727.png" height='300'> <img src="https://user-images.githubusercontent.com/68094299/87521802-d0103e00-c6bf-11ea-868e-d85cf6993647.png" height="300">

## Configuring Joy ##
1. Joy 패키지 설치
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
rostopic echo /joy
```
<p align="center"><img src="https://user-images.githubusercontent.com/68094299/87122977-d7e77100-c2c0-11ea-9015-16e452b2c174.png" width="635" height="474">
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
  
## Mode Explain ##
### Mode Change ###
&nbsp; Mode 0 : bottons\[0] <br>
&nbsp; Mode 1 : bottons\[1] <br>
&nbsp; Mode 2 : bottons\[2] <br>
&nbsp; Mode 3 : bottons\[3] <br>
### Mode description ###
- **Mode 0**    
&nbsp; speed : axes\[1]    
&nbsp; angle value : axes\[4]     
- **Mode 1**    
&nbsp; speed : axes\[1]    
&nbsp; angle value(left) : axes\[2]    
&nbsp; angle value(right) : axes\[5]    
- **Mode 2(forward)**    
&nbsp; speed : sqrt{angle value(left)^2 + angle value(right)^2}/sqrt{2}    
&nbsp; angle value(left) : axes\[2]    
&nbsp; angle value(right) : axes\[5]    
- **Mode 3(backward)**    
&nbsp; speed : - sqrt{angle value(left)^2 + angle value(right)^2}/sqrt{2}    
&nbsp; angle value(left) : axes\[2]    
&nbsp; angle value(right) : axes\[5] <br></br>

 ## Todo list ##
  1. ~~Joystick setup.~~ (2020/07/08)
  2. ~~Generate dyros_tocabi_joystick Node that publish WalkingCommand to DaegyuLim's Custom-Controller.~~ (2020/07/09)
  3. ~~Control walking speed with joystick.~~ (2020/07/10)
  4. ~~Make launch file (joy + dyros_tocabi_joystick)~~ (2020/07/13)
  5. Processing Joy message <br>
&nbsp; ~~add message(Angle velocity) publishing code~~ (2020/07/13) <br>
&nbsp; ~~implement moving average filter(with filling array[100] <= using Circular Queue)~~(2020/07/13) <br>
&nbsp; ~~add message(Duration, Knee Angle, Foot Height) publishing code~~ (2020/07/15) <br>
&nbsp; ~~convert continuous value into discrete value~~ (not necessary) <br>
&nbsp; ~~joystick key setting(need to discuss) (need to explain)~~<br>
  6. Make Integrate launch file {(mujoco + Custum-Controller + tocabi_gui) + (joy + dyros_tocabi_joystick)}
  7. ~~Processing Publish Timing~~ (2020/07/14) <br>
&nbsp; ~~if value change, run publish code~~  <- auto repeat in 20 Hz <br>
&nbsp; ~~Queues in moving average filters must be updated at the same time interval~~  <- coalesce interval has added(0.2s) <br>
  8. ~~Queue size can be change~~ (2020/07/15)
  9. ~~Various Control Mode~~ {Mode 0(button 0) ~ Mode 3(button 3)} (2020/07/15)
