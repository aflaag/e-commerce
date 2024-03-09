#include "main.h"

int main(){
    int num_types = 4;
    std::string req_types[num_types] = {"take-purchase-delivery", "update-assigned-delivery", "update-refund-request", "view-available-purchase"};

    Server server(42070, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}