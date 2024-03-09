#include "test_stream.h"

#define WRITE_STREAM "update-refund-request-in"

int add_to_stream() {
    redisContext *c2r;
    redisReply *reply;

    c2r = redisConnect("localhost", 6379);

    // initialize stream
    initStreams(c2r, WRITE_STREAM);

    reply = RedisCommand(c2r, "XADD %s * client_id 6  courier jqCr7uLJ~Oe}|d=3[33-dikbvdkEof{[@h0]NT>[Zy&&zO9tIWG?BQ.cDqW|J]o_CCy>tfbX1E6U:F)U~8^L7ms2y:i<]~x44!KjKWkJ~n;y3j;9zFFU05a(~HAV-MHu/D|2vC|?{cw?AVB3 refund 46", WRITE_STREAM);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);

    return 0;
}