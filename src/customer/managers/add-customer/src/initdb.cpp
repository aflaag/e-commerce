
#include "main.h"

/* buy stock  */

void init_logdb(Con2DB db1, int pid)
{

 PGresult *res;
 int rows, k;
 
 
 /*  init  */
 
      sprintf(sqlcmd, "BEGIN"); 
      res = db1.ExecSQLcmd(sqlcmd);
      PQclear(res);

  sprintf(sqlcmd,
"INSERT INTO Timevar VALUES (DEFAULT, %d, \'%s\', \'%s\', \'%s\', \'%s\') ON CONFLICT DO NOTHING",
	  pid, "Traffic Light", "x", "discrete", "Values: green, orange, red");

  res = db1.ExecSQLcmd(sqlcmd);
     PQclear(res);
     
 sprintf(sqlcmd, "COMMIT"); 
      res = db1.ExecSQLcmd(sqlcmd);
      PQclear(res);

      
#if (DEBUG > 0)
      sprintf(sqlcmd, "SELECT * FROM Timevar where ((pid = %d) AND (varname = 'x'))", pid);

    res = db1.ExecSQLtuples(sqlcmd);
    rows = PQntuples(res);
      
    fprintf(stderr, "initdb(): inserted in Timevar (%d, %d, %s, %s, %s, %s)\n",
	    atoi(PQgetvalue(res, 0, PQfnumber(res, "vid"))),
	    atoi(PQgetvalue(res, 0, PQfnumber(res, "pid"))),
	    PQgetvalue(res, 0, PQfnumber(res, "sysname")),
	    PQgetvalue(res, 0, PQfnumber(res, "varname")),
	    PQgetvalue(res, 0, PQfnumber(res, "vardomain")),
	    PQgetvalue(res, 0, PQfnumber(res, "varinfo"))
	    );
#endif


     
 }  /*   init_logdb()  */
 
