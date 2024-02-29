#include "address.h"

Address::Address(char* address_owner_email, 
            char* address_zip_code, 
            char* address_street, 
            char* address_steet_number, 
            char* address_city){

    email = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    zip_code = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    street = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    street_number = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    city = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(email, address_owner_email);
    strcpy(zip_code, address_zip_code);
    strcpy(street, address_street);
    strcpy(street_number, address_steet_number);
    strcpy(city, address_city);
}

Address::~Address(){
    free(email);
    free(zip_code);
    free(street);
    free(street_number);
    free(city);
}

Address* Address::from_stream(redisReply* reply, int stream_num, int msg_num){

    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char email[PARAMETERS_LEN];
    char zip_code[PARAMETERS_LEN];
    char street[PARAMETERS_LEN];
    char street_number[PARAMETERS_LEN];
    char city[PARAMETERS_LEN];

    char read_fields = 0b00000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);

        if (!strcmp(key, "email")) {
            sprintf(email, "%s", value);
            read_fields |=0b00001;
            
        } else if (!strcmp(key, "zip_code")) {
            sprintf(zip_code, "%s", value);
            read_fields |=0b00010;

        } else if (!strcmp(key, "street")) {
            sprintf(street, "%s", value);
            read_fields |=0b00100;

        } else if (!strcmp(key, "street_number")) {
            sprintf(street_number, "%s", value);
            read_fields |=0b01000;

        } else if (!strcmp(key, "city")) {
            sprintf(city, "%s", value);
            read_fields |=0b10000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }
    if (read_fields != 0b11111){
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Address(email, zip_code, street, street_number, city);
}
    