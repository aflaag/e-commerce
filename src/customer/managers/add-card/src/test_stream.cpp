#include "test_stream.h"

#define WRITE_STREAM "add-card-in"

int add_to_stream() {
    redisContext *c2r;
    redisReply *reply;

    c2r = redisConnect("localhost", 6379);

    // delete stream if exists
    // reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM2);
    // assertReply(c2r, reply);
    // dumpReply(reply, 0);

    // initialize stream
    initStreams(c2r, WRITE_STREAM);

    reply = RedisCommand(c2r, "XADD %s * client_id 1 number 1111222233335555 email sara@gmail.com", WRITE_STREAM);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);

    return 0;
}