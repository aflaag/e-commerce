#include "supplier.h"

Supplier::Supplier(char* supplier_business_name){

    business_name = (char*) malloc(sizeof(char) * PARAMETERS_LEN);

    strcpy(business_name, supplier_business_name);
}

Supplier::~Supplier(){
    free(business_name);
}

Supplier* Supplier::from_stream(redisReply* reply, int stream_num, int msg_num){

    char key[PARAMETERS_LEN];
    char value[PARAMETERS_LEN];

    char business_name[PARAMETERS_LEN];

    ReadStreamMsgVal(reply, stream_num, msg_num, 2, key);
    ReadStreamMsgVal(reply, stream_num, msg_num, 3, value);
                    
    if (!strcmp(key, "business_name")) {
        sprintf(business_name, "%s", value);

    } else {
        throw std::invalid_argument("Stream error: invalid fields");
    }

    return new Supplier(business_name);
}
    