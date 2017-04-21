#define Motor_left_1 6
//#define Motor_left_2 5
#define Motor_right_1 10
//#define Motor_right_2 9
#define IR_left_4 2
#define IR_left_3 3
#define IR_left_2 4
#define IR_left_1 7
#define IR_center 8
#define IR_right_1 11
#define IR_right_2 12
#define IR_right_3 13
#define IR_right_4 5
#define IR_back_left 40
#define IR_back_center 40
#define IR_back_right 40

static int sensorReading[8]={};
static int left[3]={};
const int noOfSensor=8;
int integralCapacity=100;

static int err[100]={};
static int integral = 0;
static int prev_error=0;
boolean BLACK=0;
boolean WHITE=1;
boolean TRACK =WHITE;
boolean LEFT=0;
boolean RIGHT=1;
boolean toRotate = LEFT;
boolean START=0;
boolean END=1;
boolean positn=START;
int Kp=200;
int Ki=10;
int Kd=40;
static int Left_pwm;
static int Right_pwm;

int prev_l4 = 0;
int prev_r4 =0;

int max_drive = Kp*3 + Ki*20+Kd;

void readSensors()
{

  if(digitalRead(IR_left_4)==HIGH) sensorReading[0]=1^TRACK;
  else sensorReading[0]=0^TRACK;
  if(digitalRead(IR_left_3)==HIGH) sensorReading[1]=1^TRACK;
  else sensorReading[1]=0^TRACK;
  if(digitalRead(IR_left_2)==HIGH) sensorReading[2]=1^TRACK;
  else sensorReading[2]=0^TRACK;
  if(digitalRead(IR_left_1)==HIGH) sensorReading[3]=1^TRACK;
  else sensorReading[3]=0^TRACK;
  if(digitalRead(IR_center)==HIGH) sensorReading[4]=1^TRACK;
  else sensorReading[4]=0^TRACK;
  if(digitalRead(IR_right_1)==HIGH) sensorReading[5]=1^TRACK;
  else sensorReading[5]=0^TRACK;
  if(digitalRead(IR_right_2)==HIGH) sensorReading[6]=1^TRACK;
  else sensorReading[6]=0^TRACK;
  if(digitalRead(IR_right_3)==HIGH) sensorReading[7]=1^TRACK;
  else sensorReading[7]=0^TRACK;
  //if(digitalRead(IR_right_4)==HIGH) sensorReading[8]=1^TRACK;
  //else sensorReading[8]=0^TRACK;
  
  /*if(digitalRead(IR_back_left)==HIGH) left[0]=1^TRACK;
  else sensorReading[8]=0^TRACK;
  if(digitalRead(IR_back_center)==HIGH) left[1]=1^TRACK;
  else sensorReading[8]=0^TRACK;
  if(digitalRead(IR_back_right)==HIGH) left[2]=1^TRACK;
  else sensorReading[8]=0^TRACK;
  */
  
  
  

}

/*void setPosition()
  {
   if(isAllIrsOn()&&positn==START) {
     positn=END;
    Serial.print("\nStart\n");
 }
   else 
   {
     positn=END;
      Serial.print("\nEnd\n");
   }
  }
*/

int getCurrentPosition()
{
  int end=0;
  int distance=0;
  for(int i=0;i<noOfSensor;i++)
  {
    if(sensorReading[i]==0){
      end=i+1;
      distance++;
    }
  }
  if(distance==0)
  {
    if(integral>0) return (int)(noOfSensor)/2;
    else return -(int)(noOfSensor)/2;
  }
  int cur_pos = (end-distance/2)-((int)noOfSensor/2+1);
 // printf("---->%d %d %d \n",end,distance,cur_pos);
  return cur_pos;
}

int determinePWM()
{
  integral =0;


  int error=getCurrentPosition();
  
  Serial.print("lksjfd");
 // int error = getCurrentPosition();

  for(int i=0;i<integralCapacity;i++)
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

  int derivative = (error - prev_error);
  int drive = error*Kp+integral*Ki+derivative*Kd;
  int displacement = ((float)drive/(float)max_drive)*(float)250;
  if(displacement>250) displacement=250;
  if(displacement<-250) displacement=-250;
  prev_error=error;
  Serial.print("\nin pwm:  ");
 Serial.print(error);
  Serial.print("  ");
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
   analogWrite(Motor_left_1,250);
   //digitalWrite(Motor_left_2,LOW);
   analogWrite(Motor_right_1,250-displacement);
   //digitalWrite(Motor_right_2,LOW);
   
  }
  
  else
  {
    analogWrite(Motor_left_1,250+displacement);
  // digitalWrite(Motor_left_2,LOW);
   analogWrite(Motor_right_1,250);
  // digitalWrite(Motor_right_2,LOW);
   
  }
  
  return displacement;


}

void setRotationDirection()
{
  if(sensorReading[0]==0 && sensorReading[6]==1)
  toRotate=LEFT;
  else if(sensorReading[0]==1 && sensorReading[6]==0)
  toRotate =RIGHT;
}
void RotateSharp()
{
  Serial.print("\n\n\nrotating sharp to ");
  if(toRotate==LEFT)
  {
    Serial.print("Left\n\\n");
    analogWrite(Motor_left_1,0);
   //digitalWrite(Motor_left_2,LOW);
   analogWrite(Motor_right_1,250);
   //digitalWrite(Motor_right_2,LOW);
   
  }
  else if(toRotate==RIGHT) 
  {
    Serial.print("Right\n\n ");
  analogWrite(Motor_left_1,250);
  // digitalWrite(Motor_left_2,LOW);
   analogWrite(Motor_right_1,0);
  // digitalWrite(Motor_right_2,LOW);
}

}

  void rotateHardLeft()
  {
   analogWrite(Motor_left_1,0);
  // digitalWrite(Motor_left_2,LOW);
   analogWrite(Motor_right_1,250);
  // digitalWrite(Motor_right_2,LOW);
   Serial.print("\n\nRotating HardLeft\n\n");
   delay(200);
  }
  
  void moveForward()
  {
    Serial.print("\n\nMoving forward***\n\n");
    analogWrite(Motor_left_1,250);
  // digitalWrite(Motor_left_2,LOW);
   analogWrite(Motor_right_1,250);
  // digitalWrite(Motor_right_2,LOW);
  }
  
  void stopMoving()
  {
    
    Serial.print("\n\nMoving stopped***\n\n");
    analogWrite(Motor_left_1,0);
  // digitalWrite(Motor_left_2,LOW);
   analogWrite(Motor_right_1,0);
   //digitalWrite(Motor_right_2,LOW);
  }
/////////////////////////////////////////////////////
int isOutOfTrack()
  {
    int count=0;
    for(int i=0;i<8;i++)
    {
      if(sensorReading[i]==0) count++;
    } 
    if(count==0) return 1;
    else return 0;
  }
  
int isAllFrontIrsOn()
{
  
 for(int i=0;i<8;i++)
{
 if(sensorReading[i]==1) return 0;
} 
return 1;
}

int isAllIrsOn()
{
 if(isAllFrontIrsOn() && left[0]==0 && left[1]==0 && left[2]==0) return 1;
 else return 0;
}
void setup()
{
 Serial.begin(9600);
  pinMode(Motor_left_1,OUTPUT);
 // pinMode(Motor_left_2,OUTPUT);
  pinMode(Motor_right_1,OUTPUT);
 // pinMode(Motor_right_2,OUTPUT);
  pinMode(IR_left_4,INPUT);
  pinMode(IR_left_3,INPUT);
  pinMode(IR_left_2,INPUT);
  pinMode(IR_left_1,INPUT);
  pinMode(IR_center,INPUT);
  pinMode(IR_right_1,INPUT);
  pinMode(IR_right_2,INPUT);
  pinMode(IR_right_3,INPUT);
  pinMode(IR_right_4,INPUT);
 // pinMode(IR_right_4,INPUT);
  //pinMode(IR_front,INPUT);
  digitalWrite(IR_left_4,LOW);
  digitalWrite(IR_left_3,LOW);
  digitalWrite(IR_left_2,LOW);
  digitalWrite(IR_left_1,LOW);
  digitalWrite(IR_center,LOW);
  digitalWrite(IR_right_1,LOW);
  digitalWrite(IR_right_2,LOW);
  digitalWrite(IR_right_3,LOW);
  digitalWrite(IR_right_4,LOW);
  
 
}

void loop()
{
  digitalWrite(IR_left_3,LOW);
  digitalWrite(IR_left_2,LOW);
  digitalWrite(IR_left_1,LOW);
  digitalWrite(IR_center,LOW);
  digitalWrite(IR_right_1,LOW);
  digitalWrite(IR_right_2,LOW);
  digitalWrite(IR_right_3,LOW);
  //readSensors();
  //setRotationDirection();
  Serial.print("\nsensor reading-->");
  /*sensorReading[0]=1;
  sensorReading[1]=1;
  sensorReading[2]=0;
  sensorReading[3]=0;
  sensorReading[4]=0;
  sensorReading[5]=1;
  sensorReading[6]=1;*/
  //digitalWrite(Motor_left_1,HIGH);
  //digitalWrite(Motor_left_2,LOW);
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  readSensors();
  setRotationDirection();
  
  for(int i= 0;i<8;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  digitalWrite(IR_left_4,LOW);
    digitalWrite(IR_left_3,LOW);
  digitalWrite(IR_left_2,LOW);
  digitalWrite(IR_left_1,LOW);
  digitalWrite(IR_center,LOW);
  digitalWrite(IR_right_1,LOW);
  digitalWrite(IR_right_2,LOW);
  digitalWrite(IR_right_3,LOW);
  digitalWrite(IR_left_4,LOW);
  
  if(isOutOfTrack())
  RotateSharp();
  else if(isAllFrontIrsOn()&&!isAllIrsOn())
  rotateHardLeft();
  else if(isAllIrsOn())
  {
   if(positn=START)
   moveForward();
  else if(positn=END)
   stopMoving(); 
  }
  else
  determinePWM();
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /*delay(3000);
  Serial.print("starting-->");
  sensorReading[0]=0;
  sensorReading[1]=0;
  sensorReading[2]=0;
  sensorReading[3]=0;
  sensorReading[4]=0;
  sensorReading[5]=0;
  sensorReading[6]=0;
  sensorReading[7]=0;
  sensorReading[8]=0;
  left[0]=0;
  left[1]=0;
  left[2]=0;
  for(int i= 0;i<9;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  readSensors();
  setRotationDirection();

  if(positn==START)
  {
    moveForward();
    delay(1000);
    positn=END;
  }
  
  else if(isOutOfTrack())
  RotateSharp();
  else if(isAllFrontIrsOn()&&!isAllIrsOn())
  rotateHardLeft();
  else if(isAllIrsOn())
  {
   if(positn=START)
   moveForward();
  else if(positn=END)
   stopMoving(); 
  }
  else
  determinePWM();
  delay(1000);
  
  
  Serial.print("\nsensor reading-->");
  sensorReading[0]=1;
  sensorReading[1]=1;
  sensorReading[2]=0;
  sensorReading[3]=0;
  sensorReading[4]=0;
  sensorReading[5]=1;
  sensorReading[6]=1;
  sensorReading[7]=1;
  sensorReading[8]=1;
  for(int i= 0;i<9;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  setRotationDirection();
  if(isOutOfTrack())
  RotateSharp();
  else
  determinePWM();
  delay(1000);
  
  
  Serial.print("\nsensor reading-->");
  sensorReading[0]=1;
  sensorReading[1]=0;
  sensorReading[2]=0;
  sensorReading[3]=0;
  sensorReading[4]=1;
  sensorReading[5]=1;
  sensorReading[6]=1;
  sensorReading[7]=1;
  sensorReading[8]=1;
  for(int i= 0;i<9;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  setRotationDirection();
  if(isOutOfTrack())
  RotateSharp();
  else
  determinePWM();
  delay(1000);
  
  Serial.print("\nsensor reading-->");
  sensorReading[0]=0;
  sensorReading[1]=0;
  sensorReading[2]=1;
  sensorReading[3]=1;
  sensorReading[4]=1;
  sensorReading[5]=1;
  sensorReading[6]=1;
  sensorReading[7]=1;
  sensorReading[8]=1;
  for(int i= 0;i<9;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  setRotationDirection();
  if(isOutOfTrack())
  RotateSharp();
  else
  determinePWM();
  delay(1000);
  
  Serial.print("\nsensor reading-->");
  sensorReading[0]=0;
  sensorReading[1]=1;
  sensorReading[2]=1;
  sensorReading[3]=1;
  sensorReading[4]=1;
  sensorReading[5]=1;
  sensorReading[6]=1;
  sensorReading[7]=1;
  sensorReading[8]=1;
  for(int i= 0;i<9;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  setRotationDirection();
  if(isOutOfTrack())
  RotateSharp();
  else
  determinePWM();
  delay(1000);
  
  
  Serial.print("\nsensor reading-->");
  sensorReading[0]=1;
  sensorReading[1]=1;
  sensorReading[2]=1;
  sensorReading[3]=1;
  sensorReading[4]=1;
  sensorReading[5]=1;
  sensorReading[6]=1;
  sensorReading[7]=1;
  sensorReading[8]=1;
  for(int i= 0;i<9;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  setRotationDirection();
  if(isOutOfTrack())
  RotateSharp();
  else
  determinePWM();
  delay(1000);
  
  
  Serial.print("\nsensor reading-->");
  sensorReading[0]=1;
  sensorReading[1]=1;
  sensorReading[2]=1;
  sensorReading[3]=0;
  sensorReading[4]=0;
  sensorReading[5]=0;
  sensorReading[6]=1;
  sensorReading[7]=1;
  sensorReading[8]=1;
  for(int i= 0;i<9;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  setRotationDirection();
  if(isOutOfTrack())
  RotateSharp();
  else
  determinePWM();
  delay(1000);
  
  
  Serial.print("\nsensor reading-->");
  sensorReading[0]=1;
  sensorReading[1]=1;
  sensorReading[2]=1;
  sensorReading[3]=1;
  sensorReading[4]=0;
  sensorReading[5]=0;
  sensorReading[6]=0;
  sensorReading[7]=1;
  sensorReading[8]=1;
  for(int i= 0;i<9;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  setRotationDirection();
  if(isOutOfTrack())
  RotateSharp();
  else
  determinePWM();
  delay(1000);
  
  
  Serial.print("\nsensor reading-->");
  sensorReading[0]=1;
  sensorReading[1]=1;
  sensorReading[2]=1;
  sensorReading[3]=1;
  sensorReading[4]=1;
  sensorReading[5]=1;
  sensorReading[6]=0;
  sensorReading[7]=0;
  sensorReading[8]=0;
  for(int i= 0;i<9;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  setRotationDirection();
  if(isOutOfTrack())
  RotateSharp();
  else
  determinePWM();
  delay(1000);
  
  
  Serial.print("\nsensor reading-->");
  sensorReading[0]=1;
  sensorReading[1]=1;
  sensorReading[2]=1;
  sensorReading[3]=1;
  sensorReading[4]=1;
  sensorReading[5]=1;
  sensorReading[6]=1;
  sensorReading[7]=0;
  sensorReading[8]=0;
  for(int i= 0;i<9;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  setRotationDirection();
  if(isOutOfTrack())
  RotateSharp();
  else
  determinePWM();
  delay(1000);
  
  Serial.print("\nsensor reading-->");
  sensorReading[0]=1;
  sensorReading[1]=1;
  sensorReading[2]=1;
  sensorReading[3]=1;
  sensorReading[4]=1;
  sensorReading[5]=1;
  sensorReading[6]=1;
  sensorReading[7]=1;
  sensorReading[8]=0;
  for(int i= 0;i<9;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  setRotationDirection();
  if(isOutOfTrack())
  RotateSharp();
  else
  determinePWM();
  delay(1000);
  
  Serial.print("\nsensor reading-->");
  sensorReading[0]=0;
  sensorReading[1]=0;
  sensorReading[2]=0;
  sensorReading[3]=0;
  sensorReading[4]=0;
  sensorReading[5]=0;
  sensorReading[6]=0;
  sensorReading[7]=0;
  sensorReading[8]=0;
  left[0]=0;
  left[1]=1;
  left[2]=0;
  for(int i= 0;i<9;i++)
  {
    Serial.print(sensorReading[i]);
    Serial.print(" ");
  }
  Serial.print("\n\n");
  //readSensors();
  setRotationDirection();

  if(positn==START)
  {
    moveForward();
    delay(1000);
    positn=END;
  }
  
  else if(isOutOfTrack())
  RotateSharp();
  else if(isAllFrontIrsOn()&&!isAllIrsOn())
  rotateHardLeft();
  else if(isAllIrsOn())
  {
   stopMoving(); 
  }
  else
  determinePWM();
  delay(1000);
  
  //analogWrite(Motor_left_1,250);
  //analogWrite(Motor_right_1,250);
  
 */



}


