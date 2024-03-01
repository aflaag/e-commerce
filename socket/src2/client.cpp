#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <unistd.h> 
#include <sys/times.h>
#include <cerrno>
  
using namespace std; 

#define SERVER_PORT  42069


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

    memset(buffer, 0, 1000);

    // specifying address 
    sockaddr_in addr; 
    addr.sin_family = AF_INET; 
    addr.sin_port = htons(42069); 
    addr.sin_addr.s_addr = INADDR_ANY;
  
    // sending connection request 

    connect(clientSocket, (struct sockaddr*)&addr, sizeof(addr)); 

    memset(buffer, 0, 1000);
    const char* message1 = "add-customer name example surname example email sara@gmail.com phone_number +34567453"; 
    send(clientSocket, message1, strlen(message1), 0);
    rc = recv(clientSocket, buffer, sizeof(buffer), 0);

    printf("%s", buffer);

    memset(buffer, 0, 1000);
    const char* message2 = "add-address email user2@gmail.com zip_code 0000 street ivan street_number 1b city 1"; 
    send(clientSocket, message2, strlen(message2), 0);
    rc = recv(clientSocket, buffer, sizeof(buffer), 0);

    printf("%s", buffer);

    memset(buffer, 0, 1000);
    const char* message3 = "search-products product rt"; 
    send(clientSocket, message3, strlen(message3), 0);
    rc = recv(clientSocket, buffer, sizeof(buffer), 0);

    printf("%s", buffer);

    memset(buffer, 0, 1000);

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
    
    // closing socket 
    close(clientSocket); 
  
    return 0; 
}