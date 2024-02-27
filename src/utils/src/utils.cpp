#include "utils.h"

void send_response_status(redisContext* c2r, const char *stream, const char *client_id, const char *resp_status, const char *msg_id){
    redisReply *reply;

    // LOG
    printf("Response: %s %s %s %s\n", stream, client_id, resp_status, msg_id);

    reply = RedisCommand(c2r, "XADD %s * client_id %s resp_status %s", stream, client_id, resp_status);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);
}