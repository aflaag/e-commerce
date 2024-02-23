
#include "main.h"

/* buy stock  */

void log2db(Con2DB db1, int pid, long int nanosec, traffic_light_type x)
{

 PGresult *res;
 int rows, k;
 char trcolor[20];
 int vid = 0;
 long int dbnanosec, nsafters;
 char datebuf[1000];
   
 /*  init  */
 int2color(trcolor, x);

 #if (DEBUG > 1000000)
 fprintf(stderr, "log2db(): pid = %d, varname = %s\n", pid, "x");
 #endif
   
 sprintf(sqlcmd, "SELECT vid FROM TimeVar where ((pid = %d) AND (varname = \'x\'))", pid);
   res = db1.ExecSQLtuples(sqlcmd);
   vid = atoi(PQgetvalue(res, 0, PQfnumber(res, "vid")));
   PQclear(res);

 #if (DEBUG > 1000000)
   fprintf(stderr, "log2db(): vid = %d\n", vid);
 #endif

   
      sprintf(sqlcmd, "BEGIN"); 
      res = db1.ExecSQLcmd(sqlcmd);
      PQclear(res);

      
  sprintf(sqlcmd,
"INSERT INTO LogTable VALUES (%ld, %d, %d, \'%s\') ON CONFLICT DO NOTHING",
	  nanosec,
	  vid,
	  x,
	  trcolor
	  );

  res = db1.ExecSQLcmd(sqlcmd);
     PQclear(res);
     
 sprintf(sqlcmd, "COMMIT"); 
      res = db1.ExecSQLcmd(sqlcmd);
      PQclear(res);

      
#if (DEBUG > 0)

      // fprintf(stderr, "log2db(): check insertion\n");
    
    sprintf(sqlcmd, "SELECT * FROM LogTable where (nanosec = %ld)", nanosec);

    res = db1.ExecSQLtuples(sqlcmd);
    rows = PQntuples(res);

    dbnanosec = strtol(PQgetvalue(res, 0, PQfnumber(res, "nanosec")), NULL, 10);
    
    fprintf(stderr, "log2db(): inserted in LogTable (%ld, %d, %d, \'%s\')\n",
	    dbnanosec,
	    atoi(PQgetvalue(res, 0, PQfnumber(res, "vid"))),
	    atoi(PQgetvalue(res, 0, PQfnumber(res, "varvalue"))),
	    PQgetvalue(res, 0, PQfnumber(res, "loginfo"))
	    );
    PQclear(res);

    nsafters = nanos2day(datebuf, dbnanosec);
    
    fprintf(stderr, "log2db(): ns = %ld = TIME_UTC = %s + %ld ns\n", dbnanosec, datebuf, nsafters);
#endif

   
 

      
 }  /*   log2db()  */
 
