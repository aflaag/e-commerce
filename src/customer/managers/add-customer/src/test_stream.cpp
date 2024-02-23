#include "test_stream.h"

#define READ_STREAM "read_stream"

int add_to_stream() {
    redisContext *c2r;
    redisReply *reply;

    c2r = redisConnect("localhost", 6379);

    // delete stream if exists
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    // initialize stream
    initStreams(c2r, READ_STREAM);

    reply = RedisCommand(c2r, "XADD %s * cazzo palle cazzo palle cazzo palle cazzo palle", READ_STREAM);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);

    return 0;
}