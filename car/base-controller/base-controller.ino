/*
@作者 : lifuguan, Wacokgde
@时间 : 2019.6.10
@状态 : 测试中
@作用 : 以后再写。。。
 */



#include <ros.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>

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

char base_link[] = "/base_link";
char odom[] = "/odom";

void setup()
{
  nh.initNode();
  broadcaster.init(nh);
}

void loop()
{  
  //速度（矢量）给出
  double dx = 0.01;
  double dtheta = 0.00;
  

  x += cos(theta)*dx*0.1;
  y += sin(theta)*dx*0.1;
  theta += dtheta*0.1;
  if(theta > 3.14)
    theta=-3.14;
    
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


