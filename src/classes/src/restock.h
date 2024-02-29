#ifndef restock_h
#define restock_h

/* Library imports */

#include <string.h>
#include <stdexcept>

/* Local imports */

#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class Restock {
    public:
        char *id;
        char *quantity;
        char *supplier;
        char *product;

        Restock(char* restock_id, char* restock_quantity, char* restock_supplier, char* restock_product);

        ~Restock();

        static Restock* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif
