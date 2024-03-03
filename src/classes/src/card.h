#ifndef card_h
#define card_h

/* Library imports */

#include <string.h>
#include <stdexcept>

/* Local imports */

#include "../../utils/src/const.h"
#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class Card{
    public:
        char *number = NULL;
        char *email = NULL;

        Card(char* card_number, char* card_owner_email);

        ~Card();

        static Card* from_stream(redisReply* reply, int stream_num, int msg_num);
};

#endif
