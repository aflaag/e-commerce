#ifndef db_h
#define db_h

/* Library imports */

#include <string.h>
#include <stdexcept>

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class Customer{
    public:
        char* email;
        char* name;
        char* surname;
        char* phone_number;

        Customer(char* customer_email, char* customer_name, char* customer_surname, char* customer_phone_number);

        ~Customer();

        static Customer* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif