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

#define MAX_CONNECTIONS 200
#define TRUE 1
#define FALSE 0

class Server {
    public:
        Server(int);
        void run();

    private:
        int sockfd;
        int sockPort;
        int end_server = 0;
};