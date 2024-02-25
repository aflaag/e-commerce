#include "main.h"

#define READ_STREAM "handler-add-customer"
#define WRITE_STREAM "add-customer-handler"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *res;
    int num_streams, k, i;
    
    char query[1000];
    char response[100];
    char id[30];

    bool invalid_entry;

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

    Customer* customer;

    while(1) {
        reply = RedisCommand(c2r,
            "XREADGROUP GROUP diameter Alice BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(c2r, reply);

        num_streams = ReadNumStreams(reply);

        if (num_streams == 0) {
            continue;
        } 
        
        for (k=0; k < num_streams; k++) {
            for (i=0; i < ReadStreamNumMsg(reply, k); i++) {
                ReadStreamNumMsgID(reply, k, i, id);
                printf("%s\n", id);
                
                try{
                    customer = Customer::from_stream(reply, k, i);
                }
                catch(std::invalid_argument exp){
                    printf("Invalid entry - Aborting insert\n");
                    invalid_entry = true;
                    continue;
                }

                invalid_entry = true;
            }
        }

        if(invalid_entry){
            continue;
        }

        freeReplyObject(reply);

        sprintf(query, "INSERT INTO Customer (email, name, surname , phone_number) VALUES (\'%s\', \'%s\', \'%s\', \'%s\')", 
                        customer->email, customer->name, customer->surname, customer->phone_number);

        res = db.RunQuery(query, false);

        reply = RedisCommand(c2r, "XACK %s diameter %s", WRITE_STREAM, id);
        assertReplyType(c2r, reply, REDIS_REPLY_INTEGER);
        freeReplyObject(reply);

        if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK) {
            // gestione disconnessione db o per il meme ci connettiamo sempre
            sprintf(response, "BAD REQUEST");
        } else {
            sprintf(response, "SUCCESSFUL REQUEST");
        }

        printf("%s\n", response);

        reply = RedisCommand(c2r, "XADD %s * response %s", WRITE_STREAM);
        assertReplyType(c2r, reply, REDIS_REPLY_STRING);
        freeReplyObject(reply);
    }

    db.finish();

    return 0;
}