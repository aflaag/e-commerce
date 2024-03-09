#include "main.h"

// void handle_sigint(int s){
//     end_server = 1;
// }

int main(){
    // struct sigaction sigIntHandler;

    // sigIntHandler.sa_handler = handle_sigint;
    // sigemptyset(&sigIntHandler.sa_mask);
    // sigIntHandler.sa_flags = 0;

    // sigaction(SIGINT, &sigIntHandler, NULL);

    int num_types = 10;
    std::string req_types[num_types] = {"add-customer", "add-address", "search-products", "add-card", "add-purchase", "add-rating", "add-refund-request", "delete-purchase", "view-order", "view-refund"};

    Server server(42069, REDIS_SERVER, REDIS_PORT, req_types, num_types);

    server.run();
}