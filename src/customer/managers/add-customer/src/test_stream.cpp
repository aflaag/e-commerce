#include "test_stream.h"

#define WRITE_STREAM "add-customer-in"

int add_to_stream() {
    redisContext *c2r;
    redisReply *reply;

    c2r = redisConnect("localhost", 6379);

    reply = RedisCommand(c2r, "XADD %s * client_id 1 name example surname example email sara@gmail.com phone_number +34567453", WRITE_STREAM);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);

    return 0;
}