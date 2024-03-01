#ifndef utils_h
#define utils_h

/* Library imports */

#include <string>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>

/* Local imports */

#include "../../lib/con2redis/src/con2redis.h"

void send_response_status(redisContext* c2r, const char *stream, const char *client_id, const char *resp_status, const char *msg_id, const int nrows);

std::string replace_substring(std::string input, const std::string& target, const std::string& replacement);

std::string get_current_timestamp_as_string();

#endif 
