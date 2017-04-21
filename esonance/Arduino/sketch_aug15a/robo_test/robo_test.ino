#define right_Motor_11 2
#define right_Motor_12 3
#define right_Motor_21 4
#define right_Motor_22 5
#define left_Motor_11 6
#define left_Motor_12 7
#define left_Motor_21 8
#define left_Motor_22 9
#define left_IR_3 22
#define left_IR_2 24
#define left_IR_1 26
#define center_IR 28
#define right_IR_1 30
#define right_IR_2 32
#define right_IR_3 34

void move_forwrd(){
  digitalWrite(right_Motor_11,HIGH);
  digitalWrite(right_Motor_12,LOW);
  digitalWrite(right_Motor_21,HIGH);
  digitalWrite(right_Motor_22,LOW);
  digitalWrite(left_Motor_11,HIGH);
  digitalWrite(left_Motor_12,LOW);
  digitalWrite(left_Motor_21,HIGH);
  digitalWrite(left_Motor_22,LOW);
  delay(10);
}
void move_left(){
  digitalWrite(right_Motor_11,HIGH);
  digitalWrite(right_Motor_12,LOW);
  digitalWrite(right_Motor_21,HIGH);
  digitalWrite(right_Motor_22,LOW);
  delay(10);
}
void move_right(){
  digitalWrite(left_Motor_11,HIGH);
  digitalWrite(left_Motor_12,LOW);
  digitalWrite(left_Motor_21,HIGH);
  digitalWrite(left_Motor_22,LOW);
  delay(10);
}
void move_hard_left(){
  digitalWrite(right_Motor_11,HIGH);
  digitalWrite(right_Motor_12,LOW);
  digitalWrite(right_Motor_21,HIGH);
  digitalWrite(right_Motor_22,LOW);
  digitalWrite(left_Motor_12,HIGH);
  digitalWrite(left_Motor_11,LOW);
  digitalWrite(left_Motor_22,HIGH);
  digitalWrite(left_Motor_21,LOW);
}
void move_hard_right(){
  digitalWrite(right_Motor_12,HIGH);
  digitalWrite(right_Motor_11,LOW);
  digitalWrite(right_Motor_22,HIGH);
  digitalWrite(right_Motor_21,LOW);
  digitalWrite(left_Motor_11,HIGH);
  digitalWrite(left_Motor_12,LOW);
  digitalWrite(left_Motor_21,HIGH);
  digitalWrite(left_Motor_22,LOW);
}
void setup(){
  pinMode(right_Motor_11,OUTPUT);
  pinMode(right_Motor_12,OUTPUT);
  pinMode(right_Motor_21,OUTPUT);
  pinMode(right_Motor_22,OUTPUT);
  pinMode(left_Motor_11,OUTPUT);
  pinMode(left_Motor_12,OUTPUT);
  pinMode(left_Motor_21,OUTPUT);
  pinMode(left_Motor_22,OUTPUT);
  pinMode(left_IR_3,INPUT);
  pinMode(left_IR_2,INPUT);
  pinMode(left_IR_1,INPUT);
  pinMode(center_IR,INPUT);
  pinMode(right_IR_1,INPUT);
  pinMode(right_IR_2,INPUT);
  pinMode(right_IR_3,INPUT);
}

void loop(){
  if(digitalRead(left_IR_3) == HIGH && digitalRead(left_IR_2)==HIGH && digitalRead(left_IR_1) == HIGH && digitalRead(center_IR) == HIGH);
  
}
