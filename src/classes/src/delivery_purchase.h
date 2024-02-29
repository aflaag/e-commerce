#ifndef delivery_purchase_h
#define delivery_purchase_h

/* Library imports */

#include <string.h>
#include <stdexcept>

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"
#include "../../utils/src/const.h"
#include "../../utils/src/utils.h"

/* Classes */

class DeliveryPurchase {
    public:
        char *delivery_code;
        char *time;
        char *courier;
        char *purchase;

        DeliveryPurchase(char* purchase_id, char* del_code, char* courier_id);
        //DeliveryPurchase(char* code, char* timestamp);

        ~DeliveryPurchase();

        static DeliveryPurchase* from_stream(redisReply* reply, int stream_num, int msg_num);
        //static DeliveryPurchase* update_from_stream(redisReply* reply, int stream_num, int msg_num);
        std::string to_insert_query();
};

#endif
