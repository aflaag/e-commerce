#include "refund.h"

Refund::Refund(char* refund_id, 
        char* refund_request_instant, 
        char* refund_state, 
        char* refund_assigned_delivery){

    id = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    request_instant = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    state = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    assigned_delivery = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

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

    char id[100];
    char* request_instant;
    char state[100] = "Requested";
    char assigned_delivery[100];

    // prendo il timestamp corrente
    auto current_time = std::chrono::system_clock::now();
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);
    request_instant = std::ctime(&current_time_t);

    ReadStreamMsgVal(reply, stream_num, msg_num, 2, key);
    ReadStreamMsgVal(reply, stream_num, msg_num, 3, value);
                
    if (!strcmp(key, "assigned_delivery")) {
        sprintf(assigned_delivery, "%s", value);

    } else {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Refund(id,request_instant, state, assigned_delivery);
}
    