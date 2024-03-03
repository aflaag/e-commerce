#ifndef refunded_product_h
#define refunded_product_h

/* Library imports */

#include <string.h>
#include <stdexcept>
#include "../../utils/src/const.h"

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class RefundedProduct {
    public:
        char *refund_request = NULL;
        char *product = NULL;
        char *quantity = NULL;

        RefundedProduct(char* refund_request_in, char* refunded_product, char* refund_quantity);

        ~RefundedProduct();

        static RefundedProduct* from_stream(redisReply* reply, int stream_num, int msg_num, int starting_value, char* purchase);
        std::string to_insert_query();
};

#endif
