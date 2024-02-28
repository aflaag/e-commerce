#include "customer_handler.h"

#define MAX_MANAGER_STREAM 20

int main(){
    redisContext *c2r;
    redisReply *reply;

    char next_valPARAMETERS_LEN;

    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    // delete stream if exists
    reply = RedisCommand(c2r, "DEL %s", READ_CLIENT_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    reply = RedisCommand(c2r, "DEL %s", WRITE_CLIENT_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    // initialize stream
    initStreams(c2r, READ_CLIENT_STREAM);
    initStreams(c2r, WRITE_CLIENT_STREAM);

    reply = RedisCommand(c2r, "XADD %s * request_type add-manager name example surname example email sara@gmail.com phone_number +34567453", READ_CLIENT_STREAM);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);

    while(1){
        // Serve clients
        reply = RedisCommand(c2r, "XREAD BLOCK 0 COUNT 1 STREAMS %s $", READ_CLIENT_STREAM);
        assertReply(c2r, reply);

        if (ReadNumStreams(reply) == 0) {
            continue;
        }

        ReadStreamNumMsgID(reply, 0, 0, msg_id);

        // Check if the first key/value pair is the client_id
        ReadStreamMsgVal(reply, 0, 0, 0, first_key);        // Index of first field of msg = 0
        ReadStreamMsgVal(reply, 0, 0, 1, request_type);     // Index of second field of msg = 1

        // DA QUI

        if(strcmp(first_key, "request_type")){
            reply = RedisCommand(c2r, "XADD %s * resp_status %s", WRITE_CLIENT_STREAM, "INVALID#REQUEST");
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);
            continue;
        }
        
        char* manager_req = (char*) malloc(sizeof(char) * );

        if(!strcmp(request_type, "add-customer")){
            snprintf(manager_req, MAX_MANAGER_STREAM, "XADD %s client_id %s", WRITE_ADD_CUSTOMER_STREAM, WRITE_CLIENT_STREAM);
        }
        // else if(!strcmp(request_type, "add-card")){
        //     snprintf(manager_req, MAX_MANAGER_STREAM, "XADD %s", WRITE_ADD_CUSTOMER_STREAM);
        // }
        else{
            reply = RedisCommand(c2r, "XADD %s * resp_status %s", WRITE_CLIENT_STREAM, "INVALID#REQUEST#TYPE");
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);
            continue;
        }

        for(int i = 2; i < ReadStreamMsgNumVal(reply, 0, 0); i++){
            ReadStreamMsgVal(reply, 0, 0, i, next_val);

            int final_len = strlen(manager_req) + 1 + strlen(next_val) + 1;
            char* tmp = manager_req;

            manager_req = (char*) calloc(sizeof(char), final_len);
            snprintf(manager_req, final_len, "%s %s", tmp, next_val);
            free(tmp);
        }

        freeReplyObject(reply);

        reply = RedisCommand(c2r, "%s", manager_req);
        assertReplyType(c2r, reply, REDIS_REPLY_STRING);
        freeReplyObject(reply);
        free(manager_req);
    }

    return 0;
}
