#include "../include/dyros_tocabi_joystick/tocabi_joystick.h"

TocabiJoystick::TocabiJoystick()
{
    joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &TocabiJoystick::joyCallback, this);
    
    joy_command_pub_ = nh_.advertise<std_msgs::Float32>("/joy_topic", 1);
    // joy_command_pub_ = nh_.advertise<geometry_msgs::Twist>("/joy_topic", 1);
    
    walkingspeed_pub = nh_.advertise<std_msgs::Float32 >("/tocabi/walkingspeedcommand", 100);
    walkingduration_pub = nh_.advertise<std_msgs::Float32>("/tocabi/walkingdurationcommand", 100);
    walkingangvel_pub = nh_.advertise<std_msgs::Float32>("/tocabi/walkingangvelcommand", 100);
    kneetargetangle_pub = nh_.advertise<std_msgs::Float32>("/tocabi/kneetargetanglecommand", 100);
    footheight_pub = nh_.advertise<std_msgs::Float32>("/tocabi/footheightcommand", 100);

    speed_value = 0;
}

void TocabiJoystick::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
    // // std_msgs::Float64 float64;
    // // float64 = joy->axes[0]; 
    // // joy_command_pub_.publish(float64);
    // // std::cout << float64 << std::endl;

    // geometry_msgs::Twist twist;

    // // float value_round = 0.0;
    // // value_round = joy->axes[0];
    // // value_round += + 0.005;
    // // value_round = (int)(value_round*100);
    // // value_round = value_round/100;

    // twist.linear.x = roundf((joy->axes[0])*100);
    // twist.linear.x = (int)(twist.linear.x);
    // twist.linear.x /= 100;

    // // twist.linear.y = joy->axes[1];
    // joy_command_pub_.publish(twist);

    // //반올림, 이전 메세지 저장 구현이 어렵다 ㅠ

    // std_msgs::Float32 joy_axis_1;
    // std_msgs::Float32 joy_axis_2;
    // std_msgs::Float32 joy_axis_5;
    // joy_axis_1.data = 0;
    // joy_axis_1.data = roundf((joy->axes[1]*100));
    // joy_axis_1.data = (int)(joy_axis_1.data);
    // joy_axis_1.data /= 100;

    // joy_axis_2.data = 0;
    // joy_axis_2.data = roundf((joy->axes[2]*100));
    // joy_axis_2.data = (int)(joy_axis_2.data);
    // joy_axis_2.data /= 100;

    // joy_axis_5.data = 0;
    // joy_axis_5.data = roundf((joy->axes[5]*100));
    // joy_axis_5.data = (int)(joy_axis_5.data);
    // joy_axis_5.data /= 100;
    
    // joy_command_pub_.publish(joy_axis_1);
    
    // // if(joy_axis_1 != walkingspeed_msg_pre.data){
        
    // //     walkingspeedcb(joy_axis_1);
    // //     walkingspeed_msg_pre.data = walkingspeed_msg.data;
    // // }

    // if(joy->axes[7] == 1.0 && speed_value < 10 ){
    //     walk_cmd_ = true;
    //     speed_value++;
    // }
    // else if(joy->axes[7] == -1.0 && speed_value > -10){
    //     walk_cmd_ = true;
    //     speed_value--;
    // }
    // else{
    //     walk_cmd_ = false;
    // }
    
    // if(walk_cmd_){
    //     if(walk_cmd_ != walk_cmd_pre_)
    //         walkingspeedcb(speed_value);
    // }
    // walk_cmd_pre_ = walk_cmd_;

    speed_value = round(joy->axes[1]*100); //조이스틱 값을 100배하여 반올림을해 정수형으로 만들어줌
    if(speed_value >= -4 && speed_value <= 6){
        walkingspeed_msg.data = speed_value/100;
        walkingspeed_pub.publish(walkingspeed_msg);
    }
    

}

// void TocabiJoystick::walkingspeedcb(double value)
// {
//     double max_speed = 0.6;
//     double min_speed = 0.4;
//     double scale = value;
    
//     if(scale >= 0){
//         walkingspeed_msg.data = scale*max_speed;
//     }
//     else{
//         walkingspeed_msg.data = scale*min_speed;
//     }
//     joy_command_pub_.publish(walkingspeed_msg);
//     walkingspeed_pub.publish(walkingspeed_msg);
// }

void TocabiJoystick::walkingspeedcb(double value)
{
    double max_speed = 0.6;
    double min_speed = 0.4;
    double scale = value;
    
    if(scale >= 0){
        walkingspeed_msg.data = scale*max_speed/10;
    }
    else{
        walkingspeed_msg.data = scale*min_speed/10;
    }
    joy_command_pub_.publish(walkingspeed_msg);
    walkingspeed_pub.publish(walkingspeed_msg);
}

void TocabiJoystick::walkingdurationcb(int value)
{
    double max_duration = 1;
    double min_duration = 0.2;
    double scale = value;

    walkingduration_msg.data = scale/100*(max_duration - min_duration) + min_duration;
    walkingduration_pub.publish(walkingduration_msg);
}

void TocabiJoystick::walkingangvelcb(int value)
{
    double max_angvel = 1;
    double min_angvel = -1;
    double scale = value;

    walkingangvel_msg.data = scale/100*(max_angvel - min_angvel) + min_angvel;
    walkingangvel_pub.publish(walkingangvel_msg);
}

void TocabiJoystick::kneetargetanglecb(int value)
{
    double max_knee = M_PI/2;
    double min_knee = 0;
    double scale = value;

    kneetargetangle_msg.data = scale/100*(max_knee - min_knee) + min_knee;
    kneetargetangle_pub.publish(kneetargetangle_msg);
}

void TocabiJoystick::footheightcb(int value)
{
    double max_footz = 0.1;
    double min_footz = 0.005;
    double scale = value;

    footheight_msg.data = scale/100*(max_footz - min_footz) + min_footz;
    footheight_pub.publish(footheight_msg);
}




int main(int argc, char** argv)
{
    std::cout << "Started~~"<<std::endl;
    ros::init(argc, argv, "dyros_tocabi_joystick");
    TocabiJoystick tocabijoystick;
    std::cout << "Dyros Tocabi Joystick Controller Started"<<std::endl;
    while(ros::ok())
    {
        ros::spinOnce();
    }
}