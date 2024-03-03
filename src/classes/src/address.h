#ifndef address_h
#define address_h

/* Library imports */

#include <string.h>
#include <stdexcept>
#include "../../utils/src/const.h"

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class Address{
    public:
        char *email = NULL;
        char *zip_code = NULL;
        char *street = NULL;
        char *street_number = NULL;
        char *city = NULL;

        Address(char* address_owner_email, char* address_zip_code, char* address_street, char* address_steet_number, char* address_city);

        ~Address();

        static Address* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif