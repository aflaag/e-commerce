#include "test_stream.h"

#define WRITE_STREAM "add-rating-in"

int add_to_stream() {
    redisContext *c2r;
    redisReply *reply;

    c2r = redisConnect("localhost", 6379);

    // initialize stream
    initStreams(c2r, WRITE_STREAM);

    reply = RedisCommand(c2r, "XADD %s * client_id 1 product P1 customer user1@gmail.com stars 3", WRITE_STREAM);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);

    return 0;
}