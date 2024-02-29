#ifndef product_h
#define product_h

/* Library imports */

#include <string.h>
#include <stdexcept>

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"
#include "../../utils/src/const.h"
#include "../../utils/src/utils.h"

/* Classes */

class Product {
    public:
        char *name;
        char *code;
        char *description;
        char *price;

        Product(char* code, char* name, char* description, char* price);

        ~Product();

        static Product* from_stream(redisReply* reply, int stream_num, int msg_num, int starting_value);
        std::string to_insert_query();
};

#endif
