#include "rating.h"

Rating::Rating(char *rating_product,
        char *rating_customer,
        char *rating_stars){

    product = (char*) malloc(sizeof(char) * 100);
    customer = (char*) malloc(sizeof(char) * 100);
    stars = (char*) malloc(sizeof(char) * 100);

    strcpy(product, rating_product);
    strcpy(customer, rating_customer);
    strcpy(stars, rating_stars);
}

Rating::~Rating(){
    free(product);
    free(customer);
    free(stars);
}

Rating* Rating::from_stream(redisReply* reply, int stream_num, int msg_num){

    char key[100];
    char value[100];

    char product[100];
    char customer[100];
    char stars[100];

    char read_fields = 0b000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "product")) {
            sprintf(product, "%s", value);
            read_fields |=0b001;

        } else if (!strcmp(key, "customer")) {
            sprintf(customer, "%s", value);
            read_fields |=0b010;

        }else if (!strcmp(key, "stars")) {
            sprintf(stars, "%s", value);
            read_fields |=0b100;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b111){
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Rating(product, customer, stars);
}
    