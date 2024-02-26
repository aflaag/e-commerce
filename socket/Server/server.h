#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <sstream>
#include <list>
#include <cerrno>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring> 
#include <iostream>

#define MAX_CONNECTIONS 200

class Server {
    public:
        Server(int);
        void accept_clients();

    private:
        int sockfd;
        int sockPort;
        std::list<int> clients;
};