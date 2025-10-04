#include "Transmitter.h"



/***************888Functions Definitions Section Start*****************/
void adjust_joystick_values(unsigned short *value, unsigned char is_inverse)
{
 
  if((*value)>=JOYSTICK_LOW_DEADBAND &&(*value)<=JOYSTICK_HIGH_DEADBAND)
  {
    *value=JOYSTICK_CENTER_VALUE;
  }
  if(is_inverse==1)
  {
    if(*value <JOYSTICK_CENTER_VALUE)
    {
      *value+=(((JOYSTICK_CENTER_VALUE-*value)*2)+1);
    }
    else if(*value >JOYSTICK_CENTER_VALUE)
    {
      *value-=(((*value-JOYSTICK_CENTER_VALUE-1)*2)+1);
    }
  }
  


}



/***************888Functions Definitions Section End*****************/