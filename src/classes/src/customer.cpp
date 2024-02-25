#include "customer.h"

Customer::Customer(char* customer_email,
        char* customer_name,
        char* customer_surname,
        char* customer_phone_number){

    email = (char*) malloc(sizeof(char) * 100);
    name = (char*) malloc(sizeof(char) * 100);
    surname = (char*) malloc(sizeof(char) * 100);
    phone_number = (char*) malloc(sizeof(char) * 100);

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

    char key[100];
    char value[100];

    char name[100];
    char email[100];
    char surname[100];
    char phone_number[100];

    for (int field_num = 0; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
        
        if (!strcmp(key, "name")) {
            sprintf(name, "%s", value);

        } else if (!strcmp(key, "email")) {
            sprintf(email, "%s", value);

        } else if (!strcmp(key, "surname")) {
            sprintf(surname, "%s", value);

        } else if (!strcmp(key, "phone_number")) {
            sprintf(phone_number, "%s", value);

        } else {
            // printf("%s %s\n", key, value);
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    return new Customer(email, name, surname, phone_number);
}
    