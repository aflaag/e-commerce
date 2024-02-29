#ifndef handler_h
#define handler_h

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
#include <iostream>

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

class Handler{
    public:
        Handler(const char* redis_server, int redis_port, std::string req_types[], int num_req_types);

        void send_to_managers(int client_id, std::string cmd);
        std::string read_from_managers();

    private:
        redisContext *c2r;
        std::string* types;
        int num_types;
        
        void init_streams();
};

#endif