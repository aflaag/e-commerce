#include <cstring> 
#include <iostream> 
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h> 
#include <cerrno>
#include <vector>
#include <sstream>

#define MAX_CONNECTIONS 200
#define TRUE 1
#define FALSE 0

class Server {
    public:
        Server(int port);
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
};