#ifndef refunded_product_h
#define refunded_product_h

/* Library imports */

#include <string.h>
#include <stdexcept>
#include "../../utils/src/const.h"
#include "../../utils/src/utils.h"

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class UpdateRefund {
    public:
        char *refund_request;
        char *new_time;
        char *courier;
        char *state;

        UpdateRefund(char* refund_request_in, char* timestamp, char* next_state);
        UpdateRefund(char* refund_request_in, char* courier_id);

        ~UpdateRefund();

        static UpdateRefund* from_stream(redisReply* reply, int stream_num, int msg_num, int starting_value);
        std::string to_update_query();
};

#endif
