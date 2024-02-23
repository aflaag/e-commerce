
#include "main.h"

/* compute next state  */

traffic_light_type next(traffic_light_type x)
{


  return ((traffic_light_type) ((x + rand()%2)%3));
  

  /*
  In fact:

  
  If (x == GREEN) traffic light stays green or become orange
In this case x == 0.
Thus  
(x + rand()%2) is 0 or 1 
and thus
((x + rand()%2)%3) is 0 or 1, 
that, is x GREEN or ORANGE.

If (x == ORANGE) traffic light stays orange or becomes red.
In this case x == 1.
Thus
(x + rand()%2) is 1 or 2
and thus
((x + rand()%2)%3) is 1 or 2, 
that, is x ORANGE or RED.

If (x == RED) traffic light stays RED or becomes GREEN.
In this case x == 2.
Thus
(x + rand()%2) is 2 or 3
and thus
((x + rand()%2)%3) is 2 or 0, 
that, is x RED or GREEN.

  */


  
}  /*   next()  */
 
