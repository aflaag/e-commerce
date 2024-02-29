#ifndef ordered_product_h
#define ordered_product_h

/* Library imports */

#include <string.h>
#include <stdexcept>
#include "../../utils/src/const.h"

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class OrderedProduct {
    public:
        char *product;
        char *purchase;
        char *quantity;

        OrderedProduct(char* order_product, char* order_purchase, char* order_quantity);

        ~OrderedProduct();

        static OrderedProduct* from_stream(redisReply* reply, int stream_num, int msg_num, int starting_value, char* purchase);
        std::string to_insert_query();
};

#endif
