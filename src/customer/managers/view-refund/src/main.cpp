#include "main.h"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res;

    char query[QUERY_LEN], response[RESPONSE_LEN], msg_id[MESSAGE_ID_LEN], first_key[KEY_LEN], client_id[VALUE_LEN], second_key[KEY_LEN], product_name[VALUE_LEN], refund_id[VALUE_LEN];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    while(1) {
        reply = RedisCommand(c2r, "XREADGROUP GROUP main customer BLOCK 0 COUNT 1 STREAMS %s >", READ_STREAM);

        assertReply(c2r, reply);

        if (ReadNumStreams(reply) == 0) {
            continue;
        } 
        printf("ciao\n");
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

        // Take the input
        ReadStreamMsgVal(reply, 0, 0, 2, second_key);    // Index of first field of msg = 0
        ReadStreamMsgVal(reply, 0, 0, 3, refund_id);  // Index of second field of msg = 1

        std::string str_refund_id = refund_id;
        sprintf(query,  "SELECT rp.product AS product, rp.quantity AS quantity, rr.refund_state AS state, "
                        "       rr.request_instant AS request_instant, rr.refund_assignment_instant AS assigned, "
                        "       rr.refund_start AS refund_start, rr.refund_end AS refund_end, rr.courier AS courier, ad.purchase AS purchase "
                        "FROM RefundRequest AS rr, RefundedProduct AS rp, AssignedDelivery AS ad "
                        "WHERE rr.id = \'%s\' AND rr.id = rp.refund_request AND ad.delivery_code = rr.assigned_delivery", refund_id);
        printf("%s", query);
        query_res = db.RunQuery(query, true);
        
        if ((PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) || PQntuples(query_res) <= 0) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        Refund* refund;
        refund = new Refund(refund_id,
                            PQgetvalue(query_res, 0, PQfnumber(query_res, "request_instant")),
                            PQgetvalue(query_res, 0, PQfnumber(query_res, "assigned")),
                            PQgetvalue(query_res, 0, PQfnumber(query_res, "refund_start")),
                            PQgetvalue(query_res, 0, PQfnumber(query_res, "refund_end")),
                            PQgetvalue(query_res, 0, PQfnumber(query_res, "state")),
                            PQgetvalue(query_res, 0, PQfnumber(query_res, "courier")),
                            PQgetvalue(query_res, 0, PQfnumber(query_res, "purchase"))
                    );
        
        std::list<std::pair<std::string, std::string>> products;

        for(int row = 0; row < PQntuples(query_res); row++){
             products.push_back(std::make_pair(PQgetvalue(query_res, row, PQfnumber(query_res, "product")), 
                                                 PQgetvalue(query_res, row, PQfnumber(query_res, "quantity"))));
        }
        
        printf("ciao2, %d\n", PQntuples(query_res));
        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST#SUCCESS", msg_id, PQntuples(query_res) + 1);

        // send refund information
        reply = RedisCommand(c2r, "XADD %s * row 0 request_instant %s assigment_instant %s refund_start %s "
                                  "refund_end %s state %s courier %s purchase %s", WRITE_STREAM, 
                                    refund->request_instant,refund->assigment_instant, refund->start, 
                                    refund->end, refund->state, refund->courier, refund->purchase);
        assertReplyType(c2r, reply, REDIS_REPLY_STRING);
        freeReplyObject(reply);

        int row = 1;
        for (const auto& product : products) {
            printf("%s, %s\n", product.first.c_str(), product.second.c_str());
            reply = RedisCommand(c2r, "XADD %s * row %d product %s quantity %s", WRITE_STREAM, row, product.first.c_str(), product.second.c_str());
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);
            row++;
            
        }
        printf("ciao1\n");
        read_from_stream();
        
    }

    db.finish();

    return 0;
}
