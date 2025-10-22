#include <Servo.h>
#include <Wire.h>
#include <math.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>


#define MPU6050_ADDRESS 0x68
#define pi 3.1415
#define gravity 9.8
#define filter_alpha_z 0.6
#define filter_alpha_x 0.6

Adafruit_MPU6050 mpu;


Servo Servo_1;
Servo Servo_2;//舵机一：端口11；舵机2：端口10

int Servoc_1;
int Servoc_2;//舵机一绕z，舵机二绕x

float ax,ay,az;
float raw_angle_z,raw_angle_x;
float filtered_angle_z=120;
float filtered_angle_x=120;//一阶滤波


void do_sev(float deg_z,float deg_x);
float filtering_acc(float given_acc);
float filtering_deg(float given_deg);
float lowPassFilter(float given_raw_angle,float given_filtered_angle);

void do_sev(float deg_z,float deg_x){
  Servo_1.write(deg_z);
  Servo_2.write(deg_x);
  // delay(30);
}

float filtering_acc(float given_acc){
  if(-1<given_acc && given_acc<1){
    return 0;
  }else{
    return given_acc;
  }
}

float filtering_deg(int given_deg){
  if(given_deg<=20){
    return 20;
  }else if(given_deg>=170){
    return 170;
  }else{
    return given_deg;
  }
}

float lowPassFilter(float given_raw_angle,float given_filtered_angle,float given_alpha){
  given_filtered_angle=given_alpha*given_raw_angle+(1-given_alpha)*given_filtered_angle;
  return given_filtered_angle;

}

void setup() {
  // put your setup code here, to run once:
  Servo_1.attach(11);
  Servo_2.attach(10);
  do_sev(120,120);
  Serial.begin(9600);
  Wire.begin();
  if(mpu.begin()){
    Serial.println("connected");
  }else{
    Serial.println("error");
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t accelerate , gyro, temp;
  mpu.getEvent(&accelerate ,&gyro, &temp);
  ax=accelerate.acceleration.x-0.4;
  ay=accelerate.acceleration.y-0.05;
  az=accelerate.acceleration.z-0.44;
  ax=filtering_acc(ax);
  ay=filtering_acc(ay);
  raw_angle_z=atan2(ax,sqrt(ay*ay+az*az))*180/pi;
  raw_angle_x=atan2(ay,sqrt(ax*ax+az*az))*180/pi;
  filtered_angle_z=lowPassFilter(raw_angle_z,filtered_angle_z,filter_alpha_z)*1.5;
  filtered_angle_x=lowPassFilter(raw_angle_x,filtered_angle_x,filter_alpha_x)*1.2;

  Servoc_1=filtering_deg((int)filtered_angle_z);
  Servoc_2=filtering_deg((int)filtered_angle_x);
  do_sev(Servoc_1,Servoc_2);
  // Serial.println(raw_angle_z);
  delay(20);
}
