#ifndef test_stream_h
#define test_stream_h


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

#include "../../../../lib/con2redis/src/con2redis.h"

using namespace std;

int add_to_stream();
#endif