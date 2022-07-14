
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
#define IP_ADDRESS "212.179.121.73" 
#define PORT 1337 

int main(void)
{ 
  int sock; 
  struct sockaddr_in cli_name; //init a struct sockaddr_in, Structure describing an Internet socket address.
  int count;
  int value; 

  printf("Client is alive and establishing socket connection.\n");


  sock = socket(AF_INET, SOCK_STREAM, 0);// creat the socket and return int if the call are secsses.(AF_INIT(domain) - ipv4 internt protocol ,SOCK_DGRAM - Types of sockets.
  // , protocol (we put 0 if we want to serve a socket))

  if (sock < 0) //if the socket unsecsses close it
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }
      

  bzero(&cli_name, sizeof(cli_name)); // The bzero() function write '0' into cli_name in length of sizeof(cli_name)
  cli_name.sin_family = AF_INET; //the address family for the transport address is set to AF_INET
  cli_name.sin_addr.s_addr = inet_addr(IP_ADDRESS);//cast from ipv4 to binary data in order for the computer can read the data that was sent
  cli_name.sin_port = htons(PORT);// converts the host adress to network addres


  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0) //if error was raised when trying to connent to the address, close the program, otherwise, connect the socket to the address
    { perror ("Error establishing communications");
      close(sock);
      exit(1);
    }


  for (count = 1; count <= SIM_LENGTH; count++)
    { read(sock, &value, 4); //read the value that he recvies from the server,and print is to the terminal of the client 
    //(read in respons to the function write() in the server)
      printf("Client has received %d from socket.\n", value);
    }

  printf("Exiting now.\n");

  close(sock);  // close sock..
  exit(0); 

} 
