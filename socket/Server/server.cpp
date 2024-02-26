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
    serverAddress.sin_port = htons(8080); 
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = sockPort;

    if (bind(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1){
        throw std::invalid_argument("Error binding the socket");
    }
}

void Server::accept_clients(){
    if (listen(sockfd, MAX_CONNECTIONS) == -1) {
        throw std::invalid_argument("Error listening in socket");
    }

    errno = 0;
    while(1) {
        int newClient = accept(sockfd, nullptr, nullptr);
        if (newClient == -1) {
            if (errno != EWOULDBLOCK) {
                throw std::invalid_argument("Error listening in socket");
            }
            printf("niente\n");
            break;
        }
        std::cout << newClient << std::endl;
        clients.push_back(newClient);
    }
}


