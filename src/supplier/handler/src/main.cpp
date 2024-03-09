#include "main.h"

int main(){
    int num_types = 5;
    std::string req_types[num_types] = {"add-product", "add-restock", "add-supplier", "update-product", "view-statistic"};

    Server server(42071, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}