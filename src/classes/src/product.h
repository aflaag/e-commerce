#ifndef product_h
#define product_h

/* System libraries */

#include <string.h>
#include <stdexcept>

/* Local libraries */

#include "../../lib/con2redis/src/con2redis.h"
#include "../../utils/src/const.h"
#include "../../utils/src/utils.h"

/* Classes */

class Product {
    public:
        char *name = NULL;
        char *code = NULL;
        char *description = NULL;
        char *price = NULL;

        Product(char* code, char* name, char* description, char* price);
        Product(char* code, char* description);

        ~Product();

        static Product* from_stream(redisReply* reply, int stream_num, int msg_num);
        static Product* update_from_stream(redisReply* reply, int stream_num, int msg_num);
        std::string to_insert_query();
};

#endif
