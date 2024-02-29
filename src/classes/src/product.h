#ifndef product_h
#define product_h

/* Library imports */

#include <string.h>
#include <stdexcept>
#include "../../utils/src/const.h"

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

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