#include "test_stream.h"

#define WRITE_STREAM "add-card-in"

int add_to_stream() {
    redisContext *c2r;
    redisReply *reply;

    c2r = redisConnect("localhost", 6379);

    reply = RedisCommand(c2r, "XADD %s * client_id 1 number 1111222233335555 email user3@gmail.com", WRITE_STREAM);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);

    return 0;
}