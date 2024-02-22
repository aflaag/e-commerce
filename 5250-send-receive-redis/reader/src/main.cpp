
#include "main.h"

// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000

#define READ_STREAM "stream2"
#define WRITE_STREAM "stream1"
#define WRITE2_STREAM "stream3"

#define NMSG 3

using namespace std;


int main() {
  
    redisContext *c2r;
    redisReply *reply;
   int read_counter = 0;
    int send_counter = 0;
    int block = 1000000000;
    int pid;
    unsigned seed;
    char username[100];
    //    char key[100];
    //    char value[100];
    char msg[1000];
    int i;
    
    /*  prg  */

#if (DEBUG > 0)
  setvbuf(stdout, (char*) NULL, _IONBF, 0);
  setvbuf(stderr, (char*) NULL, _IONBF, 0);
#endif

  seed = (unsigned) time(NULL);
  srand(seed);

  sprintf(username, "%u", rand());
 
  pid = getpid();
    
  printf("main(): pid %d: user %s: connecting to redis ...\n", pid, username);

 c2r = redisConnect("localhost", 6379);
    
 printf("main(): pid %d: user %s: connected to redis\n", pid, username);

  // delete stream if it exists
 reply = RedisCommand(c2r, "DEL %s", READ_STREAM);
 assertReply(c2r, reply);
 dumpReply(reply, 0);

 reply = RedisCommand(c2r, "DEL %s", WRITE2_STREAM);
 assertReply(c2r, reply);
 dumpReply(reply, 0);

 reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM);
 assertReply(c2r, reply);
 dumpReply(reply, 0);


 /* Create streams/groups */
    initStreams(c2r, READ_STREAM);
    initStreams(c2r, WRITE_STREAM);
    initStreams(c2r, WRITE2_STREAM);


    
 while (1)
   {
     // send 3 messages with 3 items
     for (i=0; i < NMSG; i++)
       {
	 send_counter++;
	 sprintf(msg, "Amykey:%d Amyvalue:%d Bmykey:%d Bmyvalue:%d Cmykey:%d Cmyvalue:%d",
		 send_counter, send_counter, send_counter, send_counter, send_counter, send_counter);

     printf("main(): pid =%d: sending redis cmd: XADD %s * Amykey:%d Amyvalue:%d Bmykey:%d Bmyvalue:%d Cmykey:%d Cmyvalue:%d\n",
	    pid, WRITE_STREAM, send_counter, send_counter, send_counter, send_counter, send_counter, send_counter);
     
     reply = RedisCommand(c2r, "XADD %s * Amykey:%d Amyvalue:%d Bmykey:%d Bmyvalue:%d Cmykey:%d Cmyvalue:%d",
			  WRITE_STREAM, send_counter, send_counter, send_counter, send_counter, send_counter, send_counter);
     //     reply = RedisCommand(c2r, "XADD %s * %s", WRITE_STREAM, msg);
     assertReplyType(c2r, reply, REDIS_REPLY_STRING);
     printf("main(): pid =%d: stream %s: Added msg %s - dumpRerply:\n", pid, WRITE_STREAM, msg);

     //dumpReply(reply, 0);
     freeReplyObject(reply);

     reply = RedisCommand(c2r, "XADD %s * Dmykey:%d Dmyvalue:%d Emykey:%d Emyvalue:%d Fmykey:%d Fmyvalue:%d", WRITE2_STREAM,
			  send_counter, send_counter, send_counter, send_counter, send_counter, send_counter);
     //     reply = RedisCommand(c2r, "XADD %s * %s", WRITE_STREAM, msg);
     assertReplyType(c2r, reply, REDIS_REPLY_STRING);
	 sprintf(msg, "Dmykey:%d Dmyvalue:%d Emykey:%d Emyvalue:%d Fmykey:%d Fmyvalue:%d",
		 send_counter, send_counter, send_counter, send_counter, send_counter, send_counter);
	 
     printf("main(): pid = %d: stream %s: Added msg %s - dumpRerply:\n", pid, WRITE2_STREAM, msg);
     
     //  dumpReply(reply, 0);
     freeReplyObject(reply);

     
       }  // for i
     
     //  read
     read_counter++;
     printf("main(): pid %d: user %s: Trying to read msg %d from stream %s\n", pid, username, read_counter, READ_STREAM);

     reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT 1 NOACK STREAMS %s >", 
			  username, block, READ_STREAM);

     assertReply(c2r, reply);
     // dumpReply(reply, 0);
     freeReplyObject(reply);

      /* sleep   */
     micro_sleep(1000000);

     break;
     
   }  // while ()
 


    redisFree(c2r);
}
