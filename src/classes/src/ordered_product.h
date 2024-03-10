#ifndef ordered_product_h
#define ordered_product_h

/* System libraries */

#include <string.h>
#include <stdexcept>

/* Local libraries */

#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class OrderedProduct {
    public:
        char *product = NULL;
        char *purchase = NULL;
        char *quantity = NULL;

        OrderedProduct(char* order_product, char* order_purchase, char* order_quantity);

        ~OrderedProduct();

        static OrderedProduct* from_stream(redisReply* reply, int stream_num, int msg_num, int starting_value, char* purchase);
        std::string to_insert_query();
};

#endif
