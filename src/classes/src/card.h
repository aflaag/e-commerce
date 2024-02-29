#ifndef card_h
#define card_h

/* Library imports */

#include <string.h>
#include <stdexcept>
#include "../../utils/src/consts.h"

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class Card{
    public:
        char *number;
        char *email;

        Card(char* card_number, char* card_owner_email);

        ~Card();

        static Card* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif