#ifndef refunded_product_h
#define refunded_product_h

/* System libraries */

#include <string.h>
#include <stdexcept>

/* Local libraries */

#include "../../utils/src/const.h"
#include "../../utils/src/utils.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class UpdateRefund {
    public:
        char *refund_request = NULL;
        char *new_time = NULL;
        char *courier = NULL;
        char *state = NULL;

        UpdateRefund(char* refund_request_in, char* timestamp, char* next_state);
        UpdateRefund(char* refund_request_in, char* courier_id);

        ~UpdateRefund();

        static UpdateRefund* from_stream(redisReply* reply, int stream_num, int msg_num, int starting_value);
        std::string to_update_query();
};

#endif
