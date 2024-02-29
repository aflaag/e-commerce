#include "delivery_purchase.h"

DeliveryPurchase::DeliveryPurchase(char* purchase_id, char* del_code, char* courier_id) {

    delivery_code = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    courier = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    purchase = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(delivery_code, del_code);
    strcpy(courier, courier_id);
    strcpy(purchase, purchase_id);
}
/*
DeliveryPurchase::DeliveryPurchase(char* product_code, char* product_description){

    code = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    name = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    description = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    price = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(code, product_code);
    strcpy(description, product_description);
}
*/

DeliveryPurchase::~DeliveryPurchase(){
    free(delivery_code);
    free(purchase);
    free(courier);
    free(time);
}

DeliveryPurchase* DeliveryPurchase::from_stream(redisReply* reply, int stream_num, int msg_num) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char code_dl[PARAMETERS_LEN];
    char courier_id[PARAMETERS_LEN];
    char purchase_id[PARAMETERS_LEN];

    char read_fields = 0b000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "courier")) {
            sprintf(courier_id, "%s", value);
            read_fields |=0b001;

        } else if (!strcmp(key, "purchase")) {
            sprintf(purchase_id, "%s", value);
            read_fields |=0b010;

        } else if (!strcmp(key, "deliverycode")) {
            sprintf(code_dl, "%s", value);
            read_fields |=0b100;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new DeliveryPurchase(purchase_id, code_dl, courier_id);
}

/*
DeliveryPurchase* DeliveryPurchase::update_from_stream(redisReply* reply, int stream_num, int msg_num) {
    
}
*/

std::string DeliveryPurchase::to_insert_query() {
    std::string str_code = delivery_code;
    std::string str_purchase = purchase;
    std::string str_courier = courier;
    std::string str_time = get_current_timestamp_as_string();

    return "INSERT INTO AssignedDelivery (delivery_code, delivery_assignment_instant, delivery_state, courier, purchase) VALUES (\'" + str_code + "\', \'" + str_time + "\', \'Assigned\', \'" + str_courier + "\', \'" + str_purchase + "\')";
}