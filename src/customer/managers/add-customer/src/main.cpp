#include "main.h"

#define READ_STREAM "read_stream"

int main() {
    redisContext *c2r;
    redisReply *reply;
    char key[100];
    char value[100];
    int numstreams;
    int k, i, h;
    char fval[100];

    c2r = redisConnect("localhost", 6379);

    // delete stream if exists
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    // initialize stream
    initStreams(c2r, READ_STREAM);
    
    add_to_stream();

    reply = RedisCommand(c2r, "XREADGROUP GROUP diameter Alice BLOCK 0 COUNT 4 STREAMS %s >", READ_STREAM);

    assertReply(c2r, reply);

    for (k=0; k < ReadNumStreams(reply); k++) {
        for (i=0; i < ReadStreamNumMsg(reply, k); i++) {
	        for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h +=  2) {
		        ReadStreamMsgVal(reply, k, i, h, key);
                ReadStreamMsgVal(reply, k, i, h, value);
                printf("key = %s, value = %s\n", key, value);
		    }	      	      
        }
    }

    freeReplyObject(reply);

    // Con2DB db("localhost", "5432", "customer", "customer", "ecommerce");

    // PGresult *res;
    
    // char query[1000];

    // sprintf(query, "INSERT INTO Customer (email, name, surname , phone_number) VALUES (\'c@gmail.com\', \'a\', \'boh\', \'+12345678\')");

    // res = db.RunQuery(query, false);

    // if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK) {
    //     db.finish();

    //     return -1;
    // }

    // db.finish();

    return 0;
}