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
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }

        // Take the input
        ReadStreamMsgVal(reply, 0, 0, 2, second_key);    // Index of first field of msg = 0
        ReadStreamMsgVal(reply, 0, 0, 3, purchase_id);  // Index of second field of msg = 1

        if(strcmp(second_key, "purchase_id") || (ReadStreamMsgNumVal(reply, 0, 0) > 4)){
            send_response_status(c2r, WRITE_STREAM, client_id, "BAD_REQUEST", msg_id, 0);
            continue;
        }
        // std::string str_purchase_id = purchase_id;
        // std::string search_parameter = "%"+ str_purchase_id + "%";
        // sprintf(query, "SELECT * FROM Purchase WHERE name LIKE \'%s\' ", (char*)search_parameter.c_str());

        sprintf(query, "SELECT pu.id, pu.purchase_instant, pu.cancel_instant, pu.fare, pu.customer, pu.card, pu.zip_code, pu.street, pu.street_number, op.product, op.quantity, ad.delivery_code, ad.delivery_assignment_instant, ad.delivery_start, ad.delivery_end, ad.delivery_state, ad.lost_instant, ad.courier FROM OrderedProducts as op, Purchase as pu LEFT JOIN AssignedDelivery as ad ON pu.id = ad.purchase WHERE pu.id = %s AND op.purchase = pu.id", purchase_id);

        query_res = db.RunQuery(query, true);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send_response_status(c2r, WRITE_STREAM, client_id, "DB_ERROR", msg_id, 0);
            continue;
        }

        std::list<Tuple*> tuples;

        for(int row = 0; row < PQntuples(query_res); row++) {
            Purchase * purchase;
            purchase = new Purchase(PQgetvalue(query_res, row, PQfnumber(query_res, "id")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "purchase_instant")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "fare")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "cancel_instant")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "customer")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "card")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "zip_code")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "street")),
                                    PQgetvalue(query_res, row, PQfnumber(query_res, "street_number")));

            OrderedProduct * ordered_product;
            ordered_product = new OrderedProduct(PQgetvalue(query_res, row, PQfnumber(query_res, "product")),
                                                 PQgetvalue(query_res, row, PQfnumber(query_res, "id")),
                                                 PQgetvalue(query_res, row, PQfnumber(query_res, "quantity")));

            AssignedDelivery * assigned_delivery;
            assigned_delivery = new AssignedDelivery(PQgetvalue(query_res, row, PQfnumber(query_res, "delivery_code")),
                                                     PQgetvalue(query_res, row, PQfnumber(query_res, "delivery_assignment_instant")),
                                                     PQgetvalue(query_res, row, PQfnumber(query_res, "delivery_start")),
                                                     PQgetvalue(query_res, row, PQfnumber(query_res, "delivery_end")),
                                                     PQgetvalue(query_res, row, PQfnumber(query_res, "delivery_state")),
                                                     PQgetvalue(query_res, row, PQfnumber(query_res, "lost_instant")),
                                                     PQgetvalue(query_res, row, PQfnumber(query_res, "courier")),
                                                     PQgetvalue(query_res, row, PQfnumber(query_res, "id")));

            Tuple* tuple = (Tuple*) calloc(sizeof(Purchase*) + sizeof(OrderedProduct*) + sizeof(AssignedDelivery*), 1);

            tuple->purchase = purchase;
            tuple->ordered_product = ordered_product;
            tuple->assigned_delivery = assigned_delivery;
            

            tuples.push_back(tuple);
        }

        send_response_status(c2r, WRITE_STREAM, client_id, "REQUEST_SUCCESS", msg_id, PQntuples(query_res));

        for(int row = 0; row<PQntuples(query_res); row++) {
            Tuple* tuple = tuples.front();

            tuples.pop_front();
            
            Purchase* purchase = tuple->purchase;
            OrderedProduct* ordered_product = tuple->ordered_product;
            AssignedDelivery* assigned_delivery = tuple->assigned_delivery;

            reply = RedisCommand(c2r, "XADD %s * row %d purchase_id %s purchase_instant %s purchase_cancel_instant %s purchase_fare %s purchase_customer %s purchase_card %s purchase_zip_code %s purchase_street %s purchase_street_number %s ordered_product_product %s ordered_product_quantity %s assigend_delivery_code %s assigned_delivery_assignment_instant %s assigned_delivery_start %s assigned_delivery_end %s assigned_delivery_state %s assigned_delivery_lost_instant %s assigned_delivery_courier %s", WRITE_STREAM, row, purchase->id, purchase->purchase_instant, purchase->cancel_instant, purchase->fare, purchase->customer, purchase->card, purchase->zip_code, purchase->street, purchase->street_number, ordered_product->product, ordered_product->purchase, ordered_product->quantity, assigned_delivery->delivery_code, assigned_delivery->delivery_assignment_instant, assigned_delivery->delivery_start, assigned_delivery->delivery_end, assigned_delivery->delivery_state, assigned_delivery->lost_instant, assigned_delivery->courier);
            assertReplyType(c2r, reply, REDIS_REPLY_STRING);
            freeReplyObject(reply);

            free(tuple);
        }
    }

    db.finish();

    return 0;
}
