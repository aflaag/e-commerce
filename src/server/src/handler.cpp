#include "handler.h"

Handler::Handler(const char* redis_server, int redis_port, std::string req_types[], int num_req_types){
    
    c2r = redisConnect(redis_server, redis_port);
    types = req_types;
    num_types = num_req_types;

    init_streams();
}

void Handler::send_to_managers(int client_id, std::string msg){

    redisReply* reply;
    bool is_valid_req;
    int i;

    // Get request type
    for(i = 0; i < msg.length(); i++){
        if (msg[i] == 32)  break;   // 32 is the Space char in ASCII
    }
    
    if (i >= msg.length()){
        // Invalid msg
        std::cout << "Invalid msg" << std::endl;
        return;
    }

    std::string req_type = msg.substr(0, i);
    std::string req_cmd = msg.substr(i);

    is_valid_req = false;
    for(i = 0; i < num_types; i++){
        if(req_type.compare(types[i])){
            is_valid_req = true;
            break;
        }
    }

    if(!is_valid_req){
        // Invalid request type
        std::cout << "Invalid request type" << std::endl;
        return;
    }

    // Send command on the -in stream of the corresponding manager
    std::string redis_cmd = "XADD " + req_type +"-in * client_id " + std::to_string(client_id) + " " + req_cmd;

    std::cout << "\n" << redis_cmd << std::endl; 

    reply = RedisCommand(c2r, redis_cmd.c_str());

    assertReply(c2r, reply);
    dumpReply(reply, 0);
}

std::string Handler::read_from_managers(){
    return "";
}

void Handler::init_streams(){
    redisReply* reply;
    std::string read_stream;
    std::string write_stream;

    for(int i = 0; i < num_types; i++){

        read_stream = types[i] + "-in";
        write_stream = types[i] + "-out";

        reply = RedisCommand(c2r, "DEL %s", read_stream.c_str());
        assertReply(c2r, reply);
        dumpReply(reply, 0);

        reply = RedisCommand(c2r, "DEL %s", write_stream.c_str());
        assertReply(c2r, reply);
        dumpReply(reply, 0);

        initStreams(c2r, read_stream.c_str());
        initStreams(c2r, write_stream.c_str());
    }
}