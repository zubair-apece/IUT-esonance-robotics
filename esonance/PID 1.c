
#include<stdio.h>

int left_IR_3 = 1;
    int left_IR_2=1;
int left_IR_1 =1;
int center_IR =0;
int right_IR_1 =1;
int right_IR_2 =1;
int right_IR_3 =1;
int integral;

int dt= 10;
int Kp=10;
int Ki=10;
int Kd=10;

int determinePWM(int a,int b,int c,int d,int e,int f)
{
 int ideal = 0;
int actual = a*(-3)+b*(-2)+c*(-1)+d*(1)+e*(2)+f*(3);
int error = actual - ideal;
integral+=dt*error;
int derivative = (error - prev_error)/dt;
int drive = error*Kp+integral*Ki+derivative*Kd;
int displacement = (actual/12)*60;
int pwm= 196 - pwm;

return drive;
}

main()
{
    printf("%d",determinePWM(1,1,1,0,1,1,1));


}
