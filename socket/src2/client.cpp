#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <sys/times.h>
#include <cerrno>
  
using namespace std; 

#define SERVER_PORT  12345


int micro_sleep(long usec)
{
    struct timespec ts;
    int res;

    if (usec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    
    return res;  
}

int main() 
{ 
    // creating socket 
    int clientSocket = socket(AF_INET6, SOCK_STREAM, 0); 
  struct sockaddr_in6 addr;
    // specifying address 
    memset(&addr, 0, sizeof(addr));
   addr.sin6_family      = AF_INET6;
   memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
   addr.sin6_port        = htons(SERVER_PORT);
  
    // sending connection request 

    connect(clientSocket, (struct sockaddr*)&addr, 
            sizeof(addr)); 
    int i = 0;
    while(i < 10){
    connect(clientSocket, (struct sockaddr*)&addr, 
            sizeof(addr));
    cout << i << endl;
    i ++;
    }
    // closing socket 
    close(clientSocket); 
  
    return 0; 
}