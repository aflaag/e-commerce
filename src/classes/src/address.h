#ifndef address_h
#define address_h

/* Library imports */

#include <string.h>
#include <stdexcept>
#include "../../utils/src/consts.h"

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class Address{
    public:
        char *email;
        char *zip_code;
        char *street;
        char *street_number;
        char *city;

        Address(char* address_owner_email, char* address_zip_code, char* address_street, char* address_steet_number, char* address_city);

        ~Address();

        static Address* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif