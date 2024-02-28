#include "main.h"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res;

    char query[1000], response[100], msg_id[30], first_key[30], client_id[30];

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

    Purchase* purchase;

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
            send_response_status(c2r, WRITE_STREAM, client_id, "INVALID#CLIENT#STREAM", msg_id);
            continue;
        }

        // Convert request
        try{
            purchase = Purchase::from_stream(reply, 0, 0);
        }
        catch(std::invalid_argument exp){
            send_response_status(c2r, WRITE_STREAM, client_id, "INVALID#REQUEST", msg_id);
            continue;
        }

        sprintf(query, "INSERT INTO Purchase (purchase_instant, fare, customer, card, zip_code, street, street_number) VALUES (\'%s\', %s, \'%s\', \'%s\', \'%s\', \'%s\', \'%s\') RETURNING id;", purchase->purchase_instant, purchase->fare, purchase->customer, purchase->card, purchase->zip_code, purchase->street, purchase->street_number);

        query_res = db.RunQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id);
            continue;
        }

        // check if the id was created
        if (PQntuples(query_res) != 1) {
            printf("%d\n", PQntuples(query_res));
            // printf("%s\n", PQgetvalue(query_res, 0, 0));
            send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id);
            continue;
        }

        // get the purchase id
        char* purchase = PQgetvalue(query_res, 0, 0);

        std::string query_products = "";

        bool invalid_product = false;

        // 14 is where the first product is expected to be
        for (int field_num = 14; field_num < ReadStreamMsgNumVal(reply, 0, 0); field_num += 4) {
            char query_find_product[1000];

            OrderedProduct* ordered_product = OrderedProduct::from_stream(reply, 0, 0, field_num, purchase);

            sprintf(query_find_product, "SELECT code FROM Product WHERE code = \'%s\'", ordered_product->product);

            query_res = db.RunQuery(query_find_product, true);

            if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
                send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id);

                invalid_product = true;

                break;
            }

            query_products += ordered_product->to_insert_query();
            query_products += ";";
        }

        if (invalid_product) {
            continue;
        }

        query_res = db.RunQuery((char*) query_products.c_str(), false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id);

            char delete_op[1000];

            sprintf(delete_op, "DELETE FROM Purchase WHERE id = %s", purchase);

            query_res = db.RunQuery(delete_op, false);

            if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
                send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id);

                continue;
            }

            continue;
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "INSERT#SUCCESS", msg_id);
    }

    db.finish();

    return 0;
}
