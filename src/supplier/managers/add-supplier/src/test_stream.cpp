#include "test_stream.h"

#define WRITE_STREAM2 "handler-add-supplier"

int add_to_stream() {
    redisContext *c2r;
    redisReply *reply;

    c2r = redisConnect("localhost", 6379);

    // delete stream if exists
    // reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM2);
    // assertReply(c2r, reply);
    // dumpReply(reply, 0);

    // initialize stream
    initStreams(c2r, WRITE_STREAM2);

    reply = RedisCommand(c2r, "XADD %s * business_name supplier5", WRITE_STREAM2);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);

    return 0;
}