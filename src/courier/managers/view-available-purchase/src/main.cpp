#include "main.h"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN], second_key[KEY_LEN], purchase_id[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    while(1) {
        reply = RedisCommand(c2r, "XREADGROUP GROUP main customer BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(c2r, reply);

        if (ReadNumStreams(reply) == 0) {
            continue;
        } 

        // Only one stream --> stream_num = 0
        // Only one message in stream --> msg_num = 0
        ReadStreamNumMsgID(reply, 0, 0, msg_id);

        // Check if the first key/value pair is the client_id
        ReadStreamMsgVal(reply, 0, 0, 0, first_key);    // Index of first field of msg = 0
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);    // Index of second field of msg = 1

        if(strcmp(first_key, "client_id")){
            send_response_status(c2r, WRITE_STREAM, client_id, "INVALID_CLIENT#STREAM", msg_id, 0);
            continue;
        }

        sprintf(query, "SELECT id, purchase_instant, fare, customer, card, zip_code, street, street_number FROM Purchase WHERE cancel_instant IS NULL AND NOT EXISTS(SELECT * FROM AssignedDelivery WHERE purchase = id)");

        query_res = db.RunQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::list<Purchase*> purchases;

        for(int row = 0; row < PQntuples(query_res); row++) {
            Purchase * purchase;
            purchase = new Purchase(PQgetvalue(query_res, row, PQfnumber(query_res, "id")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "purchase_instant")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "fare")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "customer")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "card")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "zip_code")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "street")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "street_number")));
            
            purchases.push_back(purchase);
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST#SUCCESS", msg_id, PQntuples(query_res));

        for(int row = 0; row<PQntuples(query_res); row++) {
            Purchase* purchase = purchases.front();

            purchases.pop_front();

            reply = RedisCommand(c2r, "XADD %s * row %d purchase_id %s purchase_instant %s purchase_fare %s purchase_customer %s purchase_card %s purchase_zip_code %s purchase_street %s purchase_street_number %s", WRITE_STREAM, row, purchase->id, purchase->purchase_instant, purchase->fare, purchase->customer, purchase->card, purchase->zip_code, purchase->street, purchase->street_number);
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);

        }

        read_from_stream();
    }

    db.finish();

    return 0;
}
