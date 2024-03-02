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
#include "../../utils/src/const.h"

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
        char *purchase;

        Refund(char* refund_id, char* refund_request_instant, char* refund_state, char* refund_assigned_delivery);
        Refund(char* refund_id, char* request_instant_, char* refund_assignment_instant_, char* refund_start_, char* refund_end_, char* refund_state_, char* courier_, char* purchase);

        ~Refund();

        static Refund* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif