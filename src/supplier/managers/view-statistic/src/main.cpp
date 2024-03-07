#include "main.h"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res;

    char query[QUERY_LEN], response[100], msg_id[30], first_key[30], client_id[30], second_key[30], supplier_id[100];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    // delete stream if exists
    reply = RedisCommand(c2r, "DEL %s", READ_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    reply = RedisCommand(c2r, "DEL %s", WRITE_STREAM);
    assertReply(c2r, reply);
    dumpReply(reply, 0);

    // initialize stream
    initStreams(c2r, READ_STREAM);
    initStreams(c2r, WRITE_STREAM);
    
    add_to_stream(); // DA TOGLIERE

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
            send_response_status(c2r, WRITE_STREAM, client_id, "INVALID#CLIENT#STREAM", msg_id, 0);
            continue;
        }

        // Check if the first key/value pair is the client_id
        ReadStreamMsgVal(reply, 0, 0, 0, second_key);    // Index of first field of msg = 0
        ReadStreamMsgVal(reply, 0, 0, 1, supplier_id);    // Index of second field of msg = 1

        if(strcmp(first_key, "supplier")){
            send_response_status(c2r, WRITE_STREAM, client_id, "INVALID#CLIENT#STREAM", msg_id, 0);
            continue;
        }

        // AND pr.id = o.purchase AND pr.purchase_instant  
        sprintf(query, "SELECT p.code, p.name, p.description, p.price FROM Product AS p, OrderedProducts AS o, Purchase AS pr WHERE o.product = p.code AND pr.id = o.purchase AND pr.purchase_instant >= CURRENT_DATE - INTERVAL '30 days' AND pr.purchase_instant<= CURRENT_DATE AND EXISTS(SELECT * FROM Restock WHERE supplier = \'%s\' AND product = p.code) GROUP BY p.code ORDER BY SUM(o.quantity) DESC LIMIT 1", supplier_id);

        query_res = db.RunQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id, 0);
            continue;
        }

        std::list<Product*> products;

        for(int row = 0; row < PQntuples(query_res); row++) {
            Product * product;
            product = new Product(PQgetvalue(query_res, row, PQfnumber(query_res, "code")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "name")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "description")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "price")));
            
            products.push_back(product);
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST#SUCCESS", msg_id, PQntuples(query_res));

        for(int row = 0; row<PQntuples(query_res); row++) {
            Product* product = products.front();

            products.pop_front();

            reply = RedisCommand(c2r, "XADD %s * row %d code %s name %s desciption %s price %s", WRITE_STREAM, row, product->code, product->name, product->description, product->price);
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);

        }

        read_from_stream();
    }

    db.finish();

    return 0;
}