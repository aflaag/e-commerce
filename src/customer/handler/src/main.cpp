#include "main.h"

int main(){
    int num_types = 10;
    std::string req_types[num_types] = {"add-customer", "add-address", "search-products", "add-card", "add-purchase", "add-rating", "add-refund-request", "delete-purchase", "view-order", "view-refund"};

    Server server(42069, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}