
#define ir1 2
#define ir2 3
#define ir3 4
#define ir4 7
#define ir5 8
#define ir6 12
#define ir7 13

static int sr[7]={};
static int a,b,c,d,e,f,g;
void setup()
{
  Serial.begin(9600);
 pinMode(5,OUTPUT);
 pinMode(6,OUTPUT);
 pinMode(10,OUTPUT);
 pinMode(11,OUTPUT);
 pinMode(ir1,INPUT);
 pinMode(ir2,INPUT);
 pinMode(ir3,INPUT);
 pinMode(ir4,INPUT);
 pinMode(ir5,INPUT);
 pinMode(ir6,INPUT);
 pinMode(ir7,INPUT);
 digitalWrite(ir1,LOW);
 digitalWrite(ir2,LOW);
 digitalWrite(ir3,LOW);
 digitalWrite(ir4,LOW);
 digitalWrite(ir5,LOW);
 digitalWrite(ir6,LOW);
 digitalWrite(ir7,LOW);

 
}

void loop()
{
 
 if(digitalRead(ir1)==HIGH) sr[0]=1;
   else sr[0]=0;
   if(digitalRead(ir2)==HIGH) sr[1]=1;
   else sr[1]=0;
   if(digitalRead(ir3)==HIGH) sr[2]=1;
   else sr[2]=0;
   if(digitalRead(ir4)==HIGH) sr[3]=1;
   else sr[3]=0;
   if(digitalRead(ir5)==HIGH) sr[4]=1;
   else sr[4]=0;
   if(digitalRead(ir6)==HIGH) sr[5]=1;
   else sr[5]=0;
   if(digitalRead(ir7)==HIGH) sr[6]=1;
   else sr[6]=0;
   
   a=sr[0];
   b=sr[1];
   d=sr[2];
   e=sr[3];
   f=sr[4];
   g=sr[5];
   h=sr[6];
   
   
 
   if((a==1&&b==1&&c==1&&d==0&&e==0&&f==0&&g==0)||(a==1&&b==1&&c==0&&d==0&&e==0&&f==0&&g==0)||(a==1&&b==0&&c==0&&d==0&&e==0&&f==0&&g==0)||(a==1&&b==1&&c==1&&d==1&&e==0&&f==0&&g==0)||(a==0&&b==1&&c==1&&d==1&&e==0&&f==0&&g==0))
   {
     analogWrite(5,200);
     analogWrite(6,0);
     analogWrite(9,200);
     analogWrite(10,0);
     
   }
   
 for(int i=0;i<7;i++)
 {
  Serial.print(sr[i]); 
 }
 Serial.print(" \n");
 
 delay(500);
 
 
}
 
