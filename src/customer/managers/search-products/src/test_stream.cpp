#include "test_stream.h"

#define WRITE_STREAM "search-products-in"
#define READ_STREAM "search-products-out"

int add_to_stream() {
    redisContext *c2r;
    redisReply *reply;

    c2r = redisConnect("localhost", 6379);

    // initialize stream
    initStreams(c2r, WRITE_STREAM);

    reply = RedisCommand(c2r, "XADD %s * client_id 1 product rt", WRITE_STREAM);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);
    freeReplyObject(reply);

    return 0;
}

int read_from_stream(){

    redisContext *c2r;
    redisReply *reply;

    c2r = redisConnect("localhost", 6379);

    char first_key[30], client_id[100], msg_id[100], str_rows[100];

    int rows;

    initStreams(c2r, READ_STREAM);

    reply = RedisCommand(c2r, "XREADGROUP GROUP main customer BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

    assertReply(c2r, reply);

    // Only one stream --> stream_num = 0
    // Only one message in stream --> msg_num = 0
    ReadStreamNumMsgID(reply, 0, 0, msg_id);

    // Check if the first key/value pair is the client_id
    ReadStreamMsgVal(reply, 0, 0, 0, first_key);    // Index of first field of msg = 0
    ReadStreamMsgVal(reply, 0, 0, 1, client_id);    // Index of second field of msg = 1

    // Check if the first key/value pair is the client_id
    ReadStreamMsgVal(reply, 0, 0, 5, str_rows);    // Index of second field of msg = 1

    printf("\n\nrows number : %d\n",atoi(str_rows));

    freeReplyObject(reply);

    return 0;
}