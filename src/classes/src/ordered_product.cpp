#include "ordered_product.h"

OrderedProduct::OrderedProduct(char* order_product, 
        char* order_purchase, 
        char* order_quantity){

    product = (char*) malloc(sizeof(char) * 100);
    purchase = (char*) malloc(sizeof(char) * 100);
    quantity = (char*) malloc(sizeof(char) * 100);

    strcpy(product, order_product);
    strcpy(purchase, order_purchase);
    strcpy(quantity, order_quantity);
}

OrderedProduct::~OrderedProduct(){
    free(product);
    free(purchase);
    free(quantity);
}

OrderedProduct* OrderedProduct::from_stream(redisReply* reply, int stream_num, int msg_num, int starting_value, char* purchase) {
    char keyPARAMETERS_LEN;
    char valuePARAMETERS_LEN;

    char productPARAMETERS_LEN;
    char quantityPARAMETERS_LEN;

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

    return new OrderedProduct(product, purchase, quantity);
}
    
std::string OrderedProduct::to_insert_query() {
    std::string str_product = product;
    std::string str_purchase = purchase;
    std::string str_quantity = quantity;

    return "INSERT INTO OrderedProducts (product, purchase, quantity) VALUES (\'" + str_product + "\', \'" + str_purchase + "\', \'" + str_quantity + "\')";
}
