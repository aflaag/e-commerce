#include "customer.h"

Customer::Customer(char* customer_email,
        char* customer_name,
        char* customer_surname,
        char* customer_phone_number){

    email = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    name = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    surname = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    phone_number = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(email, customer_email);
    strcpy(name, customer_name);
    strcpy(surname, customer_surname);
    strcpy(phone_number, customer_phone_number);
}

Customer::~Customer(){
    free(email);
    free(name);
    free(surname);
    free(phone_number);
}

Customer* Customer::from_stream(redisReply* reply, int stream_num, int msg_num){

    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char name[PARAMETERS_LEN];
    char email[PARAMETERS_LEN];
    char surname[PARAMETERS_LEN];
    char phone_number[PARAMETERS_LEN];

    char read_fields = 0b0000;

    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
        
        if (!strcmp(key, "name")) {
            snprintf(name, PARAMETERS_LEN, "%s", value);
            read_fields |= 0b0001;

        } else if (!strcmp(key, "email")) {
            snprintf(email, PARAMETERS_LEN, "%s", value);
            read_fields |= 0b0010;

        } else if (!strcmp(key, "surname")) {
            snprintf(surname, PARAMETERS_LEN, "%s", value);
            read_fields |= 0b0100;

        } else if (!strcmp(key, "phone_number")) {
            snprintf(phone_number, PARAMETERS_LEN, "%s", value);
            read_fields |= 0b1000;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b1111){
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Customer(email, name, surname, phone_number);
}
    