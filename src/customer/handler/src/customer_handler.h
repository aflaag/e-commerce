#ifndef customer_handler_h
#define customer_handler_h

/* Library imports */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/times.h>
#include <cassert>
#include <cerrno>

/* Local imports */

#include "../../../../lib/con2db/pgsql.h"
#include "../../../../lib/con2redis/src/con2redis.h"
#include "../../../../classes/src/customer.h"

/* Constants */

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define READ_CLIENT_STREAM "client-out"
#define WRITE_CLIENT_STREAM "client-in"

#define READ_ADD_CUSTOMER_STREAM "add-customer-out"
#define WRITE_ADD_CUSTOMER_STREAM "add-customer-in"

#endif