#include "server.h"

int kill_server = FALSE;

void handle_signals(int s){
    kill_server = TRUE;
    std::cout << "\nKilling server..." << std::endl;
}

Server::Server(const char* server_id, int server_port, const char* redis_ip, int redis_port, std::string req_types[], int num_req_types) {
    int on = 1;

    server = server_id;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockPort = server_port;
    
    if (sockfd < 0) {
        throw std::invalid_argument("Error creating the socket");
    }

    if (setsockopt(sockfd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on)) < 0) {
        perror("setsockopt() failed");
        close(sockfd);
        exit(-1);
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

    struct sigaction signalHandler;

    fd_set working_set;
    int rc, i, new_client, ready_requests;
    struct timeval timeout;

    int client_id;
    bool response;
    char query[QUERY_LEN];
    std::string out_str;

    signalHandler.sa_handler = handle_signals;
    sigemptyset(&signalHandler.sa_mask);
    signalHandler.sa_flags = 0;

    sigaction(SIGINT, &signalHandler, NULL);
    sigaction(SIGTERM, &signalHandler, NULL);

    ready_requests = 0;
    FD_ZERO(&current_set);
    max_fd = sockfd;
    FD_SET(sockfd, &current_set);

    timeout.tv_sec  = 0;
    timeout.tv_usec = 5000;

    std::cout << "Running " << server << " server" << std::endl;

    while(!kill_server) {

        memcpy(&working_set, &current_set, sizeof(current_set));

        // Listens for incoming requests. Returns the number of incoming requests
        rc = select(max_fd + 1, &working_set, NULL, NULL, &timeout);

        if (rc < 0) {
            std::cout << "\nSocket selection error" << std::endl;
            break;
        }

        ready_requests = rc;

        // For all connections (itself included)
        for (i = 0; i <= max_fd && ready_requests > 0; ++i) {
            
            if ((i != sockfd) && FD_ISSET(i, &working_set)) {
                ready_requests -= 1;
                    receive(i);
            }
        }

        if (FD_ISSET(sockfd, &working_set)) {
            add_new_clients();
        }

        // Read managers responses and send to clients
        response = true;
        while(response){
            out_str = "";
            client_id = -1;

            response = handler->read_from_managers(&out_str, &client_id);

            if(response){
                send_response(client_id, out_str);
            }
        }

        timeout.tv_sec  = 1;
        timeout.tv_usec = 0;
    }

    // Close connections
    close_connections();
}

void Server::add_new_clients() {
    // Accept incoming connections
    int new_client;
    char query[QUERY_LEN];

    do {
        new_client = accept(sockfd, NULL, NULL);
        
        if (new_client < 0) {

            if (errno != EWOULDBLOCK) {
                std::cout << "\nClient accept failed" << std::endl;
                kill_server = TRUE;
            }
            break;
        }

        sprintf(query, "INSERT INTO Client(server_name, file_descriptor, connection_instant) VALUES (\'%s\', %d, CURRENT_TIMESTAMP)", server, new_client);
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
            std::cout << "\nClient accept failed" << std::endl;
        }
    } while (new_client != -1);
}

void Server::send_response(int client_id, std::string out_str) {
    char query[QUERY_LEN];

    size_t pos = out_str.find('\n');

    // Extract the first line
    std::string first_line = out_str.substr(0, pos);

    sprintf(query, "WITH max_client_conn AS (SELECT max(connection_instant) AS instant FROM Client WHERE server_name = \'%s\' AND file_descriptor = %d), "
                    "     last_request AS (SELECT MAX(c.request_instant) AS instant FROM Communication AS c, max_client_conn AS m WHERE c.client_server_name = \'%s\' AND c.client_file_descriptor = %d AND c.client_connection_instant = m.instant) "
                    "UPDATE Communication SET response_status = \'%s\', response_instant = CURRENT_TIMESTAMP "
                    "WHERE client_server_name = \'%s\' AND client_file_descriptor = %d AND client_connection_instant = (SELECT instant FROM max_client_conn) AND request_instant = (SELECT instant FROM last_request)", server, client_id, server, client_id, first_line.c_str(), server, client_id);
    query_res = log_db.RunQuery(query, false);

    if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
        send(client_id, "SERVER_ERROR", 12, 0);
        return;
    }

    std::cout << "\nClient id: " << client_id << " - Response: " << out_str << std::endl;
    send(client_id, out_str.c_str(), out_str.length(), 0);
}


void Server::receive(int i) {
    std::string	msg;
    char buffer[100];
    int rc;
    int close_conn = FALSE;
    char query[QUERY_LEN];

    // Read incoming data
    do {
        bzero(buffer, sizeof(buffer));
        rc = recv(i, buffer, sizeof(buffer) -1, 0);

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

    // If connection was closed by client or crashed
    if (close_conn) {
        sprintf(query, "UPDATE Client SET disconnection_instant = CURRENT_TIMESTAMP WHERE server_name = \'%s\' AND file_descriptor = %d AND disconnection_instant IS NULL", server, i);
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

    sprintf(query, "INSERT INTO Communication(request, request_instant, client_server_name, client_file_descriptor, client_connection_instant)"
                   "VALUES (\'%s\', CURRENT_TIMESTAMP, \'%s\', %d, (SELECT connection_instant FROM Client WHERE disconnection_instant IS NULL and server_name = \'%s\' and file_descriptor = %d))", msg.c_str(), server, i, server, i);

    query_res = log_db.RunQuery(query, false);
    if (PQresultStatus(query_res) != PGRES_COMMAND_OK && PQresultStatus(query_res) != PGRES_TUPLES_OK) {
        send(i, "SERVER_ERROR", 12, 0);
        return;
    }

    if(!handler->send_to_managers(i, msg)){
        send_response(i, "BAD_REQUEST");
    }
}

void Server::close_connections() {
    int i;
    char query[QUERY_LEN];

    for (i=0; i <= max_fd; ++i){
        if (FD_ISSET(i, &current_set)) {
            sprintf(query, "UPDATE Client SET disconnection_instant = CURRENT_TIMESTAMP WHERE server_name = \'%s\' AND file_descriptor = %d AND disconnection_instant IS NULL", server, i);
            query_res = log_db.RunQuery(query, false);
            close(i);
        }
    }
}
