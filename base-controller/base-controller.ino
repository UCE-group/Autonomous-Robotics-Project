#include <ros.h>
#include <PID_v1.h>
#include <string.h>
int IN1_AL = 23;
int IN2_AL = 25;
int IN3_AR = 27;
int IN4_AR = 29;       //对应控制右轮L298N模块-1 IN1/2/3/4,用于控制电机方向与启停
int IN1_BL = 31;
int IN2_BL = 33;
int IN3_BR = 35;
int IN4_BR = 37;       //对应控制左轮L298N模块-2 IN1/2/3/4,用于控制电机方向与启停
long int longterm = 0; //长期控制的四轮平均值
String vel_read_str = "";
String omg_read_str = "";
double vel_read = 0;
double omg_read = 0;
double vel_in = 0.0;
double omg_in = 0.0;
void test();

class motor_settings
{
private:

public:

  double Kp = 0.0, Ki = 0.0, Kd = 0.0; //P/I/D的值
  int status = 0;
  int hall = 0;                //arduino上传入霍尔传感器计数的引脚
  int PWM = 0;                 //arduino上控制分别控制左/右电机的引脚
  int counter = 0;             //次数int 用于计数
  long int counter_long = 0;   //长期次数long int 用于长期控制
  double counter_rotation = 0; //次数double 用于计算转速
  double input_PID = 80;       //次数double 用于计算转速
  double output_PID = 80;      //pid的输出值 由pid算法计算出 用于提供电机PWM[对应test的zkb(占空比)]
  double rad = 0;              //转速 由次数除以33[30(减速比)*11(编码器缺口数)/10(每秒采样10次)]
  double vel_out = 0.0;
  unsigned long times = 0;
  int take_time = 100;
  double SetPoint;
  
  PID myPID;
  
  void pid_process()
  {
    if (SetPoint < 80)
    {
      myPID.SetTunings(8, 4, 0.02);
    }
    if (SetPoint < 61)
    {
      myPID.SetTunings(2, 3, 0.02);
    }
    if (SetPoint < 41)
    {
      myPID.SetTunings(0.8, 1.5, 0.02);
    }
    input_PID = counter_rotation;
    myPID.Compute(); //PID转换完成返回值为1
    analogWrite(PWM, output_PID);
    if (millis() >= times)
    {
      counter_rotation = counter;
      rad = counter_rotation / 33;

      times = millis() + take_time;
      counter = 0; //输出速度结果后清零，记录下一秒的触发次数
    }

    
  }
  void vel_process()
  {
    if (status == 1 || status == 3)
    {
        vel_out = vel_in + omg_in * 15;
    }
    else if (status == 2 || status == 4)
    {
        vel_out = vel_in - omg_in * 15;
    }
  }
  void test()
  {
    Serial.println();
    Serial.print(output_PID);
    Serial.println("-zkb");
    Serial.print(counter_rotation);
    Serial.println("-cishu");
    Serial.print(rad);
    Serial.println("rad/s");
  }
  //用于在setup函数中设置各电机引脚状态
  motor_settings(int a, int b, int c) : hall(a), PWM(b), status(c), myPID(&input_PID, &output_PID, &SetPoint, Kp, Ki, Kd, DIRECT)
  { 
    myPID.SetMode(AUTOMATIC);                                     //设置PID为自动模式
    myPID.SetSampleTime(100);                                     //设置PID采样频率为100ms
    myPID.SetOutputLimits(10, 250);                               // 输出在40-240之间
    pinMode(hall,INPUT);
    pinMode(PWM,OUTPUT);

  }
};



motor_settings right_front_wheel(18, 10, 1);
motor_settings left_front_wheel(19, 11, 2);
motor_settings right_back_wheel(2, 9, 4);
motor_settings left_back_wheel(3, 8, 3);


void left_front_count()
{
  left_front_wheel.counter++;
  left_front_wheel.counter_long++;
}
void right_front_count()
{
  right_front_wheel.counter++;
  right_front_wheel.counter_long++;
}
void left_back_count()
{
  left_back_wheel.counter++;
  left_back_wheel.counter_long++;
}
void right_back_count()
{
  right_back_wheel.counter++;
  right_back_wheel.counter_long++;
}


void longterm_ctrl()
{
  longterm = (left_front_wheel.counter_long + right_front_wheel.counter_long + left_back_wheel.counter_long + right_back_wheel.counter_long)/4;

}

void get_velomg() //此处程序为遥控车用,读取串口2收到的速度与角速度信息,格式为:线速度(m/s)v角速度(rad/s)o,示例:0.5v0.2o
{
  if(Serial2.available()>0) 
  { 
    vel_read_str = "";
    omg_read_str = "";
    while(1)
    {
      if(Serial2.available()>0&&Serial2.peek()!=118) //118对应字符v
      {
        vel_read_str += char(Serial2.read());
        delay(2);
        Serial.print(vel_read_str);
      }
      
      else if(Serial2.peek()==118)
      { 
        Serial2.read();
        break;
      }
    }
    Serial.println("veldone");
    delay(5);
    while(1) //111对应字符o
    {
      if(Serial2.available()>0&&Serial2.peek()!=111) //111对应字符o
      {
        omg_read_str += char(Serial2.read());
        delay(2);
        Serial.print(omg_read_str);
      }
      else if(Serial2.peek()==111)
      { 
        Serial2.read();
        break;
      }
    }
    Serial.println("omgdone");    
  };
  vel_in = atof(vel_read_str.c_str())*100;
  omg_in = atof(omg_read_str.c_str());
  Serial.print("vel_in=");
  Serial.print(vel_in);
  Serial.print("omg_in=");
  Serial.println(omg_in);
}

void setup()
{
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
  //get_velomg();                  //配合遥控建图使用
  omg_in = 0.0;                    //转向速度输入
  vel_in = 0.5*100;                //前进速度输入 
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
}
