#include "product.h"

Product::Product(char* product_code, 
        char* product_name, 
        char* product_description,
        char* product_price ){

    code = (char*) malloc(sizeof(char) * 100);
    name = (char*) malloc(sizeof(char) * 100);
    description = (char*) malloc(sizeof(char) * 100);
    price = (char*) malloc(sizeof(char) * 100);

    strcpy(code, product_code);
    strcpy(name, product_name);
    strcpy(description, product_description);
    strcpy(price, product_price);
}

Product::~Product(){
    free(code);
    free(name);
    free(description);
    free(price);
}

Product* Product::from_stream(redisReply* reply, int stream_num, int msg_num, int starting_value) {
    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char code[PARAMETERS_LEN];
    char price[PARAMETERS_LEN];
    char description[PARAMETERS_LEN];
    char name[PARAMETERS_LEN];

    char read_fields = 0b0000;

    for (int field_num = starting_value; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "code")) {
            sprintf(code, "%s", value);
            read_fields |=0b0001;

        } else if (!strcmp(key, "price")) {
            sprintf(price, "%s", value);
            read_fields |=0b0010;

        } else if (!strcmp(key, "description")) {
            sprintf(description, "%s", value);
            read_fields |=0b0100;

        } else if (!strcmp(key, "name")) {
            sprintf(name, "%s", value);
            read_fields |=0b1000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111) {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Product(code, name, description, price);
}
    
std::string Product::to_insert_query() {
    std::string str_code = code;
    std::string str_name = name;
    std::string str_description = description;
    std::string str_price = price;

    replaceSubstring(str_name, SPACE_REDIS_STRING, SPACE);
    replaceSubstring(str_description, SPACE_REDIS_STRING, SPACE);

    return "INSERT INTO Products (code, name, description, price) VALUES (\'" + str_code + "\', \'" + str_name + "\', \'" + str_description + "\', \'" + str_description + "\')";
}
