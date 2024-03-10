#ifndef server_h
#define server_h

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
#include <signal.h>

#include "handler.h"
#include "../../lib/con2db/pgsql.h"

#define MAX_CONNECTIONS 200
#define TRUE 1
#define FALSE 0

#define POSTGRESQL_SERVER "localhost"
#define POSTGRESQL_PORT "5432"
#define POSTGRESQL_USER "handler"
#define POSTGRESQL_PSW "handler"
#define POSTGRESQL_DBNAME "logdb"

class Server {
    public:
        Server(const char* server_id, int server_port, const char* redis_ip, int redis_port, std::string req_types[], int num_req_types);
        void run();
        ~Server();

    private:
        void add_new_clients();
        void receive(int i);
        void close_connections();
        void send_response(int client_id, std::string out_str);

        const char* server;
        int sockfd;
        int sockPort;
        fd_set current_set;
        int max_fd;
        Handler* handler;

        Con2DB log_db = Con2DB(POSTGRESQL_SERVER, POSTGRESQL_PORT, POSTGRESQL_USER, POSTGRESQL_PSW, POSTGRESQL_DBNAME);
        PGresult *query_res;
};

#endif
