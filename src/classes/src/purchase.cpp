#include "purchase.h"

Purchase::Purchase(
        char* purchase_id,
        char* purchase_instant_in,
        char* purchase_fare,
        char* purchase_customer, 
        char* purchase_card,
        char* purchase_zip_code, 
        char* purchase_street, 
        char* purchase_street_number){

    id = (char*) malloc(sizeof(char) * 100);
    purchase_instant = (char*) malloc(sizeof(char) * 100);
    fare = (char*) malloc(sizeof(char) * 100);
    customer = (char*) malloc(sizeof(char) * 100);
    card = (char*) malloc(sizeof(char) * 100);
    zip_code = (char*) malloc(sizeof(char) * 100);
    street = (char*) malloc(sizeof(char) * 100);
    street_number = (char*) malloc(sizeof(char) * 100);

    strcpy(id, purchase_id);
    strcpy(purchase_instant, purchase_instant_in);
    strcpy(fare, purchase_fare);
    strcpy(customer, purchase_customer);
    strcpy(card, purchase_card);
    strcpy(zip_code, purchase_zip_code);
    strcpy(street, purchase_street);
    strcpy(street_number, purchase_street_number);
}

Purchase::~Purchase(){
    free(id);
    free(purchase_instant);
    free(fare);
    free(customer);
    free(card);
    free(zip_code);
    free(street);
    free(street_number);
}

Purchase* Purchase::from_stream(redisReply* reply, int stream_num, int msg_num){

    char key[100];
    char value[100];

    char id[100];
    char purchase_instant[100];
    char fare[100];
    char customer[100];
    char card[100];
    char zip_code[100];
    char street[100];
    char street_number[100];

    char read_fields = 0b0000000;

    // prendo il timestamp corrente
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    purchase_instant = std::ctime(&current_time_t) 

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "id")) {
            sprintf(id, "%s", value);
            read_fields |=0b0000001;

        } else if (!strcmp(key, "fare")) {
            sprintf(fare, "%s", value);
            read_fields |=0b0000010;

        }else if (!strcmp(key, "customer")) {
            sprintf(customer, "%s", value);
            read_fields |=0b0000100;

        }else if (!strcmp(key, "card")) {
            sprintf(card, "%s", value);
            read_fields |=0b0001000;

        }else if (!strcmp(key, "zip_code")) {
            sprintf(zip_code, "%s", value);
            read_fields |=0b0010000;

        }else if (!strcmp(key, "street")) {
            sprintf(street, "%s", value);
            read_fields |=0b0100000;

        }else if (!strcmp(key, "street_number")) {
            sprintf(street_number, "%s", value);
            read_fields |=0b1000000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111111){
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Purchase(id, purchase_instant, fare, customer, card, zip_code, street, street_number);
}
    