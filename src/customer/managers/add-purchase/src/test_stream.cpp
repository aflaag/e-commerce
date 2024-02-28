#include "test_stream.h"

#define WRITE_STREAM "add-purchase-in"

int add_to_stream() {
    redisContext *c2r;
    redisReply *reply;

    c2r = redisConnect("localhost", 6379);


    // initialize stream
    initStreams(c2r, WRITE_STREAM);

    reply = RedisCommand(c2r, "XADD %s * client_id 1 fare 0.0 email user1@gmail.com card 1234567890123456 zip_code 01028 street boh street_number 1a product P1 quantity 10 product P2 quantity 20", WRITE_STREAM);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);

    return 0;
}