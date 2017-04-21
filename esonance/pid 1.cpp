
#include<stdio.h>
#include<math.h>


int left_IR_3 = 1;
int left_IR_2=1;
int left_IR_1 =1;
int center_IR =0;
int right_IR_1 =1;
int right_IR_2 =1;
int right_IR_3 =1;
int integral=0;
int prev_error=0;

static int sensorReading[9]={};
int dt= 10;
int Kp=200;
int Ki=20;
int Kd=80;
int max_drive=4*Kp+10*Ki+10*Kd;



int getCurrentPosition()
{
    int end=0;
    int distance=0;
    for(int i=0;i<=8;i++)
    {
        if(sensorReading[i]==0){
        end=i+1;
        distance++;
        }

    }
    if(distance==0)
    {
        if(integral>0) return 5;
        else return -5;
    }
    int cur_pos = (end-distance/2)-5;


    return cur_pos;
}

int determinePWM(int a,int b,int c,int d,int e,int f,int g,int h,int i)
{


    /*printf("--------------------------\n");
    l4=a;l3=b;l2=c;l1=d;cntr=e;r1=f;r2=g;r3=h;r4=i;
    */
    sensorReading[0]=a;
    sensorReading[1]=b;
    sensorReading[2]=c;
    sensorReading[3]=d;
    sensorReading[4]=e;
    sensorReading[5]=f;
    sensorReading[6]=g;
    sensorReading[7]=h;
    sensorReading[8]=i;
 /*   printf("******************\n");
    int cur_pos=getCurrentPosition();
    printf("******************\n");
    if(e!=0&&center_changed())
    {
        int i = getPosition();
        if(i==1) printf("in right, turn left");
        else if(i==-1) printf("in left, turn right");
        else printf("in center..");
        printf("\n");

    }
   else
   {
        printf("turn %d \n",-1*position);

   }
   float ideal = 0;
*/

int error = getCurrentPosition();


printf("\n");
printf("\n\n-->%d %d %d %d %d %d %d %d %d\n",a,b,c,d,e,f,g,h,i);


integral=integral+error;
int derivative = (error - prev_error);
int drive = error*Kp+integral*Ki+derivative*Kd;
int displacement = ((float)drive/(float)max_drive)*(float)256;
if(displacement>256) displacement=256;
if(displacement<-256) displacement=-256;
int pwm= 196 - displacement;
prev_error=error;
printf("error %d  integral %d  derivative %d  drive %d  displacement %d\n",error,integral,derivative,drive,displacement);
if(displacement>0)
{
    printf("move right\n");

}
else
{
    printf("move left\n");
}

return displacement;
}

main()
{

         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,1,1,0));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,1,1,0));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,1,0,0));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,1,0,0));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,0,0,0));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,0,0,0));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,0,0,0));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,0,0,0));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,0,0,0));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,0,0,0));
         printf("  [%d] \n",determinePWM(1,1,1,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,1,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,1,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,1,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,1,1,1,1,1,1,0));
         printf("  [%d] \n",determinePWM(1,1,1,1,1,1,1,0,0));
          printf("  [%d] \n",determinePWM(1,1,1,1,0,0,0,1,1));
          printf("  [%d] \n",determinePWM(1,1,1,0,0,0,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,0,0,0,1,1,1,1));
         printf("  [%d] \n",determinePWM(1,0,0,0,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(0,0,0,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(0,1,1,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,1,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,1,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,1,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,1,0,0));
         printf("  [%d] \n",determinePWM(1,1,0,0,0,1,1,0,0));
         printf("  [%d] \n",determinePWM(1,1,1,0,0,0,1,0,0));
         printf("  [%d] \n",determinePWM(1,1,0,0,0,1,1,0,0));
         printf("  [%d] \n",determinePWM(1,0,0,0,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(0,0,0,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(0,0,1,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(0,1,1,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(0,0,1,1,1,1,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,0,0,0,1,1,1,1));
         printf("  [%d] \n",determinePWM(1,1,1,1,0,0,0,1,1));

}
