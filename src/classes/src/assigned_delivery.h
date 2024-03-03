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
        char* delivery_code = NULL;
        char* delivery_assignment_instant = NULL;
        char* delivery_start = NULL;
        char* delivery_end = NULL;
        char* delivery_state = NULL;
        char* lost_instant = NULL;
        char* courier = NULL;
        char* purchase = NULL;

        AssignedDelivery(char* ad_delivery_code, char* ad_delivery_assignment_instant, char* ad_delivery_start, char* ad_delivery_end, char* ad_delivery_state, char* ad_lost_instant, char* ad_courier, char* ad_purchase);

        ~AssignedDelivery();
};

#endif
