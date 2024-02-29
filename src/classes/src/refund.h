#ifndef refund_h
#define refun_h

/* Library imports */

#include <string.h>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <ctime>

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"
#include "../../utils/src/consts.h"

/* Classes */

class Refund{
    public:
        char *id;
        char *request_instant;
        char *assigment_instant;
        char *start;
        char *end;
        char *state;
        char *assigned_delivery;
        char *courier;

        Refund(char* refund_id, char* refund_request_instant, char* refund_state, char* refund_assigned_delivery);

        ~Refund();

        static Refund* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif