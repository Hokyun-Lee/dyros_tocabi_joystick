#include "../include/dyros_tocabi_joystick/tocabi_joystick.h"

TocabiJoystick::TocabiJoystick(int size_) : speed_MAF(size_), angvel_MAF(size_)
{
    joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &TocabiJoystick::joyCallback, this);
    
    joy_command_pub_ = nh_.advertise<std_msgs::Float32>("/joy_topic", 1);
    // joy_command_pub_ = nh_.advertise<geometry_msgs::Twist>("/joy_topic", 1);
    
    walkingspeed_pub = nh_.advertise<std_msgs::Float32 >("/tocabi/walkingspeedcommand", 100);
    walkingduration_pub = nh_.advertise<std_msgs::Float32>("/tocabi/walkingdurationcommand", 100);
    walkingangvel_pub = nh_.advertise<std_msgs::Float32>("/tocabi/walkingangvelcommand", 100);
    kneetargetangle_pub = nh_.advertise<std_msgs::Float32>("/tocabi/kneetargetanglecommand", 100);
    footheight_pub = nh_.advertise<std_msgs::Float32>("/tocabi/footheightcommand", 100);
    // speed_value = 0;

    walkingduration_msg.data = 0.6;
    kneetargetangle_msg.data = 0.1;
    footheight_msg.data = 0.05;
    mode = 0; //default : 0
}

void TocabiJoystick::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
    if(joy->buttons[0]){
        mode = 0;
    }
    else if(joy->buttons[1]){
        mode = 1;
    }
    else if(joy->buttons[2]){
        mode = 2;
    }
    else if(joy->buttons[3]){
        mode = 3;
    }
    std::cout << "Now, Interface Mode : " << mode << std::endl << std::endl;

    if(joy->buttons[8] == 1){ //msg initialize
        walkingduration_msg.data = 0.6;
        kneetargetangle_msg.data = 0.1;
        footheight_msg.data = 0.05;
        
        walkingduration_pub.publish(walkingduration_msg);
        kneetargetangle_pub.publish(kneetargetangle_msg);
        footheight_pub.publish(footheight_msg);
    }

    if(joy->buttons[7] == 1 && walkingduration_msg.data < max_duration){
        walkingduration_msg.data += (max_duration)/40;
        if(walkingduration_msg.data > max_duration)
            walkingduration_msg.data = max_duration;
        walkingduration_pub.publish(walkingduration_msg);
    }
    else if(joy->buttons[6] == 1 && walkingduration_msg.data > min_duration){
        walkingduration_msg.data -= (max_duration)/40;
        if(walkingduration_msg.data < min_duration)
            walkingduration_msg.data = min_duration;
        walkingduration_pub.publish(walkingduration_msg);
    }

    std::cout << "Walking Duration: " << walkingduration_msg.data << std::endl;

    if(joy->axes[6] != 0){
        if(joy->axes[6] == -1 && kneetargetangle_msg.data < max_kneeangle){
            kneetargetangle_msg.data += (max_kneeangle)/45; // (pi/2)/45 = 2 deg
            if(kneetargetangle_msg.data > max_kneeangle)
                kneetargetangle_msg.data = max_kneeangle;
        }
        else if(joy->axes[6] == 1 && kneetargetangle_msg.data > 0){
            kneetargetangle_msg.data -= (max_kneeangle)/45;
            if(kneetargetangle_msg.data < 0)
                kneetargetangle_msg.data = 0;
        }
        kneetargetangle_pub.publish(kneetargetangle_msg);
    }
    
    std::cout << "Knee Target angle[rad]: " << kneetargetangle_msg.data << std::endl;

    if(joy->axes[7] != 0){
        if(joy->axes[7] == 1 && footheight_msg.data < max_footheight){
            footheight_msg.data += (max_footheight)/20;
            if(footheight_msg.data > max_footheight)
                footheight_msg.data = max_footheight;
        }
        else if(joy->axes[7] == -1 && footheight_msg.data > min_footheight){
            footheight_msg.data -= (max_footheight)/20;
            if(footheight_msg.data < min_footheight)
                footheight_msg.data = min_footheight;
        }
        footheight_pub.publish(footheight_msg);
    }
    
    std::cout << "Foot Height[cm]: " << footheight_msg.data << std::endl << std::endl;

    if(mode == 0){
        walkingspeed_msg.data = (joy->axes[1])*speed_scale;
        if(walkingspeed_msg.data > max_speed){
            walkingspeed_msg.data = max_speed;
        }
        else if(walkingspeed_msg.data < min_speed){
            walkingspeed_msg.data = min_speed;
        }
        speed_MAF.enqueue(walkingspeed_msg.data);
        float speed_avg = 0;
        std::cout << "speed queue : ";
        std::cout.precision(5);
        for(int i = 0; i < speed_MAF.capacity; i++){
            if(i % 10 == 0)
                std::cout << std::endl;
            std::cout << speed_MAF.cq[i] << " ";
            speed_avg += speed_MAF.cq[i];
        }
        std::cout << std::endl << std::endl;
        walkingspeed_msg.data = speed_avg/speed_MAF.capacity;
        walkingspeed_pub.publish(walkingspeed_msg);

        walkingangvel_msg.data = -1 * joy->axes[3];

        if(walkingangvel_msg.data > max_angvel){
            walkingangvel_msg.data = max_angvel;
        }
        else if(walkingangvel_msg.data < min_angvel){
            walkingangvel_msg.data = min_angvel;
        }
        angvel_MAF.enqueue(walkingangvel_msg.data);
        float angvel_avg = 0;
        std::cout << "angvel queue : ";
        std::cout.precision(5);
        for(int i = 0; i < angvel_MAF.capacity; i++){
            if(i % 10 == 0)
                std::cout << std::endl;
            std::cout << angvel_MAF.cq[i] << " ";
            angvel_avg += angvel_MAF.cq[i];
        }
        std::cout << std::endl << std::endl;
        walkingangvel_msg.data = angvel_avg/angvel_MAF.capacity;
        walkingangvel_pub.publish(walkingangvel_msg);

        //walking speed와 angle velocity를 동시에 줄때 한계점(넘어지는 지점)이 존재함 ((처리 필요))
        std::cout << "walking speed: " << walkingspeed_msg.data << std::endl;
        std::cout << "angle velocity: " << walkingangvel_msg.data << std::endl << std::endl;
    }
    else if(mode == 1){
        walkingspeed_msg.data = (joy->axes[1])*speed_scale;
        if(walkingspeed_msg.data > max_speed){
            walkingspeed_msg.data = max_speed;
        }
        else if(walkingspeed_msg.data < min_speed){
            walkingspeed_msg.data = min_speed;
        }
        speed_MAF.enqueue(walkingspeed_msg.data);
        float speed_avg = 0;
        std::cout << "speed queue : ";
        std::cout.precision(5);
        for(int i = 0; i < speed_MAF.capacity; i++){
            if(i % 10 == 0)
                std::cout << std::endl;
            std::cout << speed_MAF.cq[i] << " ";
            speed_avg += speed_MAF.cq[i];
        }
        std::cout << std::endl << std::endl;
        walkingspeed_msg.data = speed_avg/speed_MAF.capacity;
        walkingspeed_pub.publish(walkingspeed_msg);

        double axes_2 = 0;
        double axes_5 = 0;
        axes_2 = (joy->axes[2] + 1)/2;
        axes_5 = (joy->axes[5] + 1)/2;

        walkingangvel_msg.data = (axes_2 - axes_5)*angvel_scale;
        // double max_angvel = 0.5;
        // double min_angvel = -0.5;
        // if(walkingangvel_msg.data > max_angvel){
        //     walkingangvel_msg.data = max_angvel;
        // }
        // else if(walkingangvel_msg.data < min_angvel){
        //     walkingangvel_msg.data = min_angvel;
        // }
        angvel_MAF.enqueue(walkingangvel_msg.data);
        float angvel_avg = 0;
        std::cout << "angvel queue : ";
        std::cout.precision(5);
        for(int i = 0; i < angvel_MAF.capacity; i++){
            if(i % 10 == 0)
                std::cout << std::endl;
            std::cout << angvel_MAF.cq[i] << " ";
            angvel_avg += angvel_MAF.cq[i];
        }
        std::cout << std::endl << std::endl;
        walkingangvel_msg.data = angvel_avg/angvel_MAF.capacity;
        walkingangvel_pub.publish(walkingangvel_msg);

        //walking speed와 angle velocity를 동시에 줄때 한계점(넘어지는 지점)이 존재함 ((처리 필요))
        std::cout << "walking speed: " << walkingspeed_msg.data << std::endl;
        std::cout << "angle velocity: " << walkingangvel_msg.data << std::endl << std::endl;
    }
    else if(mode == 2){
        double axes_2 = 0;
        double axes_5 = 0;
        axes_2 = (joy->axes[2] + 1)/2; //scaling [-1,1] into [0,1]
        axes_5 = (joy->axes[5] + 1)/2;

        walkingspeed_msg.data = 0.6 - sqrt(axes_2*axes_2 + axes_5*axes_5)/sqrt(2)*speed_scale;
        if(walkingspeed_msg.data > max_speed){
            walkingspeed_msg.data = max_speed;
        }
        else if(walkingspeed_msg.data < 0){
            walkingspeed_msg.data = 0;
        }
        speed_MAF.enqueue(walkingspeed_msg.data);
        float speed_avg = 0;
        std::cout << "speed queue : ";
        std::cout.precision(5);
        for(int i = 0; i < speed_MAF.capacity; i++){
            if(i % 10 == 0)
                std::cout << std::endl;
            std::cout << speed_MAF.cq[i] << " ";
            speed_avg += speed_MAF.cq[i];
        }
        std::cout << std::endl << std::endl;
        walkingspeed_msg.data = speed_avg/speed_MAF.capacity;
        walkingspeed_pub.publish(walkingspeed_msg);

        walkingangvel_msg.data = (axes_2 - axes_5)*angvel_scale;
        angvel_MAF.enqueue(walkingangvel_msg.data);
        float angvel_avg = 0;
        std::cout << "angvel queue : ";
        std::cout.precision(5);
        for(int i = 0; i < angvel_MAF.capacity; i++){
            if(i % 10 == 0)
                std::cout << std::endl;
            std::cout << angvel_MAF.cq[i] << " ";
            angvel_avg += angvel_MAF.cq[i];
        }
        std::cout << std::endl << std::endl;
        walkingangvel_msg.data = angvel_avg/angvel_MAF.capacity;
        walkingangvel_pub.publish(walkingangvel_msg);

        //walking speed와 angle velocity를 동시에 줄때 한계점(넘어지는 지점)이 존재함 ((처리 필요))
        std::cout << "walking speed: " << walkingspeed_msg.data << std::endl;
        std::cout << "angle velocity: " << walkingangvel_msg.data << std::endl << std::endl;
    }
    else if(mode == 3){
        double axes_2 = 0;
        double axes_5 = 0;
        axes_2 = (joy->axes[2] + 1)/2; //scaling [-1,1] into [0,1]
        axes_5 = (joy->axes[5] + 1)/2;
        double speed_scale_backward = 0.4;
        walkingspeed_msg.data = -0.4 + sqrt(axes_2*axes_2 + axes_5*axes_5)/sqrt(2)*speed_scale_backward;
        if(walkingspeed_msg.data > 0){
            walkingspeed_msg.data = 0;
        }
        else if(walkingspeed_msg.data < min_speed){
            walkingspeed_msg.data = min_speed;
        }
        speed_MAF.enqueue(walkingspeed_msg.data);
        float speed_avg = 0;
        std::cout << "speed queue : ";
        std::cout.precision(5);
        for(int i = 0; i < speed_MAF.capacity; i++){
            if(i % 10 == 0)
                std::cout << std::endl;
            std::cout << speed_MAF.cq[i] << " ";
            speed_avg += speed_MAF.cq[i];
        }
        std::cout << std::endl << std::endl;
        walkingspeed_msg.data = speed_avg/speed_MAF.capacity;
        walkingspeed_pub.publish(walkingspeed_msg);

        walkingangvel_msg.data = (axes_2 - axes_5)*angvel_scale;
        angvel_MAF.enqueue(walkingangvel_msg.data);
        float angvel_avg = 0;
        std::cout << "angvel queue : ";
        std::cout.precision(5);
        for(int i = 0; i < angvel_MAF.capacity; i++){
            if(i % 10 == 0)
                std::cout << std::endl;
            std::cout << angvel_MAF.cq[i] << " ";
            angvel_avg += angvel_MAF.cq[i];
        }
        std::cout << std::endl << std::endl;
        walkingangvel_msg.data = angvel_avg/angvel_MAF.capacity;
        walkingangvel_pub.publish(walkingangvel_msg);

        //walking speed와 angle velocity를 동시에 줄때 한계점(넘어지는 지점)이 존재함 ((처리 필요))
        std::cout << "walking speed: " << walkingspeed_msg.data << std::endl;
        std::cout << "angle velocity: " << walkingangvel_msg.data << std::endl << std::endl;
    }
    std::cout << "------------------------------------" << std::endl << std::endl;
}

void TocabiJoystick::modechange(){
    
}

void TocabiJoystick::walkingspeedcb(double value)
{
    double scale = value;
    
    if(scale >= 0){
        walkingspeed_msg.data = scale*max_speed;
    }
    else{
        walkingspeed_msg.data = scale*min_speed;
    }
    joy_command_pub_.publish(walkingspeed_msg);
    walkingspeed_pub.publish(walkingspeed_msg);
}

void TocabiJoystick::walkingdurationcb(int value)
{

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

MovAvgFilter::MovAvgFilter(int size):front(0), back(0), capacity(size)
{
    cq = new double[size + 1]; //더미노드까지 포함하여 포인터를 생성함
    for(int i = 0; i < size; i++){
        enqueue(0);
    }
}

void MovAvgFilter::enqueue(double data)
{
    int pos;
    // back이 마지막 인덱스에 채우는 경우
    // back은 계속 순환하니까 0으로 초기화
    // 마지막엔 pos로 담는다.
    if (back == capacity-1){
        pos = back;
        cq[pos] = data;
        back = 0;
    }
    else
        cq[back++] = data;
}

int MovAvgFilter::dequeue()
{
    int pos = front;
    // front가 마지막 인덱스의 데이터를 제거하는 경우
    // front도 순환하므로 0으로 초기화
    // 리턴은 pos로 한다.
    if (front == capacity - 1) front = 0;
    else front++;
    return cq[pos];
}

int MovAvgFilter::getFront() const
{
    return cq[front];
}
 
int MovAvgFilter::getBack() const
{
    // back은 enqueue에서 증가하므로 -1 해서 접근해야 함
    return cq[back-1];
}
 
bool MovAvgFilter::isEmpty() const
{
    return front == back;
}

int MovAvgFilter::getSize() const
{
    // front가 back보다 작거나 같은경우
    if (front <= back) return back - front;
    // front가 back보다 큰 경우는 
    // back이 다시 돌아오고 front가 진행하는 경우이다.
    else if (front > back) return capacity - front + back;
}

bool MovAvgFilter::isFull() const
{
    // front가 back보다 작으면 back이 더미 바로 전에 있어야 함
    if (front <= back) return back - front == capacity - 1;
    // front가 back보다 크면 back이 front 바로 전에 있어야 함
    else return back + 1 == front;
}


int main(int argc, char** argv)
{
    std::cout << "Started~~"<<std::endl;
    ros::init(argc, argv, "dyros_tocabi_joystick");
    int capacity = 50;
    TocabiJoystick tocabijoystick(capacity);
    std::cout << "Dyros Tocabi Joystick Controller Started"<<std::endl;
    ros::Rate rate(20);
    while(ros::ok())
    {
        ros::spinOnce();
        rate.sleep();
    }
}