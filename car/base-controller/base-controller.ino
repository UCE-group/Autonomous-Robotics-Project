/*
@作者 : lifuguan, Wacokgde
@时间 : 2019.6.13
@状态 : 测试中
@作用 : 以后再写。。。
 */


#include "base-controller.h"
#include <ros.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>

ros::NodeHandle  nh;

/*各个坐标系之间的关系 ： map --> odom --> base_link
@map : 是虚拟世界中的固定frame
@odom : 是一个很好的固定世界参照frame
@base_link : 参照系紧紧粘在移动机器人基座上的任何一个位置和角度
 */
geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;


//初始位置（绝对矢量位置）
double x = 0.0;
double y = 0.0;
double theta = 0.00;

//速度（矢量）给出
double linear = 0.00;
double angular = 0.00;

void motor_cb(const geometry_msgs::Twist& vel)
{
  linear = vel.linear.x /* * 100*/ ; //ROS中的单位是m/s;这里换算成cm的单位(有待考证)
  angular = vel.angular.z;
}
ros::Subscriber<geometry_msgs::Twist> sub("/cmd_vel", motor_cb);

char base_link[] = "/base_link";
char odom[] = "/odom";

void setup()
{
  nh.initNode();
  broadcaster.init(nh);
  nh.subscribe(sub);

  
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(IN1_AL, OUTPUT);
  pinMode(IN2_AL, OUTPUT);
  pinMode(IN3_AR, OUTPUT);
  pinMode(IN4_AR, OUTPUT);

  pinMode(IN1_BL, OUTPUT);
  pinMode(IN2_BL, OUTPUT);
  pinMode(IN3_BR, OUTPUT);
  pinMode(IN4_BR, OUTPUT);

  digitalWrite(IN1_AL, HIGH);
  digitalWrite(IN2_AL, LOW);
  digitalWrite(IN3_AR, LOW);
  digitalWrite(IN4_AR, HIGH);

  digitalWrite(IN1_BL, HIGH);
  digitalWrite(IN2_BL, LOW);
  digitalWrite(IN3_BR, LOW);
  digitalWrite(IN4_BR, HIGH);
  attachInterrupt(digitalPinToInterrupt(left_front_wheel.hall), left_front_count, FALLING);
  attachInterrupt(digitalPinToInterrupt(right_front_wheel.hall), right_front_count, FALLING);
  attachInterrupt(digitalPinToInterrupt(left_back_wheel.hall), left_back_count, FALLING);
  attachInterrupt(digitalPinToInterrupt(right_back_wheel.hall), right_back_count, FALLING); //中断函数 用于0计数

}

void loop()
{  
  x += cos(theta)*linear*0.1;
  y += sin(theta)*linear*0.1;
  theta += angular*0.1;
  if(theta > 3.14)
    theta=-3.14;
    //get_velomg();                  //配合遥控建图使用

  /*from 92 to 109 */
  omg_in = theta;                 //转向速度输入
  vel_in = linear;                //前进速度输入 
  left_front_wheel.vel_process();
  left_front_wheel.SetPoint = left_front_wheel.vel_out / (16 * 3.14) * 33; //填入的数字除以33即为转速/所需转速乘以33即为Setpoint_l
  left_front_wheel.pid_process();
  //left_front_wheel.test();  

  right_front_wheel.vel_process();
  right_front_wheel.SetPoint = right_front_wheel.vel_out / (16 * 3.14) * 33; //填入的数字除以33即为转速/所需转速乘以33即为Setpoint_l
  right_front_wheel.pid_process();
  
  left_back_wheel.vel_process();
  left_back_wheel.SetPoint = left_back_wheel.vel_out / (16 * 3.14) * 33; //填入的数字除以33即为转速/所需转速乘以33即为Setpoint_l
  left_back_wheel.pid_process();

  right_back_wheel.vel_process();
  right_back_wheel.SetPoint = right_back_wheel.vel_out / (16 * 3.14) * 33; //填入的数字除以33即为转速/所需转速乘以33即为Setpoint_l
  right_back_wheel.pid_process();


  // tf odom->base_link
  t.header.frame_id = odom;
  t.child_frame_id = base_link;
  
  t.transform.translation.x = x;
  t.transform.translation.y = y;
  
  t.transform.rotation = tf::createQuaternionFromYaw(theta);
  t.header.stamp = nh.now();
  
  broadcaster.sendTransform(t);
  nh.spinOnce();
  
  delay(10);
}


