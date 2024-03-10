#ifndef supplier_h
#define supplier_h

/* System libraries */

#include <string.h>
#include <stdexcept>

/* Local libraries */

#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class Supplier{
    public:
        char *business_name = NULL;

        Supplier(char* supplier_business_name);

        ~Supplier();

        static Supplier* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif