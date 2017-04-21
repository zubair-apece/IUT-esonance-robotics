


void main(){

while(1)
{
    if(/*no  obstacle in front*/)
    {
    //continue solving maze
        if(/*front sensor is not sensing && left most sensor is not sensing && rightmost sensor is not sensing && centre sensor is sensing --------dead end*/)
        {
            if(/*goForward15cm()*/)
            {
                //go forward;
            }
            else
            {
                //turn 180 deg;
                //go forward;
            }



        }
        if(/*front sensor is sensing & left most sensor is not sensing & right most sensor is sensing & centre sensor is sensing*/)
        {
            //go forward;

        }
        else if(/*all back sensors are sensing || (front sensor is sensing & left most sensor is sensing & centre sensor is sensing)*/)
        {
            // go 90 deg left
        }
        else
        {
            //goForwardPID
        }
        else


        else if()
    }
    else
    {
        //go_round_the_obstacle()
    }


}
}
