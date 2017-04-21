#define left_Motor_11 2
#define left_Motor_12 3
#define right_Motor_11 4
#define right_Motor_12 5
#define left_Motor_21 6
#define left_Motor_22 7
#define right_Motor_21 8
#define right_Motor_22 9
#define left_IR_3 22
#define left_IR_2 24
#define left_IR_1 26
#define center_IR 28
#define right_IR_1 30
#define right_IR_2 32
#define right_IR_3 34
#define front_IR 36

static int dt=10;

const int BLACK = 0;
const int WHITE = 1;

const int Kp = 50;
const int Ki = 2;
const int Kd = 0;

const int TRACK = BLACK;


const int max_drive=14*Kp+10*2;
int integral = 0;
int prev_error=0;

static int l3,l2,l1,c,r1,r2,r3,front;


int err[10]={};

int determinePWM()
{
    integral=0;
    int ideal = 0;
    int actual = l3*(-3*3)+l2*(-2*2)+l1*(-1*1)+r1*(1)+r2*(2*2)+r3*(3*3);
    int error = actual - ideal;
    for(int i=0;i<10;i++)
    {
      err[9-i]=err[8-i];
    }

    for(int i=0;i<10;i++)
    {
      integral=integral+err[i];
      printf("%d ",err[i]);
    }

    err[0]=error;
    int drive = error*Kp+integral*Ki;
    int displacement = (drive/max_drive)*55;
    int pwm= 196 - displacement;
    prev_error=error;
    return displacement;
}

/*void turnLeft()
{

  analogWrite(left_Motor_12,196);
  analogWrite(left_Motor_22,196);
  analogWrite(right_Motor_11,220);
  analogWrite(right_Motor_21,220);
 
}

void turnRight()
{

  analogWrite(left_Motor_11,220);
  analogWrite(left_Motor_21,220);
  analogWrite(right_Motor_12,196);
  analogWrite(right_Motor_22,196);
 
}

void goStraight()
{

  analogWrite(left_Motor_11,196);
  analogWrite(left_Motor_21,196);
  analogWrite(right_Motor_12,196);
  analogWrite(right_Motor_22,196);
 
}

int turnLEFT()
{
 if(!digitalRead(center_IR))
{
 return 1;
} 
else
{
  
  analogWrite(left_Motor_12,100);
  analogWrite(left_Motor_22,100);
  analogWrite(right_Motor_11,220);
  analogWrite(right_Motor_21,220);
  
}

return 0;

}

int goForward10cm()
{
 return 0; 
}*/

void goForwardPI()
{ 
  int pwm= determinePWM();
  
  
  if(pwm>+10)//turn right
  {
     analogWrite(left_Motor_11,196+pwm);
    digitalWrite(left_Motor_12,LOW);
    analogWrite(left_Motor_21,196+pwm);
    digitalWrite(left_Motor_22,LOW);
    analogWrite(right_Motor_12,40-pwm);
  digitalWrite(right_Motor_11,LOW);
  analogWrite(right_Motor_22,40-pwm);
  digitalWrite(right_Motor_21,LOW);
  }
  else if(pwm<-10)//turn left
  {
     analogWrite(left_Motor_12,60+pwm);
  digitalWrite(left_Motor_11,LOW);
  analogWrite(left_Motor_22,60+pwm);
  digitalWrite(left_Motor_21,LOW);
  analogWrite(right_Motor_11,196-pwm);
  digitalWrite(right_Motor_12,LOW);
  analogWrite(right_Motor_21,196-pwm);
  digitalWrite(right_Motor_22,LOW);
  }
  else
  {
    analogWrite(left_Motor_11,196-pwm);
  digitalWrite(left_Motor_12,LOW);
  analogWrite(left_Motor_21,196-pwm);
  digitalWrite(left_Motor_22,LOW);
  analogWrite(right_Motor_11,196+pwm);
  digitalWrite(right_Motor_12,LOW);
  analogWrite(right_Motor_21,196+pwm);
  digitalWrite(right_Motor_22,LOW);
  }
  
  
}



void setup()
{
  pinMode(right_Motor_11,OUTPUT);
  pinMode(right_Motor_12,OUTPUT);
  pinMode(right_Motor_21,OUTPUT);
  pinMode(right_Motor_22,OUTPUT);
  pinMode(left_Motor_11,OUTPUT);
  pinMode(left_Motor_12,OUTPUT);
  pinMode(left_Motor_21,OUTPUT);
  pinMode(left_Motor_22,OUTPUT);
  pinMode(36,OUTPUT);
  pinMode(38,OUTPUT);
  digitalWrite(36,HIGH);
  digitalWrite(38,HIGH);
  pinMode(left_IR_3,INPUT);
  pinMode(left_IR_2,INPUT);
  pinMode(left_IR_1,INPUT);
  pinMode(center_IR,INPUT);
  pinMode(right_IR_1,INPUT);
  pinMode(right_IR_2,INPUT);
  pinMode(right_IR_3,INPUT);
  }

void loop()
{
 
    //continue solving maze
    
    if(digitalRead(left_IR_3)==LOW) {l3=1^TRACK;
    else l3=0^TRACK;
    if(digitalRead(left_IR_2)==LOW) {l2=1^TRACK;
    else l2=0^TRACK;
    if(digitalRead(left_IR_1)==LOW) {l1=1^TRACK;
    else l1=0^TRACK;
    if(digitalRead(center_IR)==LOW) c=1^TRACK;
    else c=0^TRACK;
    if(digitalRead(right_IR_1)==LOW) r1=1^TRACK;
    else r1=0^TRACK;
    if(digitalRead(right_IR_2)==LOW) r2=1^TRACK;
    else r2=0^TRACK;
    if(digitalRead(right_IR_3)==LOW) r3=1^TRACK;
    else r3=0^TRACK;
    if(digitalRead(front_IR)==LOW) front=1^TRACK;
    else front=0^TRACK;
    
    delay(3);
    goForwardPI();
    
    
    
   /* if(front==0 && l3==0 && r3==0 && c==1)
    {
            if(goForward10cm())
            {
                //go forward;
            }
            else
            {
               // turnLEFT();
            }

     }
     else if(front ==1 && l3==0 && r3==1 && c==1)
     {
            goStraight();
     }
     else if((l3&&l2&&l1&&c&&r1&&r2&&r3) || (front==1 && l3==1 && c==1))
     {
            while(turnLEFT())
     }
     else
     {
            goForwardPI();
     }*/
     
     
       
  
}

