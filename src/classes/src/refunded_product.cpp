#include "refunded_product.h"

RefundedProduct::RefundedProduct(char* refund_request_in, 
        char* refunded_product, 
        char* refund_quantity){

    refund_request = (char*) malloc(sizeof(char) * 100);
    product = (char*) malloc(sizeof(char) * 100);
    quantity = (char*) malloc(sizeof(char) * 100);

    strcpy(refund_request, refund_request_in);
    strcpy(product, refunded_product);
    strcpy(quantity, refund_quantity);
}

RefundedProduct::~RefundedProduct(){
    free(refund_request);
    free(product);
    free(quantity);
}

RefundedProduct* RefundedProduct::from_stream(redisReply* reply, int stream_num, int msg_num, int starting_value, char* refund) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char product[PARAMETERS_LEN];
    char quantity[PARAMETERS_LEN];

    char read_fields = 0b00;

    for (int field_num = starting_value; field_num < starting_value + 4; field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "product")) {
            sprintf(product, "%s", value);
            read_fields |=0b01;

        } else if (!strcmp(key, "quantity")) {
            sprintf(quantity, "%s", value);
            read_fields |=0b10;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b11) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new RefundedProduct(refund, product, quantity);
}
    
std::string RefundedProduct::to_insert_query() {
    std::string str_product = product;
    std::string str_refund = refund_request;
    std::string str_quantity = quantity;

    return "INSERT INTO RefundedProduct (refund_request, product, quantity) VALUES (\'" + str_refund + "\', \'" + str_product + "\', \'" + str_quantity + "\')";
}
