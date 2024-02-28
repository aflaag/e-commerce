#ifndef purchase_h
#define purchase_h

/* Library imports */

#include <string.h>
#include <stdexcept>
// Da tenere per il tempo corrente, non provare a toglierle
#include <iostream>
#include <chrono>
#include <ctime>

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class Purchase{
    public:
        char *id;
        char *purchase_instant;
        char *cancel_instant;
        char *fare;
        char *customer;
        char *card;
        char *zip_code;
        char *street;
        char *street_number;

        Purchase(char* purchase_id, char* purchase_instant_in, char* purchase_fare, char* purchase_customer, 
                char* purchase_card, char* purchase_zip_code, char* purchase_street, char* purchase_street_number);

        ~Purchase();

        static Purchase* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif
