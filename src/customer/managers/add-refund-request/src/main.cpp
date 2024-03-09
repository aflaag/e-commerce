#include "main.h"

int main() {
    redisContext *c2r;
    redisReply *reply;

    PGresult *query_res;

    char query[QUERY_LEN], response[100], msg_id[30], first_key[30], client_id[30];

    Con2DB db(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
    c2r = redisConnect(REDIS_SERVER, REDIS_PORT);

    Refund* refund;

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

        // Convert request
        try{
            refund = Refund::from_stream(reply, 0, 0);
        }
        catch(std::invalid_argument exp){
            send_response_status(c2r, WRITE_STREAM, client_id, "INVALID#REQUEST", msg_id, 0);
            continue;
        }

        printf("refund id %s\n\n\n",client_id);

        sprintf(query, "INSERT INTO RefundRequest (request_instant, refund_state, assigned_delivery) VALUES (\'%s\', \'%s\', \'%s\') RETURNING id;", refund->request_instant, refund->state, refund->assigned_delivery);


        query_res = db.RunQuery(query, true);


        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id, 0);
            continue;
        }
        
        // check if the id was created
        if (PQntuples(query_res) != 1) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id, 0);
            continue;
        }

        // get the purchase id
        char* refund_id = PQgetvalue(query_res, 0, 0);

        std::string query_products = "";

        bool invalid_product = false;

        // 4 is where the first product is expected to be
        for (int field_num = 4; field_num < ReadStreamMsgNumVal(reply, 0, 0); field_num += 4) {
            char query_find_product[QUERY_LEN];

            RefundedProduct* refunded_product = RefundedProduct::from_stream(reply, 0, 0, field_num, refund_id);

            sprintf(query_find_product, "SELECT code FROM Product WHERE code = \'%s\'", refunded_product->product);

            query_res = db.RunQuery(query_find_product, true);

            if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
                send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id, 0);

                invalid_product = true;

                break;
            }

            query_products += refunded_product->to_insert_query();
            query_products += ";";
        }

        if (invalid_product) {
            continue;
        }

        query_res = db.RunQuery((char*) query_products.c_str(), false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id, 0);

            char delete_op[QUERY_LEN];

            sprintf(delete_op, "DELETE FROM RefundRequest WHERE id = %s", refund_id);

            query_res = db.RunQuery(delete_op, false);

            if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
                send_response_status(c2r, WRITE_STREAM, client_id, "DB#ERROR", msg_id, 0);

                continue;
            }

            continue;
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST#SUCCESS", msg_id, 0);
    }

    db.finish();

    return 0;
}
