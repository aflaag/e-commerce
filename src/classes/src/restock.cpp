#include "restock.h"

Restock::Restock(char* restock_id, char* restock_quantity, char* restock_supplier, char* restock_product){
    id = (char*) malloc(sizeof(char) * 100);
    quantity = (char*) malloc(sizeof(char) * 100);
    supplier = (char*) malloc(sizeof(char) * 100);
    product = (char*) malloc(sizeof(char) * 100);

    strcpy(id, restock_id);
    strcpy(quantity, restock_quantity);
    strcpy(supplier, restock_supplier);
    strcpy(product, restock_product);
}

Restock::~Restock(){
    free(id);
    free(quantity);
    free(supplier);
    free(product);
}

Restock* Restock::from_stream(redisReply* reply, int stream_num, int msg_num){
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char id[PARAMETERS_LEN];
    char quantity[PARAMETERS_LEN];
    char supplier[PARAMETERS_LEN];
    char product[PARAMETERS_LEN];

    char read_fields = 0b0000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "id")) {
            sprintf(id, "%s", value);
            read_fields |=0b0001;

        } else if (!strcmp(key, "quantity")) {
            sprintf(quantity, "%s", value);
            read_fields |=0b0010;

        } else if (!strcmp(key, "supplier")) {
            sprintf(supplier, "%s", value);
            read_fields |=0b0100;

        } else if (!strcmp(key, "product")) {
            sprintf(product, "%s", value);
            read_fields |=0b1000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Restock(id, quantity, supplier, product);
}
    
