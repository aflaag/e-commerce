
#include "main.h"

/* initialize time  */

void update_time()
{
  
   nanos = get_nanos();

   timeadvance = ((double) (nanos - last_nanos))/((double) (1000000000L));
   global_time_sec = global_time_sec + timeadvance;
	 
   //  printf("main: elapsed time = %ld ns = %lf sec\n", nanos - last_nanos, timeadvance);
 
   last_nanos = nanos;
 
}  /* update_time()  */
 
