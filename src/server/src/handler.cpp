#include "handler.h"

Handler::Handler(const char* redis_ip, int redis_port, std::string req_types[], int num_req_types){
    
    c2r = redisConnect(redis_ip, redis_port);
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

bool Handler::read_from_managers(std::string* out_str_ptr, int* client_id_ptr){

    redisReply* reply;
    char response[1000], msg_id[30], tmp_buffer[30], client_id[30], resp_status[30], num_rows[30], row[30];
    int i, j, num_rows_int, curr_row, row_columns;
    std::string tmp_str;
    std::string out_str;

    for(i = 0; i < num_types; i++){
        reply = RedisCommand(c2r, "XREADGROUP GROUP main handler COUNT 1 STREAMS %s-out >", types[i].c_str());

        assertReply(c2r, reply);
        if (ReadNumStreams(reply) == 0)     // If empty, check next manager
            continue;

        // Get client id
        ReadStreamMsgVal(reply, 0, 0, 0, tmp_buffer);
        ReadStreamMsgVal(reply, 0, 0, 1, client_id);    

        if(strcmp(tmp_buffer, "client_id"))
            continue;   // Ignore invalid response

        *client_id_ptr = strtol(client_id, NULL, 10);
        
        // Get num rows
        ReadStreamMsgVal(reply, 0, 0, 4, tmp_buffer);
        ReadStreamMsgVal(reply, 0, 0, 5, num_rows);     

        if(strcmp(tmp_buffer, "num_rows"))
            continue;   // Ignore invalid response

        num_rows_int = strtol(num_rows, NULL, 10);

        // Get resp status
        ReadStreamMsgVal(reply, 0, 0, 2, tmp_buffer);
        ReadStreamMsgVal(reply, 0, 0, 3, resp_status);

        if(strcmp(tmp_buffer, "resp_status"))
            continue;   // Ignore invalid response

        if(strcmp(resp_status, "REQUEST#SUCCESS"))
            num_rows_int = 0;   // "Burn" possible tuples

        freeReplyObject(reply);

        curr_row = 0;

        // Add response status
        tmp_str = resp_status;
        out_str = tmp_str + "\n";
        
        while(curr_row < num_rows_int - 1){
            reply = RedisCommand(c2r, "XREADGROUP GROUP main handler BLOCK 0 COUNT 1 STREAMS %s-out >", types[i].c_str());

            assertReply(c2r, reply);
            if (ReadNumStreams(reply) == 0){
                out_str = "BAD#RESPONSE";
                break;
            }

            // Check if the first key/value pair is the num_rows
            ReadStreamMsgVal(reply, 0, 0, 0, tmp_buffer);    // Index of first field of msg = 0
            ReadStreamMsgVal(reply, 0, 0, 1, row);    

            if(strcmp(tmp_buffer, "row")){
                out_str = "BAD#TUPLES";
                break;
            }

            curr_row = strtol(row, NULL, 10);
            row_columns = ReadStreamMsgNumVal(reply, 0, 0);

            out_str = out_str + "{";

            for(j = 2; j < row_columns; j += 2){

                ReadStreamMsgVal(reply, 0, 0, j, tmp_buffer);    
                tmp_str = tmp_buffer;
                out_str = out_str + tmp_str +": ";

                ReadStreamMsgVal(reply, 0, 0, j+1, tmp_buffer);    
                tmp_str = tmp_buffer;
                out_str = out_str + tmp_str +", ";
            }

            out_str = out_str + "}\n";
        }

        *out_str_ptr = out_str;

        return true;
    }

    return false;
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