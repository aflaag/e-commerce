UpdateRefund#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <sys/times.h>
#include <cerrno>
  
using namespace std; 

#define SERVER_PORT  8080


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
  int rc;
  int len;
  char buffer[1000];

    // specifying address 
    sockaddr_in addr; 
    addr.sin_family = AF_INET; 
    addr.sin_port = htons(8080); 
    addr.sin_addr.s_addr = INADDR_ANY; 
  
    // sending connection request 

    connect(clientSocket, (struct sockaddr*)&addr, 
            sizeof(addr)); 

    const char* message = "Hello, server!"; 
    send(clientSocket, message, strlen(message), 0);
    printf("banan\n");
    rc = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (rc < 0)
        {
            if (errno != EWOULDBLOCK)
            {
            perror("  recv() failed");
            }
        }

        if (rc == 0)
        {
            printf("  Connection closed\n");
        }

        len = rc;
        printf("  %d bytes received\n", len);
    
    
    // closing socket 
    close(clientSocket); 
  
    return 0; 
}