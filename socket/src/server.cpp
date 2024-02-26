#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 
  
using namespace std; 
  
int main() 
{ 
    // creating socket 
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
  
    // specifying the address 
    sockaddr_in serverAddress; 
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons(8080); 
    serverAddress.sin_addr.s_addr = INADDR_ANY; 
  int i = 0;
    // binding socket. 
    bind(serverSocket, (struct sockaddr*)&serverAddress, 
         sizeof(serverAddress)); 

			listen(serverSocket, 5);
	
		// accepting connection request 
	while (i<10) {
		// listening to the assigned socket 
	  int clientSocket 
			= accept(serverSocket, nullptr, nullptr); 
      if (clientSocket < 0){
        cout << "boh" << endl;
      }
		// recieving data 
    cout << i << endl;
	}
    // closing the socket. 
    close(serverSocket); 
  
    return 0; 
}