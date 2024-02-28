#include "refund.h"

Refund::Refund(char* refund_id, 
        char* refund_request_instant, 
        char* refund_state, 
        char* refund_assigned_delivery){

    id = (char*) malloc(sizeof(char) * 100);
    request_instant = (char*) malloc(sizeof(char) * 100);
    state = (char*) malloc(sizeof(char) * 100);
    assigned_delivery = (char*) malloc(sizeof(char) * 100);

    strcpy(id, refund_id);
    strcpy(request_instant, refund_request_instant);
    strcpy(state, refund_state);
    strcpy(assigned_delivery, refund_assigned_delivery);
    
}

Refund::~Refund(){
    free(id);
    free(request_instant);
    free(state);
    free(assigned_delivery);
}

Refund* Refund::from_stream(redisReply* reply, int stream_num, int msg_num){

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
    