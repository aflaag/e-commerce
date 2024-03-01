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

void Server::run(){

    fd_set working_set;
    char buffer[1000];
    int rc, i, new_client, ready_requests = 0;
    struct timeval timeout;
    std::string out_str;
    int client_id;
    bool response;

    FD_ZERO(&current_set);
    max_fd = sockfd;
    FD_SET(sockfd, &current_set);

    while(!end_server) {
        printf("select failed\n");

        memcpy(&working_set, &current_set, sizeof(current_set));

        // Listens for incoming requests. Returns the number of incoming requests
        rc = select(max_fd + 1, &working_set, NULL, NULL, &timeout);

        if (rc < 0) {
            printf("select failed");
            break;
        }
        // if (rc == 0) {
        //     timeout.tv_sec  = 1;
        //     timeout.tv_usec = 0;
        //     continue;
        // }

        ready_requests = rc;

        // For all connections (itself included)

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
    do {
        new_client = accept(sockfd, NULL, NULL);
        
        if (new_client < 0) {

            if (errno != EWOULDBLOCK) {
                printf("accept error");
                end_server = TRUE;
            }
            break;
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
        close(i);
        FD_CLR(i, &current_set);
        if (i == max_fd) {
            while (FD_ISSET(max_fd, &current_set) == FALSE)
                max_fd -= 1;
        }
        // don't answer to the next requests
        return;
    }

    // std::istringstream iss(msg);
    // std::vector<std::string> lines;

    // std::string line;
    // while (std::getline(iss, line, '\n')) {
    //     lines.push_back(line);
    // }

    // for (const auto& l : lines) {
    //     std::cout << l << std::endl;
    // }

    handler->send_to_managers(i, msg);
}

void Server::close_connections() {
    int i;

    for (i=0; i <= max_fd; ++i){
        if (FD_ISSET(i, &current_set))
            close(i);
    }
}