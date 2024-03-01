#include <cstring> 
#include <iostream> 
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h> 
#include <cerrno>
#include <vector>
#include <sstream>

#include "handler.h"

#define MAX_CONNECTIONS 200
#define TRUE 1
#define FALSE 0

class Server {
    public:
        Server(int server_port, const char* redis_ip, int redis_port, std::string req_types[], int num_req_types);
        void run();

    private:
        void add_new_clients();
        void receive(int i);
        void close_connections();

        int sockfd;
        int sockPort;
        int end_server = 0;
        fd_set current_set;
        int max_fd;
        Handler* handler;
};