#ifndef main_h
#define main_h

/* Library imports */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stddef.h>
#include <limits.h>
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
#include "test_stream.h"

/* Constants */

#define READ_STREAM "customer-handler-to-manager"
#define WRITE_STREAM "customer-manager-to-handler"

#define MANAGER_GROUP "add-customer"

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "customer"
#define POSTGRESQL_PSW "customer"
#define POSTGRESQL_DBNAME "ecommerce"

/* Functions */

void send_response_status(redisContext* c2r, const char *stream, const char *client_id, const char *resp_status, const char *msg_id);

#endif
