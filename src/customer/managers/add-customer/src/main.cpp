#include "main.h"

#define READ_STREAM "read_stream"

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

    c2r = redisConnect("localhost", 6379);

    // delete stream if exists
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    // initialize stream
    initStreams(c2r, READ_STREAM);

    reply = RedisCommand(c2r, "XADD %s * cazzo palle", READ_STREAM);
     //     reply = RedisCommand(c2r, "XADD %s * %s", WRITE_STREAM, msg);
    assertReplyType(c2r, reply, REDIS_REPLY_STRING);

    freeReplyObject(reply);

    printf("test\n");

    reply = RedisCommand(c2r, "XREADGROUP GROUP test_group Alice BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

    assertReply(c2r, reply);
    // dumpReply(reply, 0);
    freeReplyObject(reply);

    printf("test\n");

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

    printf("test\n");

    // Con2DB db("localhost", "5432", "customer", "customer", "ecommerce");

    // PGresult *res;
    
    // char query[1000];

    // sprintf(query, "INSERT INTO Customer (email, name, surname , phone_number) VALUES (\'c@gmail.com\', \'a\', \'boh\', \'+12345678\')");

    // res = db.RunQuery(query, false);

    // if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK) {
    //     db.finish();

    //     return -1;
    // }

    // db.finish();

    return 0;
}