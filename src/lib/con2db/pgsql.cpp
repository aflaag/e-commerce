
#include "pgsql.h"

#define DEBUG 1000


Con2DB::Con2DB(const char *hostname,
	       const char *port,
	 const char *username,
	 const char *password,
	 const char *dbname) 
{

  char buf[1000];

  sprintf(buf, "host=%s port=%s user=%s password=%s dbname=%s",
	  hostname,
	  port,
	  username,
	  password,
	  dbname
	  );
  
  // connect linux user to database (must have privileges)
      conn = PQconnectdb(buf);

  //    PGconn *conn = PQconnectdb("user=enrico dbname=sdfs");
  //  PGconn *conn = PQconnectdb("user=sdfs dbname=sdfs");    // on MPM server

    if (PQstatus(conn) != CONNECTION_OK) {
        
        fprintf(stderr, "Con2DB(%s): Connection to database failed: %s\n",
		dbname,
		PQerrorMessage(conn)
		);
        finish();
    }

#if 0
    fprintf(stderr, "Con2DB(%s): PQconnectPoll: %d\n", dbname, PQconnectPoll(conn));        
    fprintf(stderr, "Con2DB(%s): PQstatus: %d\n", dbname, PQstatus(conn));       
    fprintf(stderr, "Con2DB(%s): Connection to DB successfully established \n", dbname);        
#endif

   
}  /* Con2DB() */



void Con2DB::finish() {
    
    PQfinish(conn);
    conn = NULL;
    exit(1);
}




/* use this for commands returning no data, e.g. INSERT */
PGresult* Con2DB::ExecSQLcmd(char *sqlcmd)
{

  if (conn == NULL)
    // error
    {
     fprintf(stderr, "ExecSQLcmd(): no connection to DB: PQexec on: %s\n", sqlcmd);
          exit (1);
    }
  
#if (DEBUG > 1000000)
  fprintf(stderr, "ExecSQLcmd(): PQexec on: %s\n", sqlcmd);        
  fprintf(stderr, "ExecSQLcmd(): PQconnectPoll: %d\n", PQconnectPoll(conn));        
  fprintf(stderr, "ExecSQLcmd(): PQstatus: %d\n", PQstatus(conn));        
#endif

   
  PGresult *res = PQexec(conn, sqlcmd);    

#if (DEBUG > 1000000)
    fprintf(stderr, "ExecSQLcmd(): PQexec done\n");        
#endif
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {

      fprintf(stderr, "ExecSQLcmd(): SQL command failed: %s\n", sqlcmd);
      fprintf(stderr, "ExecSQLcmd(): %s\n",
	      PQresStatus(PQresultStatus(res)));
      fprintf(stderr, "ExecSQLcmd(): PQresultErrorMessage: %s\n",
	      PQresultErrorMessage(res) ) ;
           
        PQclear(res);
        // finish();
    }    

#if 0
    fprintf(stderr, "ExecSQLcmd() SQL command OK: %s\n", sqlcmd);        
#endif

    return (res);
}


/* use this for commands returning data, e.g. SELECT */
PGresult* Con2DB::ExecSQLtuples(char *sqlcmd)
{

  if (conn == NULL)
    // error
    {
     fprintf(stderr, "ExecSQLtuples(): no connection to DB: sqlcmd = %s\n", sqlcmd);
     exit (1);
    }

  // conn != NULL
  
  PGresult *res = PQexec(conn, sqlcmd);    
	
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {

      fprintf(stderr, "ExecSQLtuples(): SQL command failed: %s\n", sqlcmd);
      fprintf(stderr, "ExecSQLtuples(): %s\n",
	      PQresStatus(PQresultStatus(res)));

        PQclear(res);
        finish();
    }    

#if 0
    fprintf(stderr, "ExecSQLtuples() OK: %s\n", sqlcmd);        
#endif

    return (res);
}

PGresult* Con2DB::RunQuery(char* query, bool has_tuples) {
    PGresult* trans_res;
    PGresult* query_res;

    char sqlCmd[7];

    // open transaction
    sprintf(sqlCmd, "BEGIN");
    trans_res = ExecSQLcmd(sqlCmd);

    // if not ok
    if (PQresultStatus(trans_res) != PGRES_COMMAND_OK) {
        return trans_res;     // no need to PQclear()
    }

    PQclear(trans_res);

    // execute query
    query_res = !has_tuples ? ExecSQLcmd(query) : ExecSQLtuples(query);

    // close transaction
    sprintf(sqlCmd, "COMMIT");
    trans_res = ExecSQLcmd(sqlCmd);

    // if an error occurred during query execution
    if(PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK){
        PQclear(trans_res);
        return query_res;
    }
    else if(PQresultStatus(trans_res) != PGRES_COMMAND_OK){
        PQclear(query_res);
        return trans_res;
    }
    else{
        PQclear(trans_res);
        return query_res;
    }
}