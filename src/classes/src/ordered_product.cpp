#include "ordered_products.h"

Ordered_products::Ordered_products(char* order_product, 
        char* order_purchase, 
        char* order_quantity){

    product = (char*) malloc(sizeof(char) * 100);
    purchase = (char*) malloc(sizeof(char) * 100);
    quantity = (char*) malloc(sizeof(char) * 100);

    strcpy(product, order_product);
    strcpy(purchase, order_purchase);
    strcpy(quantity, order_quantity);
}

Ordered_products::~Ordered_products(){
    free(product);
    free(purchase);
    free(quantity);
}

Ordered_products* Ordered_products::from_stream(redisReply* reply, int stream_num, int msg_num){

    char key[100];
    char value[100];

    char product[100];
    char purchase[100];
    char quantity[100];

    char read_fields = 0b000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "product")) {
            sprintf(product, "%s", value);
            read_fields |=0b001;

        } else if (!strcmp(key, "purchase")) {
            sprintf(purchase, "%s", value);
            read_fields |=0b010;

        } else if (!strcmp(key, "quantity")) {
            sprintf(quantity, "%s", value);
            read_fields |=0b100;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b111){
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Ordered_products(product, purchase, quantity);
}
    