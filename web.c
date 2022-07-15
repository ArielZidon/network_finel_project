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
// #define IP_ADDRESS "13.226.2.96" 
#define PORT 80

char* get_ip_by_hostname(char* hostname) {
  struct addrinfo* res;
  // char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;

  // takes the host name which was supplied from the command line and stores it
  // creates the addrinfo and check if it has been created successfully
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }
  // stores the host name and it's corresponding IP address
  saddr = (struct sockaddr_in*)res->ai_addr;
  hostaddr = inet_ntoa(saddr->sin_addr);
  printf("%s", hostaddr);
  return hostaddr;
}

int recognizing_url_type(char* url) {
  /* recognizing which url type is it 
  if <protocol>://<hostname>/<path> result is 1, 
  if <protocol>://<hostname>:<port>/<path> result is 2*/
  int i = 0, how_many_colons = 0;
  while (url[i] != '\0') {
    if (url[i] == ':') {
      how_many_colons++;
    }
    i++;
  }
  return how_many_colons;
}


int main(int argc, char* argv[])
{ 
  // if (argc != 2) { // if an host name is not supplied from command line
  //   perror("no hostname supplied\n");
  //   exit(1);
  // }
  
  int sock, port; 
  struct sockaddr_in cli_name;  
  char* url = argv[1];
  char hostname[200];
  char path[100];
  char sendline[4096];
  char recvline[4096];
  int len;
  char* ip_address;

  // parsing url
  int how_many_colons_in_url = recognizing_url_type(url);
  if(how_many_colons_in_url == 1) {
    sscanf(url, "http://%99[^/]", hostname);
    sscanf(argv[2], "%d", &port);
  }
  else {
    sscanf(url, "http://%99[^:]:%99d/[^\n]", hostname, &port);
    
  }
  ip_address = get_ip_by_hostname(hostname);

  printf("\nClient is alive and establishing socket connection.\n");
  
  //creating the socket, domain is IPv4 Internet protocols, type is SOCK_STREAM and protocol is 0
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) // if an error was raised when creating the socket
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }
  
  bzero(&cli_name, sizeof(cli_name)); // erasing the memory from cli_name 
  //init the attributes of cli_name
  cli_name.sin_family = AF_INET; 
  cli_name.sin_addr.s_addr = inet_addr(ip_address); // IPv4 numbers-and-dots notation into binary data in network byte order
  cli_name.sin_port = htons(port);

  //if error was raised when trying to connect to the address, explain what was the problem and close the program, otherwise, connect the socket to the address
  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0) { 
    perror ("Error establishing communications");
    close(sock);
    exit(1);
  }

  // sending GET request to the server
  len = sprintf(sendline, "GET %s HTTP/1.0\r\n\r\n", url);
  send(sock, sendline, len, 0);

  // receiving the response from the server and writing it to the terminal
  memset(recvline, 0, 4096);
  while ((len = read(sock, recvline, 4095)) > 0) {
    printf("%s", recvline);
    memset(recvline, 0, 4096);
  }
  if(len < 0){
    close(sock);
    exit(1);
  }

  printf("Exiting now.\n");

  close(sock); 
  exit(0); 
}