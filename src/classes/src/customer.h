#ifndef customer_h
#define customer_h

/* System libraries */

#include <string.h>
#include <stdexcept>

/* Local libraries */

#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class Customer{
    public:
        char* email = NULL;
        char* name = NULL;
        char* surname = NULL;
        char* phone_number = NULL;

        Customer(char* customer_email, char* customer_name, char* customer_surname, char* customer_phone_number);

        ~Customer();

        static Customer* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif