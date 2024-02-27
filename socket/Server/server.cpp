#include "server.h"

Server::Server(int port) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockPort = port;
    
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
    serverAddress.sin_port = htons(port); 
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = sockPort;

    if (bind(sockfd, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1){
        throw std::invalid_argument("Error binding the socket");
    }
    if (listen(sockfd, MAX_CONNECTIONS) == -1) {
        throw std::invalid_argument("Error listening in socket");
    }
}

void Server::run(){
    fd_set current_set, working_set;
    char buffer[1000];
    int max_fd, rc, close_conn, i, new_client, ready_requests = 0;
    struct timeval timeout;

    FD_ZERO(&current_set);
    max_fd = sockfd;
    FD_SET(sockfd, &current_set);

    timeout.tv_sec  = 30;
    timeout.tv_usec = 0;

    while(!end_server) {
        printf("select failed");

        memcpy(&working_set, &current_set, sizeof(current_set));

        // Listens for incoming requests. Returns the number of incoming requests
        rc = select(max_fd + 1, &working_set, NULL, NULL, &timeout);

        if (rc < 0) {
            printf("select failed");
            break;
        }
        if (rc == 0) {
            printf("timeout");
            break;
        }

        ready_requests = rc;

        // For all connections (itself included)

        for (i = 0; i <= max_fd && ready_requests > 0; ++i) {

            if (FD_ISSET(i, &working_set)) {
                ready_requests -= 1;
                
                // If it's itself
                if (i == sockfd) {

                    // Accept incoming conncetions
                    do {
                        new_client = accept(sockfd, NULL, NULL);
                        printf("%d\n", new_client);
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
                    } while (new_client != -1);

                // If it's a client
                } else {
                    close_conn = FALSE;

                    // Read incoming data
                    do {
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
                        printf("%s\n", buffer);

                        // we have in the buffer the request
                        // process che responce
                        // send the responce
                            // rc = send(i, buffer, len, 0);

                        // send_to_managers(i, stringa_letta);

                        if (rc < 0) {
                            perror("send() failed");
                            close_conn = TRUE;
                            break;
                        }
                    } while (TRUE);

                    // If conncetion was closed by client or crashed
                    if (close_conn) {
                        close(i);
                        FD_CLR(i, &current_set);
                        if (i == max_fd) {
                            while (FD_ISSET(max_fd, &current_set) == FALSE)
                                max_fd -= 1;
                        }
                    }
                }
            }
        }

        // Read from managers (usare pair per ritornare lista di tuple (socket, stringa))

        // Send responses
    }

    // Close connections
    for (i=0; i <= max_fd; ++i){
        if (FD_ISSET(i, &current_set))
            close(i);
    } 
}


