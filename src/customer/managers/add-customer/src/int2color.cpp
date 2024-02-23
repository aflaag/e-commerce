
#include "main.h"

/* compute next state  */

void int2color(char *buf, traffic_light_type x)
{

  if (x == GREEN)
    {sprintf(buf, "GREEN");}
  else if (x == ORANGE)
    {sprintf(buf, "ORANGE");}
  else
    {sprintf(buf, "RED");}
  

}  /*   int2color()  */
 
