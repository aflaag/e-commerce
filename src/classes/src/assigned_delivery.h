#ifndef assigned_delivery_h
#define assigned_delivery_h

/* Library imports */

#include <string.h>
#include <stdexcept>
#include "../../utils/src/const.h"

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

/* Classes */

class AssignedDelivery {
    public:
        char* delivery_code;
        char* delivery_assignment_instant;
        char* delivery_start;
        char* delivery_end;
        char* delivery_state;
        char* lost_instant;
        char* courier;
        char* purchase;

        AssignedDelivery(char* ad_delivery_code, char* ad_delivery_assignment_instant, char* ad_delivery_start, char* ad_delivery_end, char* ad_delivery_state, char* ad_lost_instant, char* ad_courier, char* ad_purchase);

        ~AssignedDelivery();
};

#endif
