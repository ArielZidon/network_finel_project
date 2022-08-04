#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#define SIM_LENGTH 10 
#define PORT 9999 //the port that the server listen to

int main(void)
{ 
  int sock; //init to be the fd num
  int connect_sock; //init to be the fd num
  struct sockaddr_in serv_name; 
  size_t len; 
  int count;
  
  sock = socket(AF_INET, SOCK_STREAM, 0); //creat the socket and return int if the call are secsses.(AF_INIT(domain) - ipv4 internt protocol ,SOCK_DGRAM - Types of sockets.
  // , protocol (we put 0 if we want to serve a socket))
    
  bzero(&serv_name, sizeof(serv_name));// The bzero() function write '0' into serv_name in length of sizeof(serv_name)
  serv_name.sin_family = AF_INET;                                   
  serv_name.sin_port = htons(PORT); 

  bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name));//assigns the address to the socket
          
  listen(sock, 1); 

  len = sizeof(serv_name); //waiting for incoming connection request to come
  
  connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len); //accepting the first connection from the client and send respons back(the shake handing of the TCP protocol)

  for (count = 1; count <= SIM_LENGTH; count++)
    { 
      write(connect_sock, &count, 4); // write the conetnt of count to the terminal of the client
      printf("Server has written %d to socket.\n", count);
    }

  close(connect_sock); //close the connection socket..
  close(sock); // close the sock..

}
