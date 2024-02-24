#include "main.h"

#define READ_STREAM "handler-add-customer"
#define WRITE_STREAM "add-customer-handler"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *res;
    
    char query[1000];

    char name[100];
    char email[100];
    char surname[100];
    char phone_number[100];
    char response[100];
    int k, i, h;
    char key[100];
    char value[100];


    Con2DB db("localhost", "5432", "customer", "customer", "ecommerce");
    c2r = redisConnect("localhost", 6379);

    // delete stream if exists
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);
    reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    // initialize stream
    initStreams(c2r, READ_STREAM);
    initStreams(c2r, WRITE_STREAM);
    
    add_to_stream();

    while(1) {
        reply = RedisCommand(c2r, "XREADGROUP GROUP diameter Alice BLOCK 0 COUNT 4 STREAMS %s >", READ_STREAM);

        assertReply(c2r, reply);

        for (k=0; k < ReadNumStreams(reply); k++) {
            for (i=0; i < ReadStreamNumMsg(reply, k); i++) {
                for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h +=  2) {
                    ReadStreamMsgVal(reply, k, i, h, key);
                    ReadStreamMsgVal(reply, k, i, h, value);
                    
                    if (key == "name") {
                        sprintf(name, "%s", value);
                    } else if (key == "email") {
                        sprintf(email, "%s", value);
                    } else if (key == "surname") {
                        sprintf(surname, "%s", value);
                    } else if (key == "phone_number") {
                        sprintf(phone_number, "%s", value);
                    } else {
                        printf("%s\n", key);
                        return 1;
                    }
                }	      	      
            }
        }

        freeReplyObject(reply);

        sprintf(query, "INSERT INTO Customer (email, name, surname , phone_number) VALUES (\'%s\', \'%s\', \'%s\', \'%s\')", 
                        email, name, surname, phone_number);

        res = db.RunQuery(query, false);

        if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK) {
            // gestione disconnessione db o per il meme ci connettiamo sempre
            sprintf(response, "BAD REQUEST");
        } else {
            sprintf(response, "SUCCESSFUL REQUEST");
        }

        reply = RedisCommand(c2r, "XADD %s * response %s", WRITE_STREAM);
        assertReplyType(c2r, reply, REDIS_REPLY_STRING);
        freeReplyObject(reply);

        // continuiamo con tuple se dobbiamo ritornare qualcosa
    }

    db.finish();

    return 0;
}