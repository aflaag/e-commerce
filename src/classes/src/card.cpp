#include "card.h"

Card::Card(char* card_number, 
        char* card_owner_email){

    number = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    email = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(number, card_number);
    strcpy(email, card_owner_email);
}

Card::~Card(){
    free(number);
    free(email);
}

Card* Card::from_stream(redisReply* reply, int stream_num, int msg_num){

    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char number[PARAMETERS_LEN];
    char email[PARAMETERS_LEN];

    char read_fields = 0b00;


    for (int field_num = 2; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num +=  2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "number")) {
            sprintf(number, "%s", value);
            read_fields |=0b01;

        } else if (!strcmp(key, "email")) {
            sprintf(email, "%s", value);
            read_fields |=0b10;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }

    if (read_fields != 0b11){
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Card(number, email);
}
    