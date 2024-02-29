#ifndef supplier_h
#define supplier_h

/* Library imports */

#include <string.h>
#include <stdexcept>
#include "../../utils/src/const.h"

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class Supplier{
    public:
        char *business_name;

        Supplier(char* supplier_business_name);

        ~Supplier();

        static Supplier* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif