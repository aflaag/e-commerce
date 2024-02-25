#include "main.h"

void send_response_status(redisContext* c2r, const char *stream, const char *client_id, const char *resp_status, const char *msg_id){
    redisReply *reply;

    // LOG
    printf("Response: %s %s %s %s\n", stream, client_id, resp_status, msg_id);

    reply = RedisCommand(c2r, "XACK %s %s %s", stream, resp_status, msg_id);
    assertReplyType(c2r, reply, REDIS_REPLY_INTEGER);
    freeReplyObject(reply);

    reply = RedisCommand(c2r, "XADD %s * client_id %s resp_status %s", WRITE_STREAM, client_id, resp_status);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);
}

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res;
    
    char query[1000], response[100], msg_id[30], first_key[30], client_id[30];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    // delete stream if exists
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    // initialize stream
    initStreams(c2r, READ_STREAM, "add-customer");
    initStreams(c2r, WRITE_STREAM, "manager");
    
    add_to_stream();

    Customer* customer;

    while(1) {
        reply = RedisCommand(c2r,
            "XREADGROUP GROUP %s manager BLOCK 0 COUNT 1 STREAMS %s >", MANAGER_GROUP, READ_STREAM);

        assertReply(c2r, reply);

        if (ReadNumStreams(reply) == 0) {
            continue;
        } 

        // Only one stream --> stream_num = 0
        // Only one message in stream --> msg_num = 0
        ReadStreamNumMsgID(reply, 0, 0, msg_id);

        // Check if the first key/value pair is the client_id
        ReadStreamMsgVal(reply, 0, 0, 0, first_key);    // Index of first field of msg = 0
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);    // Index of second field of msg = 1

        printf("%s\n", first_key);
        if(strcmp(first_key, "client_id")){
            send_response_status(c2r, WRITE_STREAM, client_id, "INVALID#CLIENT#ID", msg_id);
            continue;
        }
        
        // Convert request
        try{
            customer = Customer::from_stream(reply, 0, 0);
        }
        catch(std::invalid_argument exp){
            send_response_status(c2r, WRITE_STREAM, client_id, "INVALID#REQUEST", msg_id);
            continue;
        }

        // Run query
        sprintf(query, "INSERT INTO Customer (email, name, surname , phone_number) VALUES (\'%s\', \'%s\', \'%s\', \'%s\')", 
                        customer->email, customer->name, customer->surname, customer->phone_number);

        query_res = db.RunQuery(query, false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id);
            continue;
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "INSERT#SUCCESS", msg_id);
    }

    db.finish();

    return 0;
}