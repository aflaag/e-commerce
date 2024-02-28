#ifndef ordered_products_h
#define ordered_products_h

/* Library imports */

#include <string.h>
#include <stdexcept>

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class Ordered_products{
    public:
        char *product;
        char *purchase;
        char *quantity;

        Ordered_products(char* order_product, char* order_purchase, char* order_quantity);

        ~Ordered_products();

        static Ordered_products* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif