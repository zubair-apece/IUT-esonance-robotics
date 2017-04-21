
#include<stdio.h>

int left_IR_3 = 1;
    int left_IR_2=1;
int left_IR_1 =1;
int center_IR =0;
int right_IR_1 =1;
int right_IR_2 =1;
int right_IR_3 =1;
int integral=0;
int prev_error=0;

int dt= 10;
int Kp=50;
int Ki=2;
int Kd=200;
int max_drive=14*Kp+10*2;

int count=0;
int err[10]={};

int determinePWM(int a,int b,int c,int d,int e,int f,int g)
{
    integral=0;
 int ideal = 0;
int actual = a*(-3*3)+b*(-2*2)+c*(-1*1)+e*(1)+f*(2*2)+g*(3*3);
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
printf("\n");
printf("%d %d %d %d %d %d %d\n",a,b,c,d,e,f,g);
err[0]=error;

//integral=integral+error*dt;
int derivative = (error - prev_error)/dt;
int drive = error*Kp+integral*Ki;
int displacement = ((float)drive/(float)max_drive)*(float)60;
int pwm= 196 - displacement;
prev_error=error;
printf("ideal %d  actual %d  error %d  integral %d  derivative %d  drive %d  displacement %d\n",ideal,actual,error,integral,derivative,drive,displacement);
return pwm;
}

main()
{
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,1,0,0,0));
    printf("%d\n",determinePWM(1,1,1,0,0,0,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));
    printf("%d\n",determinePWM(1,1,1,0,1,1,1));


}
