
#include "main.h"

// cc -Wall -g -ggdb -o streams streams.c -lhiredis
// Usage: ./streams <add count> <read count> [block time, default: 1]

#define DEBUG 1000

#define READ_STREAM "stream1"
#define READ2_STREAM "stream3"

#define WRITE_STREAM "stream2"

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
    char key[100];
    char value[100];
    char streamname[100];
    char msgid[100];
    int numstreams;
    int k, i, h;
    char fval[100];
    
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

 reply = RedisCommand(c2r, "DEL %s", READ2_STREAM);
 assertReply(c2r, reply);
 dumpReply(reply, 0);

 reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM);
 assertReply(c2r, reply);
 dumpReply(reply, 0);

 
 /* Create streams/groups */
    initStreams(c2r, READ_STREAM);
    initStreams(c2r, WRITE_STREAM);
    initStreams(c2r, READ2_STREAM);


    
 while (1)
   {
     // send
     send_counter++;
     sprintf(key, "mykey:%d", send_counter);
     sprintf(value, "myvalue:%d", send_counter);
     
     reply = RedisCommand(c2r, "XADD %s * %s %s", WRITE_STREAM, key, value);
     assertReplyType(c2r, reply, REDIS_REPLY_STRING);
     printf("main(): pid =%d: stream %s: Added %s -> %s (id: %s)\n", pid, WRITE_STREAM, key, value, reply->str);
     freeReplyObject(reply);

     
     //  read
     read_counter++;

     // read from READ_STREAM


     printf("main(): pid %d: user %s: Trying to read msg %d from streams %s %s\n", pid, username, read_counter, READ_STREAM, READ2_STREAM);

     reply = RedisCommand(c2r, "XREADGROUP GROUP diameter %s BLOCK %d COUNT 10 NOACK STREAMS %s %s > >", 
			  username, block, READ_STREAM, READ2_STREAM);

  printf("main(): pid %d: user %s: calling assertReply\n", pid, username);

  assertReply(c2r, reply);
  
  printf("main(): pid %d: user %s: read num stream\n", pid, username);

  numstreams = ReadNumStreams(reply);

  printf("main(): pid %d: user %s: read %d streams\n", pid, username, numstreams);


#if 0  
  ReadStreamName(reply, streamname, 0);

  printf("main(): pid %d: user %s: streamname %s has %d msgs reading msg id\n",
	 pid, username, streamname,
         ReadStreamNumMsg(reply, 0));
 
 ReadStreamName(reply, streamname, 1);

 printf("main(): pid %d: user %s: streamname %s has %d msgs reading msg id\n",
	pid, username, streamname,
         ReadStreamNumMsg(reply, 1));

 printf("main(): pid %d: user %s: streamname %s, reading msg id\n", pid, username, streamname);
#endif
 
 for (k=0; k < ReadNumStreams(reply); k++)
   {
     ReadStreamName(reply, streamname, k);
   for (i=0; i < ReadStreamNumMsg(reply, k); i++)
     {
       ReadStreamNumMsgID(reply, k, i, msgid);
 
       printf("main(): pid %d: user %s: stream %s, streamnum %d, msg %d, msgid %s with %d values\n",
	      pid, username, streamname,
	      k, i, msgid,
	      ReadStreamMsgNumVal(reply, k, i));
	      for (h = 0; h < ReadStreamMsgNumVal(reply, k, i); h++)
		{
		  ReadStreamMsgVal(reply, k, i, h, fval);
		  printf("main(): pid %d: user %s: streamnum %d, msg %d, msgid %s value %d = %s\n", pid, username, k, i, msgid, h, fval);
		}	      	      
     }
   }

 
 //  printf("main(): pid %d: user %s: red streamname %s, msg id %s, calling dumpReply\n", pid, username, streamname, msgid);

 // dumpReply(reply, 0);

 printf("main(): pid %d: user %s: calling freeReplyObject\n", pid, username);

   freeReplyObject(reply);

     /* sleep   */
     micro_sleep(1000000);
   }  // while ()
 


    redisFree(c2r);
}
