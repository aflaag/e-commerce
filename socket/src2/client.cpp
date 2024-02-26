#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <sys/times.h>
#include <cerrno>
  
using namespace std; 


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
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
  
    // specifying address 
    sockaddr_in serverAddress; 
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons(8080); 
    serverAddress.sin_addr.s_addr = INADDR_ANY; 
  
    // sending connection request 

    connect(clientSocket, (struct sockaddr*)&serverAddress, 
            sizeof(serverAddress)); 
    int i = 0;
    while(i < 10){
    connect(clientSocket, (struct sockaddr*)&serverAddress, 
            sizeof(serverAddress));
    cout << i << endl;
    micro_sleep(100000);
    i ++;
    }
    // closing socket 
    close(clientSocket); 
  
    return 0; 
}