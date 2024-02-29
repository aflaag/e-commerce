#include "server.h"
#include "handler.h"

#define REDIS_SERVER "localhost"
#define REDIS_PORT 6379

int main(){
    std::string msg;
    int num_types = 2;
    std::string req_types[num_types] = {"add-customer", "add-address"};

    Handler handler(REDIS_SERVER, REDIS_PORT, req_types, num_types);

    msg = "add-customer name example surname example email sara@gmail.com phone_number +34567453";
    handler.send_to_managers(1, msg);

    msg = "add-address email user2@gmail.com zip_code 0000 street sara street_number 1b city 1";
    handler.send_to_managers(2, msg);

    msg = "ciao name example surname example email sara@gmail.com phone_number +34567453";
    handler.send_to_managers(3, msg);
}