#ifndef utils_h
#define utils_h

/* Libraries */

#include "../../lib/con2redis/src/con2redis.h"

void send_response_status(redisContext* c2r, const char *stream, const char *client_id, const char *resp_status, const char *msg_id);

void replaceSubstring(std::string& input, const std::string& target, const std::string& replacement);

#endif 