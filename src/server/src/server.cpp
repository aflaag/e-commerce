#include "server.h"

Server::Server(int server_port, const char* redis_ip, int redis_port, std::string req_types[], int num_req_types) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockPort = server_port;
    
    if (sockfd < 0) {
        throw std::invalid_argument("Error creating the socket");
    }

    int flags = fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;

    if (fcntl(sockfd, F_SETFL, flags) == -1){
		throw std::runtime_error("Error setting socket to NON-BLOCKING");
    }
    
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons(server_port); 
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1){
        throw std::invalid_argument("Error binding the socket");
    }
    if (listen(sockfd, MAX_CONNECTIONS) == -1) {
        throw std::invalid_argument("Error listening in socket");
    }

    handler = new Handler(redis_ip, redis_port, req_types, num_req_types);
}

Server::~Server() {
    log_db.finish();
    close_connections();
}

void Server::run(){

    fd_set working_set;
    char buffer[1000];
    int rc, i, new_client, ready_requests = 0;
    struct timeval timeout;
    std::string out_str;
    int client_id;
    bool response;
    char query[1000];
    unsigned int counter;

    FD_ZERO(&current_set);
    max_fd = sockfd;
    FD_SET(sockfd, &current_set);

    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;

    counter = 0;

    while(!end_server && counter <= 20) {
        printf("Working... - Listen counter: %d\n", counter);
        counter++;

        memcpy(&working_set, &current_set, sizeof(current_set));

        // Listens for incoming requests. Returns the number of incoming requests
        rc = select(max_fd + 1, &working_set, NULL, NULL, &timeout);

        if (rc < 0) {
            printf("select failed\n");
            break;
        }

        ready_requests = rc;

        // For all connections (itself included)

        if(ready_requests > 0){
            counter = 0;    // reset server timeout counter
        }

        for (i = 0; i <= max_fd && ready_requests > 0; ++i) {
            
            if (FD_ISSET(i, &working_set)) {
                ready_requests -= 1;
                
                // If it's itself
                if (i == sockfd) {
                    printf("new client\n");
                    add_new_clients();

                // If it's a client
                } else {
                    printf("new msg\n");
                    receive(i);
                }

            }
        }

        // Read managers responses and send to clients
        response = true;
        while(response){
            out_str = "";
            client_id = -1;

            response = handler->read_from_managers(&out_str, &client_id);

            if(response){
                counter = 0;    // reset server timeout counter

                size_t pos = out_str.find('\n');

                // Extract the first line
                std::string first_line = out_str.substr(0, pos);
                std::cout << first_line << std::endl;

                sprintf(query, "WITH max_client_conn AS (SELECT max(connection_instant) AS instant FROM Client WHERE file_descriptor = %d), "
                               "     last_request AS (SELECT MAX(c.request_instant) AS instant FROM Communication AS c, max_client_conn AS m WHERE c.client_file_descriptor = %d AND c.client_connection_instant = m.instant) "
                               "UPDATE Communication SET response_status = \'%s\', response_instant = CURRENT_TIMESTAMP "
                               "WHERE client_file_descriptor = %d AND client_connection_instant = (SELECT instant FROM max_client_conn) AND request_instant = (SELECT instant FROM last_request)", client_id, client_id, first_line.c_str(), client_id);
                query_res = log_db.RunQuery(query, false);

                if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
                    send(new_client, "SERVER_ERROR", 12, 0);
                    continue;
                }


                std::cout << client_id << " - " << out_str << std::endl;
                send(client_id, out_str.c_str(), out_str.length(), 0);
            }
        }

        timeout.tv_sec  = 1;
        timeout.tv_usec = 0;
    }

    // Close connections
    close_connections();
}


void Server::add_new_clients() {
    // Accept incoming conncetions
    int new_client;
    char query[1000];

    do {
        new_client = accept(sockfd, NULL, NULL);
        
        if (new_client < 0) {

            if (errno != EWOULDBLOCK) {
                printf("accept error");
                end_server = TRUE;
            }
            break;
        }

        sprintf(query, "INSERT INTO Client(file_descriptor, connection_instant) VALUES (%d, CURRENT_TIMESTAMP)", new_client);
        query_res = log_db.RunQuery(query, false);

        if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
            send(new_client, "SERVER_ERROR", 12, 0);
            close(new_client);
            return;
        }

        FD_SET(new_client, &current_set);
        if (new_client > max_fd) {
            max_fd = new_client;
        }
        if (fcntl(new_client, F_SETFL, O_NONBLOCK) == -1) {
		    printf("flag set error");
        }
    } while (new_client != -1);
}


void Server::receive(int i) {
    std::string	msg;
    char buffer[100];
    int rc;
    int close_conn = FALSE;
    char query[1000];

    // Read incoming data
    do {
        bzero(buffer, 100);
        rc = recv(i, buffer, sizeof(buffer), 0);

        if (rc < 0) {
            if (errno != EWOULDBLOCK) {
                close_conn = TRUE;
            }
            break;
        }
        if (rc == 0) {
            close_conn = TRUE;
            break;
        }
        msg.append(buffer);

    } while (TRUE);

    // If conncetion was closed by client or crashed
    if (close_conn) {
        sprintf(query, "UPDATE Client SET disconnection_instant = CURRENT_TIMESTAMP WHERE file_descriptor = %d AND disconnection_instant IS NULL", i);
        query_res = log_db.RunQuery(query, false);

        close(i);
        FD_CLR(i, &current_set);
        if (i == max_fd) {
            while (FD_ISSET(max_fd, &current_set) == FALSE)
                max_fd -= 1;
        }
        // don't answer to the next requests
        return;
    }

    sprintf(query, "INSERT INTO Communication(request, request_instant, client_file_descriptor, client_connection_instant)"
                   "VALUES (\'%s\', CURRENT_TIMESTAMP, %d, (SELECT connection_instant FROM Client WHERE disconnection_instant IS NULL and file_descriptor = %d))", msg.c_str(), i, i);

    query_res = log_db.RunQuery(query, false);
    if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
        send(i, "SERVER_ERROR", 12, 0);
        return;
    }

    if(!handler->send_to_managers(i, msg)){
        send(i, "BAD_REQUEST", 18, 0);
    }
}

void Server::close_connections() {
    int i;
    char query[1000];

    for (i=0; i <= max_fd; ++i){
        if (FD_ISSET(i, &current_set)) {
            sprintf(query, "UPDATE Client SET disconnection_instant = CURRENT_TIMESTAMP WHERE file_descriptor = %d AND disconnection_instant IS NULL", i);
            query_res = log_db.RunQuery(query, false);
            close(i);
        }
    }
}
