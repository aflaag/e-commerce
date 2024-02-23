#include "main.h"

int main() {
    Con2DB db("localhost", "5432", "customer", "customer", "ecommerce");

    PGresult *res;
    
    char query[1000];

    sprintf(query, "INSERT INTO Customer (email, name, surname , phone_number) VALUES (\'c@gmail.com\', \'a\', \'boh\', \'+12345678\')");

    res = db.RunQuery(query, false);

    if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK) {
        db.finish();

        return -1;
    }

    db.finish();

    return 0;
}