#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define SIM_LENGTH 10 

int sock, port,length;
char hostname[200];
struct addrinfo* res;

//<protocol>://<hostname>/<path> -> conNum == 1, 
//<protocol>://<hostname>:<port>/<path> -> conNum == 2
int url_kind(char* url) 
{  
  int i = 0,conNum = 0;
  while (url[i] != '\0') 
  {
    if (url[i] == ':')
      conNum++;
    i++;
  }
  return conNum;
}

void hostname_resolve(int argc, char* argv[])
{
 char* url = argv[1];//argv from the command line..
 int url_c = url_kind(url); //Build this function to analysis of the URL
 if(url_c == 1) //<protocol>://<hostname>/<path> 
 {
    sscanf(url, "http://%99[^/]", hostname);
    if (argc == 3) //just if have protocol number
      sscanf(argv[2], "%d", &port);
    else 
      port = 80;
  }
  else //<protocol>://<hostname>:<port>/<path> 
  { 
    sscanf(url, "http://%99[^:]:%99d/[^\n]", hostname, &port);
  }
  
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) //check if it has been created successfully 
  {
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }
}

int main(int argc, char* argv[])
{ 
  char sendL[4096];
  char recv[4096];
  struct sockaddr_in client,*saddr; 
  char* hostaddr;
  char path[100];
  char* ip_address;
  char* url = argv[1];

  hostname_resolve(argc,argv);//resolve the kind of how we get thr argument from the command line.

  saddr = (struct sockaddr_in*)res->ai_addr;//get the hostname and it's IP address
  hostaddr = inet_ntoa(saddr->sin_addr);
  printf("%s", hostaddr);
  ip_address = hostaddr;
  printf("\nClient is alive and establishing socket connection.\n");
  sock = socket(AF_INET, SOCK_STREAM, 0); //creating the socket, domain is IPv4 Internet protocols, type is SOCK_STREAM and protocol is 0
  if (sock < 0) // if an error was raised when creating the socket
  { 
    perror ("Error opening channel");
    close(sock);
    exit(1);
  }
  
  bzero(&client, sizeof(client)); 
  //init clients attributes
  client.sin_family = AF_INET; 
  client.sin_addr.s_addr = inet_addr(ip_address); // IPv4 numbers-and-dots notation into binary data in network byte order
  client.sin_port = htons(port);

  //Connecting a socket to the address unless there was an error and if so explain what the error is
  if (connect(sock, (struct sockaddr *)&client, sizeof(client)) < 0) 
  { 
    perror ("Error establishing communications");
    close(sock);
    exit(1);
  }
  printf("message has sent!\n\n");
  sleep(1);

  length = sprintf(sendL, "GET %s HTTP/1.0\r\nHost: %s\r\nContent-type: test/plain\r\n\r\n", url, hostname);//send request to the web.
  send(sock, sendL, length, 0);
  memset(recv, 0, 4096); 
  
  while ((length = read(sock, recv, 4095)) > 0) //get the response from the web, write to the terminal. 
  {
    printf("%s", recv);
    memset(recv, 0, 4096);
  }
  if(length < 0)
  {
    close(sock);
    exit(1);
  }
  printf("Exiting now.\n");
  close(sock); 
  exit(0); 
}