#include "update_refund.h"

UpdateRefund::UpdateRefund(char* refund_request_in, char* timestamp, char* next_state){

    refund_request = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    new_time = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    state = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(refund_request, refund_request_in);
    strcpy(new_time, timestamp);
    strcpy(state, next_state);
}

UpdateRefund::UpdateRefund(char* refund_request_in, char* courier_id){

    refund_request = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    courier = (char*) malloc(sizeof(char) * PARAMETERS_LEN);
    state = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(refund_request, refund_request_in);
    strcpy(courier, courier_id);
    sprintf(state, "Assigned");
}

UpdateRefund::~UpdateRefund(){
    free(refund_request);
    free(new_time);
    free(courier);
    free(state);
}

UpdateRefund* UpdateRefund::from_stream(redisReply* reply, int stream_num, int msg_num, int starting_value) {

    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char refund_id[PARAMETERS_LEN];
    char timestamp[PARAMETERS_LEN];
    char new_state[PARAMETERS_LEN];
    char courier_id[PARAMETERS_LEN];

    char read_fields = 0b00000;

    for (int field_num = starting_value; field_num < ReadStreamMsgNumVal(reply, stream_num, msg_num); field_num += 2) {
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num, key);
        ReadStreamMsgVal(reply, stream_num, msg_num, field_num + 1, value);
                    
        if (!strcmp(key, "refund")) {
            sprintf(refund_id, "%s", value);
            read_fields |=0b1000;

        } else if (!strcmp(key, "refund_end")) {
            sprintf(timestamp, "%s", value);
            sprintf(new_state, "%s", "Refunded");
            read_fields |=0b0100;

        } else if (!strcmp(key, "refund_start")) {
            sprintf(timestamp, "%s", value);
            sprintf(new_state, "%s", "OnGoing");
            read_fields |=0b0010;

        } else if (!strcmp(key, "courier")) {
            sprintf(courier_id, "%s", value);
            sprintf(new_state, "%s", "Assigned");
            read_fields |=0b0001;

        } else {
            throw std::invalid_argument("Stream error: invalid fields");
        }
    }
    
    if (read_fields != 0b1100 && read_fields != 0b1010 && read_fields != 0b1001) {
        throw std::invalid_argument("Stream error: invalid fields");
    }
    if (read_fields == 0b1001){
        return new UpdateRefund(refund_id, courier_id);
    }

    return new UpdateRefund(refund_id, timestamp, new_state);
}
    
std::string UpdateRefund::to_update_query() {
    std::string str_refund = refund_request;
    std::string str_state = state;
    if(str_state == "Assigned"){
        std::string str_courier = courier;
        return "UPDATE RefundRequest SET refund_state = \'" + str_state + "\', courier = \'" + str_courier + "\', refund_assignment_instant = CURRENT_TIMESTAMP WHERE id = " + str_refund;
    }
    std::string str_time = replace_substring(new_time, SPACE_REDIS_STRING, SPACE);
    std::string str_update = (str_state == "OnGoing" ? "refund_start" : "refund_end");

    return "UPDATE RefundRequest SET refund_state = \'" + str_state + "\', " + str_update + " = \'" + str_time + "\' WHERE id = " + str_refund;
}
