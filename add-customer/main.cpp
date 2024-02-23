
#include "main.h"


int main()
{
Con2DB db1("localhost", "5432", "customer", "customer", "ecommerce");

 PGresult *res;
 char sqlCmd [1000];
 int vid;

 sprintf(sqlCmd, "BEGIN");
 res = db1.ExecSQLcmd(sqlCmd);
 PQclear(res);

 sprintf(sqlCmd, "INSERT INTO Customer (email, name, surname , phone_number) VALUES (\'a@gmail.com\', \'a\', \'boh\', \'+12345678\')");
 res = db1.ExecSQLcmd(sqlCmd);
 PQclear(res);

 sprintf(sqlCmd, "COMMIT");
 res = db1.ExecSQLcmd(sqlCmd);
 PQclear(res);
 
 db1.finish();
 
}  /*  main()  */









