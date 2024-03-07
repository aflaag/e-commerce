#include "main.h"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res;

    std::string query;

    char response[100], msg_id[30], first_key[30], client_id[30];

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
    initStreams(c2r, READ_STREAM);
    initStreams(c2r, WRITE_STREAM);
    
    add_to_stream(); // DA TOGLIERE

    UpdateRefund* update_refund;
    
    while(1) {

        reply = RedisCommand(c2r, "XREADGROUP GROUP main courier BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

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

        if(strcmp(first_key, "client_id")){
            send_response_status(c2r, WRITE_STREAM, client_id, "INVALID#CLIENT#STREAM", msg_id, 0);
            continue;
        }

        // Convert request
        try{
            printf("ciao\n");
            update_refund = UpdateRefund::from_stream(reply, 0, 0, 2);
        }
        catch(std::invalid_argument exp){
            send_response_status(c2r, WRITE_STREAM, client_id, "INVALID#REQUEST", msg_id, 0);
            continue;
        }

        printf("ciao\n");
        

        query = update_refund->to_update_query();
        
        query_res = db.RunQuery((char *) query.c_str(), false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id, 0);
            continue;
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST#SUCCESS", msg_id, 0);
        
    }

    db.finish();

    return 0;
}