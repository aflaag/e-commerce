
#include "main.h"


/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    
    return res;  

#if 0
    if (res <= 0)
      return ((int) res);  // sleep completed or signal received
    else // remaining sleep time
      return (ts.tv_nsec);
#endif
    
}


/* msleep(): Sleep for the requested number of microseconds. */
int micro_sleep(long usec)
{
    struct timespec ts;
    int res;

    if (usec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    
    return res;  

}



int long get_nanos(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ((long) (ts.tv_sec * 1000000000L + ts.tv_nsec));
}


long int get_day_nanos(char* buf) {
   struct tm *info;
   struct timespec ts;

   timespec_get(&ts, TIME_UTC);
   
   info = localtime( &(ts.tv_sec) );
   // ISO	ISO 8601, SQL standard	1997-12-17 07:37:16-08
   sprintf(buf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
	   info -> tm_year + 1900,
	   info -> tm_mon + 1,
	   info -> tm_mday,
	   info -> tm_hour,
	   info -> tm_min,
	   info -> tm_sec
	   );

   return (ts.tv_nsec);
}


long int nanos2day(char* buf, long int nanosec) {
   struct tm *info;
   struct timespec ts;

#if (DEBUG > 1000000)
   fprintf(stderr, "nanos2day(): nanosec = %ld\n", nanosec);
#endif
   
   ts.tv_sec = nanosec/1000000000L;
   ts.tv_nsec = (nanosec % 1000000000L);
       
#if (DEBUG > 1000000)
   fprintf(stderr, "nanos2day(): ts.tv_sec = %ld, ts.tv_nsec = %ld, \n", ts.tv_sec, ts.tv_nsec);
#endif


   /* info is not NULL */
   
   info = localtime( &(ts.tv_sec) );

#if (DEBUG > 1000000)
   fprintf(stderr, "nanos2day(): info computed\n");
#endif
   
   // ISO	ISO 8601, SQL standard	1997-12-17 07:37:16-08
   sprintf(buf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
	   info -> tm_year + 1900,
	   info -> tm_mon + 1,
	   info -> tm_mday,
	   info -> tm_hour,
	   info -> tm_min,
	   info -> tm_sec
	   );

#if (DEBUG > 1000000)
   fprintf(stderr, "nanos2day(): day: %s, nanosec after sec = %ld\n", buf, ts.tv_nsec);
#endif

     
   return (ts.tv_nsec);
}
