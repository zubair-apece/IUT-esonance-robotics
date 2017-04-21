#include<stdio.h>
#include<math.h>

#define Left_Dir1 5
#define Left_Dir2 7
#define Left_Speed 6
#define Right_Dir1 2
#define Right_Dir2 4
#define Right_Speed 3

#define buzzer A4
#define button A5

int sensor[8] = { 8,9,10,11,12,13,14,15 }; // Pin assignment

int sensorReading[8] = { 0 }; // Sensor reading array

float activeSensor = 0; // Count active sensors
float totalSensor = 0; // Total sensor readings
float avgSensor = 4.5; // Average sensor reading

float Kp = 72;   // Max deviation = 8-4.5 = 3.5 ||  255/3.5 = 72
float Ki = 0.00015;
float Kd = 5;

float error = 0;
float previousError = 0;
float totalError = 0;

float power = 0;

int PWM_Right, PWM_Left;




















void PID_program()
{
    totalSensor=9;
    activeSensor=9;

    avgSensor = totalSensor/activeSensor;
    previousError = error; // save previous error for differential
    error = avgSensor - 4.5; // Count how much robot deviate from center
    totalError += error; // Accumulate error for integral

    power = (Kp*error) + (Kd*(error-previousError)) + (Ki*totalError);
    printf("%f %f %f %f \n",avgSensor,error,totalError,power);
    if( power>255 ) { power = 255.0; }
    if( power<-255.0 ) { power = -255.0; }

    if(power<0) // Turn left
    {
        printf("turn left");
      PWM_Right = 255;
 //     PWM_Left = 255 - abs(int(power));
    }

    else // Turn right
    {
        printf("turn right");
      PWM_Right = 255 - int(power);
      PWM_Left = 255;
    }

}

void lineFollow(void) {
   PID_program();

}

void readSensor(void) {
  for(int i=0; i<8; i++)
    {

    }

    avgSensor = totalSensor/activeSensor;
    activeSensor = 0; totalSensor = 0;
}

main()
{
    PID_program();
}

