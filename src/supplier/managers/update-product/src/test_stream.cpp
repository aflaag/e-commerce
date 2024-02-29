#include "test_stream.h"

#define WRITE_STREAM "update-product-in"

int add_to_stream() {
    redisContext *c2r;
    redisReply *reply;

    c2r = redisConnect("localhost", 6379);


    // initialize stream
    initStreams(c2r, WRITE_STREAM);

    reply = RedisCommand(c2r, "XADD %s * client_id 1 code P1 description ho##capito", WRITE_STREAM);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);

    return 0;
}
