#ifndef _DYROS_JET_JOYSTICK_H
#define _DYROS_JET_JOYSTICK_H
// ROS LIBRARY
#include <ros/ros.h>
// ROS MESSAGE
#include <std_msgs/Float32.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

#include <cmath>
#include <queue>

class MovAvgFilter //CircularQueue
{
public:
     MovAvgFilter(int size);
     void enqueue(double data);
     int dequeue();
     int getFront() const;
     int getBack() const;
     int getSize() const;
     bool isEmpty() const;
     bool isFull() const;

     int capacity;
     double* cq;
private:
     int front;
     int back;
};


class TocabiJoystick
{
public:
     TocabiJoystick(int size_);

     MovAvgFilter speed_MAF;
     MovAvgFilter angvel_MAF;

     virtual void walkingspeedcb(double value);
     virtual void walkingdurationcb(int value);
     virtual void walkingangvelcb(int value);
     virtual void kneetargetanglecb(int value);
     virtual void footheightcb(int value);

     void modechange();
     
private:
     void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

     ros::NodeHandle nh_;
     ros::Publisher joy_command_pub_;
     ros::Subscriber joy_sub_;
     ros::Publisher walkingspeed_pub;
     std_msgs::Float32 walkingspeed_msg;
     std_msgs::Float32 walkingspeed_msg_pre;
     ros::Publisher walkingduration_pub;
     std_msgs::Float32 walkingduration_msg;
     ros::Publisher walkingangvel_pub;
     std_msgs::Float32 walkingangvel_msg;
     ros::Publisher kneetargetangle_pub;
     std_msgs::Float32 kneetargetangle_msg;
     ros::Publisher footheight_pub;
     std_msgs::Float32 footheight_msg;

     int mode;

     bool walk_cmd_;
     bool walk_cmd_pre_;

     double speed_value;
     double angle_value;

     double max_speed = 0.6;
     double min_speed = -0.4;

     double max_angvel = 0.5;
     double min_angvel = -0.5;

     double max_kneeangle = 3.1416/2;

     double max_duration = 1.0;
     double min_duration = 0.2;

     double max_footheight = 0.1;
     double min_footheight = 0.01;

     double speed_scale = 0.6;
     double angvel_scale = 0.5;


};
 

int main(int argc, char** argv);

#endif
