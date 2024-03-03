#ifndef card_h
#define card_h

/* Library imports */

#include <string.h>
#include <stdexcept>

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"
#include "../../utils/src/const.h"

/* Classes */

class Rating{
    public:
        char *product = NULL;
        char *customer = NULL;
        char *stars = NULL;

        Rating(char* rating_product, char* rating_customer, char* rating_stars);

        ~Rating();

        static Rating* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif