#include<EEPROM.h>
#include <QTRSensors.h>
#include <NewPing.h>
#include <Servo.h>
//defining motor pins
#define motor1_in1 7
#define motor1_in2 8
#define motor2_in1 4
#define motor2_in2 6
#define motor1 3    //left motor
#define motor2 5    //right motor
//defining servo
#define servo_signal 10
//defining ultra_sound
#define echo 11
#define trigger 12
//defining sensor emitter pin
#define NUM_SENSORS             8  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN             6  // emitter is controlled by digital pin 6
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 30 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

QTRSensorsAnalog qtra((unsigned char[]) {A0, A1, A2, A3, A4, A5, A6, A7}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

Servo USound;


unsigned int white_value[NUM_SENSORS];
unsigned int black_value[NUM_SENSORS];
unsigned int mid_value[NUM_SENSORS];
char directionInObstacles;

static float err[2000]={};
static double integral = 2;
static double prev_integral[4]={};
static double max_integral=0;
int integralCapacity=2000;
unsigned long loop_timer=0;
static float prev_error=0;

int Kp=220;
int Ki=35.3;
int Kd=7;
float max_drive = Kp*5.5*5.5*5.5 + Ki*18.0+Kd*1;
boolean callibrated=false;
boolean pathSearched=false;
boolean BLACK=0;
boolean WHITE=1;
boolean TRACK =BLACK;
boolean sensorReading[8]={};
short int wayVectors[9]={};

static boolean isZ_l=false;
static boolean isZ_r=false;
static boolean isI=false;
static boolean isT=false;
static boolean isL_l=false;
static boolean isL_r=false;

boolean isLmostTouched=false;


int leftDistance=0;
int rightDistance=0;


///////////////////////////////////////////////////////////////////////////setting the direction of rotation of motors/////////////////////////////////////////////////////////////
void setMotorRotationDirection(char m,char d)
{
 if(m=='l')
{
 if(d=='b') 
 {
  digitalWrite(motor1_in1,HIGH);
  digitalWrite(motor1_in2,LOW);
 }
 else if(d=='f')
 {
   digitalWrite(motor1_in1,LOW);
  digitalWrite(motor1_in2,HIGH);
 }
} 
else if(m=='r')
{
 if(d=='b') 
 {
  digitalWrite(motor2_in1,HIGH);
  digitalWrite(motor2_in2,LOW);
 }
 else if(d=='f')
 {
   digitalWrite(motor2_in1,LOW);
  digitalWrite(motor2_in2,HIGH);
 }
}
}


//////////////////////////////////////////////////////////////////////calculating the error////////////////////////////////////////////////////////////
float getCurrentPosition()
{
  float end=0;
  float distance=0;
  for(int i=0;i<NUM_SENSORS;i++)
  {
    if(sensorReading[i]==0){
      end=i+1;
      distance++;
    }
  }
  if(distance==0)
  {
    return prev_error;
  }
  float cur_pos = (end-distance/2.0)-((float)NUM_SENSORS/2);
  Serial.print("\n");
  for(int i=0;i<8;i++)
  {
  //Serial.print(" ");
  // Serial.print(sensorReading[i]); 
  }
  
  if(cur_pos!=0) cur_pos=(abs(cur_pos)+2)*(cur_pos/abs(cur_pos));
  
  
  if(abs(cur_pos)<=2.9)
  {
    if(abs(integral)>abs(max_integral)) max_integral=integral;
    Serial.print("mxi ");
  Serial.print(max_integral);
  Serial.print("int ");
  Serial.print(integral);
    
  for(int i=0;i<4;i++)
  {
    prev_integral[3-i]=prev_integral[2-i];
    
  }
  prev_integral[0]=integral;
  for(int i=0;i<4;i++)
  {
   Serial.print("...>");
   Serial.print(prev_integral[i]); 
  }
  
  integral=0;
  //if((abs(prev_integral[1]-prev_integral[3]))>abs(max_integral*0.1)) Kp++;
  //else if((abs(prev_integral[3]-prev_integral[1]))>(max_integral*0.1)) Kp--;
  }
  
  EEPROM.write(0,Kp);
  Serial.print("\n\n");
  Serial.print(cur_pos);
  Serial.print(" ");
  Serial.print(Kp);
  
  return cur_pos;
}

////////////////////////////////////////////////////////////////////determining the  direction of robot in section2//////////////////////////////
void determineDirectionInObstacles()
{
  int left_weight=0,right_weight=0;
  for(int i=0;i<4;i++)
  {
    left_weight+=wayVectors[i];
  }
  for(int i=4;i<8;i++)
  {
    right_weight+=wayVectors[i];
  }
  if(wayVectors[3]==0&&wayVectors[4]==0&&wayVectors[5]==0) 
  {
    if((wayVectors[0]>1)&&(wayVectors[0]<8)) 
    {
      Serial.print(wayVectors[0]);
      Serial.print("-->..");
      directionInObstacles='r';
    }
    else if((wayVectors[8]>1)&&(wayVectors[8]<8)) 
    {
      Serial.print(wayVectors[8]);
      Serial.print("-->..");
      directionInObstacles='l';
    }
    else directionInObstacles='f';
    
  }
  else if(left_weight>right_weight) directionInObstacles='R';
  else if(right_weight>left_weight) directionInObstacles='L';
  
  Serial.print("            <<");
  Serial.print(directionInObstacles);
  
  
}

/////////////////////////////////////////////////////////////////moving the robot in section2//////////////////////////////////////
void goBetweenObstacles()
{
 if (directionInObstacles=='f') 
 {
   setMotorRotationDirection('l','f');
  setMotorRotationDirection('r','f');
  analogWrite(motor1,255);
  analogWrite(motor2,255);
  delay(400);
 }
 else if(directionInObstacles=='r')
 {
   setMotorRotationDirection('l','f');
  setMotorRotationDirection('r','b');
  analogWrite(motor1,0);
  analogWrite(motor2,255);
  delay(50);
 }
 else if(directionInObstacles=='l')
 {
   setMotorRotationDirection('l','b');
  setMotorRotationDirection('r','f');
  analogWrite(motor1,255);
  analogWrite(motor2,0);
  delay(50);
 }
 else if(directionInObstacles=='L')
 {
   setMotorRotationDirection('l','b');
  setMotorRotationDirection('r','f');
  analogWrite(motor1,255);
  analogWrite(motor2,100);
  delay(200);
 }
 else if(directionInObstacles=='R')
 {
   setMotorRotationDirection('l','f');
  setMotorRotationDirection('r','b');
  analogWrite(motor1,100);
  analogWrite(motor2,255);
  delay(200);
 }
 analogWrite(motor1,0);
  analogWrite(motor2,0);
 
 
}

//////////////////////////////////////////////////////////////////determining PID pwm////////////////////////////////////////////////////////////////
int determinePWM()
{
  


  float error=getCurrentPosition();
  
  //Serial.print("lksjfd");
 // int error = getCurrentPosition();

 /* for(int i=0;i<integralCapacity;i++)
    {
      err[(integralCapacity-1)-i]=err[(integralCapacity-2)-i];
    }
    if(error!=0)
    {
    for(int i=0;i<integralCapacity;i++)
    {
      integral=integral+err[i];
    }
    }
    else integral=0;

  err[0]=error;
*/

  integral=integral+error;
  float derivative = (error - prev_error);
  float drive = 1.10*error*Kp*error*error+integral*Ki+derivative*Kd;
  int displacement = ((float)drive/(float)max_drive)*(float)250;
  if(displacement>250) displacement=250;
  if(displacement<-250) displacement=-250;
  prev_error=error;
  Serial.print("\nin pwm:  ");
 Serial.print(error);
  Serial.print(" integral>> ");
 Serial.print(integral);
 Serial.print("  ");
  Serial.print(derivative);
  Serial.print("  ");
  Serial.print(displacement);
  Serial.print("  ");
 Serial.print(drive);
  Serial.print("  \n");
  
  
  if(displacement>0)//move right
  {
   analogWrite(motor2,255);
   //digitalWrite(Motor_left_2,LOW);
   analogWrite(motor1,255-displacement);
   //digitalWrite(Motor_right_2,LOW);
   
  }
  
  else
  {
    analogWrite(motor2,255+displacement);
  // digitalWrite(Motor_left_2,LOW);
   analogWrite(motor1,255);
  // digitalWrite(Motor_right_2,LOW);
   
  }
  
  
  return displacement;


}

//////////////////////////////////////////////////////////////////////////////////////////////////////Callibrating sensors////////////////////////////////////////////////////
void callibrate()
{
  
  Serial.print("\nCallibrating sensors....\n");
  setMotorRotationDirection('l','f');
  setMotorRotationDirection('r','f');
  analogWrite(motor1,255);
  analogWrite(motor2,255);
  delay(100);
  analogWrite(motor1,0);
  analogWrite(motor2,0);
  
 Serial.print("\n\nplace on white surface\n");
 delay(5000);
 qtra.read(sensorValues,QTR_EMITTERS_ON);
 for(int i=0;i<8;i++)
 {
   white_value[i]=sensorValues[i];
   Serial.print(white_value[i]);
    Serial.print(" ");
 }
 analogWrite(motor1,255);
  analogWrite(motor2,255);
  delay(100);
  analogWrite(motor1,0);
  analogWrite(motor2,0);
 Serial.print("\n\nplace on black surface\n");
 delay(5000);
 qtra.read(sensorValues,QTR_EMITTERS_ON);
 for(int i=0;i<8;i++)
 {
   black_value[i]=sensorValues[i];
    Serial.print(black_value[i]);
    Serial.print(" ");
 }
 analogWrite(motor1,255);
  analogWrite(motor2,255);
  delay(100);
  analogWrite(motor1,0);
  analogWrite(motor2,0);
 Serial.print("\n\ncalculating average value\n");
 for(int i=0;i<8;i++)
 {
  mid_value[i]=(white_value[i]+black_value[i])/2; 
  
   Serial.print(mid_value[i]);
    Serial.print(" ");
 }
 delay(5000);
  
}
//////////////////////////////////////////////////////////////////////setting digital values of sensors from analog values///////////////////////////////////
void readSensors()
{
 qtra.read(sensorValues);
 for(int i=0;i<8;i++)
{
 if(sensorValues[i]<mid_value[i]) sensorReading[i]=1^TRACK;
 else sensorReading[i]=0^TRACK;
} 
}

//////////////////////////////////////////////////////////////////////finding way for obstacle avoiding in section two///////////////////////////////////////
void findWay()
{
  USound.write(0);
  delay(500);
  for(int i=20;i<=180;i+=20)
  {
    USound.write(i);
    delay(150);
    wayVectors[8-(i/20-1)]=sonar.ping()/ US_ROUNDTRIP_CM;
    delay(10);
    
  }
  
  for(int i=0;i<9;i++)
  {
   Serial.print(wayVectors[i]);
    Serial.print(" "); 
  }
  Serial.print("\n");
  
}

void searchPath()
{
  
  USound.write(160);
  delay(500);
  leftDistance=(sonar.ping()/US_ROUNDTRIP_CM);
  USound.write(40);
  delay(500);
  rightDistance=(sonar.ping()/US_ROUNDTRIP_CM);
  USound.write(100);
  
  Serial.print(leftDistance);
  Serial.print("  ");
  Serial.print(rightDistance);
  Serial.print("  \n");
  
  
}

void avoidObstacle()
{
 searchPath(); 
 setMotorRotationDirection('l','f');
  setMotorRotationDirection('r','f');
 analogWrite(motor1,255);
 analogWrite(motor2,0);
 delay(500);
 analogWrite(motor1,255);
 analogWrite(motor2,255);
 delay(500);
 USound.write(20);
 while(1)
 {
   int distance=sonar.ping()/US_ROUNDTRIP_CM;
  if(distance>1&&distance<15)
  {
  analogWrite(motor1,255);
 analogWrite(motor2,0);
 delay(10);
  }
  else
  {
   analogWrite(motor1,0);
 analogWrite(motor2,255); 
 delay(10);
  }
 }
 

}

boolean isObstacleInFront()
{
 int distance=sonar.ping();
 distance=distance/US_ROUNDTRIP_CM;
 Serial.print(distance);
 Serial.print(" \n");
 if(distance<15&&distance>4) 
 {
   pathSearched=false;
   return true;
   
 }
 else return false;
 
}

boolean isDeadEnd()
{
  for(int i=0;i<8;i++)
  {
   if(sensorReading[i]==0)
    return false;
  }
  return true;
}


///////////////////////////////////////////////////////////////////////////////////////////checkEnd()//////////////////////////////////////////////////
void checkEnd()
{
  Serial.print("\n\n in check end\n\n\n");
 
   readSensors();
   if((abs(millis()-loop_timer)>5000)&&!isDeadEnd())
   {
    loop_timer=millis();
    isI=false;
    isT=false;
    isZ_l=false;
    isZ_r=false;
    isL_l=false;
    isL_r=false;
    Serial.print("\n\neverything cleared... \n\n");
   }
  if((sensorReading[0]==1&&(sensorReading[3]==0||sensorReading[4]==0)&&sensorReading[7]==1)&&!isZ_l&&!isZ_r&&!isL_l&&!isL_r&&!isT)
  {
   isI=true;
   Serial.print("\n\nis IIIIIIIIII");
   
  }
   else if(((sensorReading[0]==0&&sensorReading[1]==0&&sensorReading[2]==0&&sensorReading[3]==0)&&sensorReading[7]==1)&&!isZ_l&&!isZ_r&&!isT)
   {
   isL_r=true;
   Serial.print("\n\nis L rrrrrrrr");
   }
   else if(((sensorReading[7]==0&&sensorReading[6]==0&&sensorReading[5]==0&&sensorReading[4]==0)&&sensorReading[0]==1)&&!isZ_l&&!isZ_r&&!isT)
   {
   isL_l=true;
   Serial.print("\n\nis L lllllllllllll");
   }
   
   else if((sensorReading[0]==0&&(sensorReading[1]==1||sensorReading[2]==1)&&(sensorReading[3]==0||sensorReading[4]==0||sensorReading[5]==0)&&sensorReading[7]==1)&&!isT&&!isZ_l&&!isZ_r)
   {
   isZ_l=true;
   Serial.print("\n\nis Z llllllllllllll");
   }
   else if((sensorReading[7]==0&&(sensorReading[6]==1||sensorReading[5]==1)&&(sensorReading[4]==0||sensorReading[3]==0||sensorReading[2]==0)&&sensorReading[0]==1)/!isT&&!isZ_l&&!isZ_r)
   {
   isZ_r=true;
   Serial.print("\n\nis Z rrrrrrrrrrrr");
   }
   else if(sensorReading[0]==0&&sensorReading[1]==0&&sensorReading[2]==0&&sensorReading[3]==0&&sensorReading[4]==0&&sensorReading[5]==0&&sensorReading[6]==0&&sensorReading[7]==0)
   {
   isT=true;
   Serial.print("\n\nis TTTTTTTTTTTTTTTTTT");
   }
   
 
}

//////////////////////////////////////////////////////////////////////////turn()///////////////////////////

void setWhichOneTouched()
{
  readSensors();
 if((sensorReading[0]==0)&&(sensorReading[7]==1)) isLmostTouched=true;
 else if((sensorReading[0]==1)&&(sensorReading[7]==0)) isLmostTouched=false;
 
}
//////////////////////////////////////isCentr()//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

boolean isCentr()
{
 if(getCurrentPosition()>=4)
return true;
else return false;
}
//////////////////////////////////////////////////////////                              turn()///////////////////////////////////////////////////////////////////////
void turn()
{
  Serial.print("\n\nTurn\n\n");
 setWhichOneTouched();
 Serial.print("<T ");
 Serial.print(isT);
  Serial.print("> ");
 Serial.print("<Zl ");
 Serial.print(isZ_l);
  Serial.print("> ");
  Serial.print("<Zr ");
 Serial.print(isZ_r);
  Serial.print("> ");
  Serial.print("<Ll ");
 Serial.print(isL_l);
  Serial.print("> ");
  Serial.print("<Lr ");
 Serial.print(isL_r);
  Serial.print("> ");
  Serial.print("<I ");
 Serial.print(isI);
  Serial.print("> "); 
  for(int i=0;i<8;i++)
  {
   Serial.print(sensorReading[i]); 
  }
  Serial.print("\n");
if(isT)
{
  Serial.print("\n this is T....\n");
  analogWrite(motor1,0);
  analogWrite(motor2,255);
 // if(isCentr())
 // isT=false;
}
  else if(isZ_l)
{
  Serial.print("\n this is Z left....\n");
   analogWrite(motor1,0);
  analogWrite(motor2,255);
 // if(isCentr())
  //isZ_l=false;
}
else if(isZ_r)
{
  Serial.print("\n this is Z_right....\n");
    analogWrite(motor1,255);
  analogWrite(motor2,0);
 // if(isCentr())
 // isZ_r=false;
}
else if(isL_l)
{
  Serial.print("\n this is L_left....\n");
    analogWrite(motor1,0);
  analogWrite(motor2,255);
 // if(isCentr())
 // isL_l=false;
}
else if(isL_r)
{
  Serial.print("\n this is L_right....\n");
    analogWrite(motor1,255);
  analogWrite(motor2,0);
  //if(isCentr())
  //isT=false;
}
else if(isI)
{
  Serial.print("\n this is I_right....\n");
  //isI=false;
}

}

void setup()
{
  
  Serial.begin(9600);
  USound.attach(servo_signal);
  pinMode(motor1_in1,OUTPUT);
  pinMode(motor1_in2,OUTPUT);
  pinMode(motor2_in1,OUTPUT);
  pinMode(motor2_in2,OUTPUT);
  
  //pinMode(servo_signal,OUTPUT);
  
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  pinMode(A6,INPUT);
  pinMode(A7,INPUT);
  
}

void loop()
{
  
 /* if(isObstacleInFront()&&pathSearched==false)
  {
  avoidObstacle();
  pathSearched=true;
  }
  
  if(callibrated==false)
  {
  callibrate();
  callibrated=true;
  }
  readSensors();
  setMotorRotationDirection('l','f');
  setMotorRotationDirection('r','f');
 determinePWM();
 
 
 
 
 
  if(callibrated==false)
  {
  callibrate();
  callibrated=true;
  }
  if(isDeadEnd())
  {
    Serial.print("deadEnd\n\n");
    
  }
  else Serial.print("on line\n\n");
  for(int i=0;i<8;i++)
  {
    Serial.print(" ");
    Serial.print(sensorReading[i]);
  }
  delay(200);
  if(callibrated==false)
  {
  callibrate();
  callibrated=true;
  }
  readSensors();
  setMotorRotationDirection('l','f');
  setMotorRotationDirection('r','f');
 determinePWM();
 
 
  
 findWay();
 determineDirectionInObstacles();
 goBetweenObstacles();
 

  */
  if(!isDeadEnd()&&(abs(prev_error)<=4.5))
  {
  checkEnd();
  }
  
 if(callibrated==false)
  {
  callibrate();
  callibrated=true;
  }
  readSensors();
  setMotorRotationDirection('l','f');
  setMotorRotationDirection('r','f');
  if(isDeadEnd()&&(abs(prev_error)<=4.5))
  {
    
    
    turn();
    //Serial.print("deadEnd\n\n");
    
  }
  else 
  determinePWM();
  

/*
USound.write(20);
delay(150);
USound.write(40);
delay(150);
USound.write(60);
delay(150);
USound.write(80);
delay(150);
USound.write(100);
delay(150);
USound.write(120);
delay(150);
USound.write(140);
delay(150);
USound.write(160);
delay(150);
USound.write(180);
delay(150);
USound.write(160);
delay(150);
USound.write(140);
delay(150);
USound.write(120);
delay(150);
USound.write(100);
delay(150);
USound.write(80);
delay(150);
USound.write(60);
delay(150);
USound.write(40);
delay(150);
USound.write(20);
delay(150);
*/
  
}
  
