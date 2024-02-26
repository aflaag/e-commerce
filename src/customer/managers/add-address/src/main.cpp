#include "main.h"

#define READ_STREAM "handler-add-address"
#define WRITE_STREAM "add-address-handler"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *res;
    int k, i, h;
    
    char query[1000];

    char key[100];
    char value[100];

    char zip_code[100];
    char email[100];
    char street[100];
    char street_number[100];
    char city[100];
    char response[100];

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
        reply = RedisCommand(c2r,
            "XREADGROUP GROUP diameter Alice BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(c2r, reply);

        char id[30];
        int A = ReadNumStreams(reply);

        if (A == 0) {
            continue;
        } 

        for (k=0; k < A; k++) {
            for (i=0; i < ReadStreamNumMsg(reply, k); i++) {
                ReadStreamNumMsgID(reply, k, i, id);
                printf("%s\n", id);

                for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h +=  2) {
                    ReadStreamMsgVal(reply, k, i, h, key);
                    ReadStreamMsgVal(reply, k, i, h + 1, value);
                    
                    if (!strcmp(key, "email")) {
                        sprintf(email, "%s", value);
                    } else if (!strcmp(key, "zip_code")) {
                        sprintf(zip_code, "%s", value);
                    } else if (!strcmp(key, "street")) {
                        sprintf(street, "%s", value);
                    } else if (!strcmp(key, "street_number")) {
                        sprintf(street_number, "%s", value);
                    } else if (!strcmp(key, "city")) {
                        sprintf(city, "%s", value);
                    } else {
                        printf("%s\n", key);
                        return 1;
                    }
                }	      	      
            }
        }

        freeReplyObject(reply);

        sprintf(query, "INSERT INTO Address ON CONFLICT DO NOTHING (zip_code, street, street_number , city) VALUES (\'%s\', \'%s\', \'%s\', %s);"
                       "INSERT INTO AddCust (customer, zip_code, street, street_number) VALUES (\'%s\', \'%s\', \'%s\', \'%s\')", 
                        zip_code, street, street_number, city, email, zip_code, street, street_number);

        res = db.RunQuery(query, false);

        reply = RedisCommand(c2r, "XACK %s diameter %s", WRITE_STREAM, id);
        assertReplyType(c2r, reply, REDIS_REPLY_INTEGER);
        freeReplyObject(reply);
        printf("ciao\n");

        if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK) {
            // gestione disconnessione db o per il meme ci connettiamo sempre
            sprintf(response, "BAD REQUEST");
        } else {
            sprintf(response, "SUCCESSFUL REQUEST");
        }

        reply = RedisCommand(c2r, "XADD %s * response %s", WRITE_STREAM);
        assertReplyType(c2r, reply, REDIS_REPLY_STRING);
        freeReplyObject(reply);

        micro_sleep(1000);
    }

    db.finish();

    return 0;
}