#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h> 


void printsin(struct sockaddr_in *s, char *str1, char *str2) {
  printf("%s:\n", str1);
  printf("%s: ", str2);
  char str3[30];
  strcpy(str3, (char*)inet_ntoa((struct in_addr)s->sin_addr));
  printf("ip= %s, port= %d", str3, s->sin_port);
  printf("\n");
}


int main(int argc, char *argv[])
{
  int socket_fd, cc, fsize; 
  /*
  socket_fd - init a varible to be the file descriptor number
  cc - the return value from the recv call
  fsize - the size of the sockaddr_in "from"
   */

  struct sockaddr_in  s_in, from; //init a structs sockaddr_in, Structure describing an Internet socket address.
  struct { char head; u_long  body; char tail;} msg; //defines the structure of the message that will be recived

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); /* creat the socket and return int if the call are secsses.(AF_INIT(domain) - ipv4 internt protocol ,SOCK_DGRAM - Types of sockets.
  , protocol (we put 0 if we want to serve a socket))*/

  bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this, you can do this with memset (put "0") allso*/

  s_in.sin_family = (short)AF_INET;//the address family for the transport address is set to AF_INET
  s_in.sin_addr.s_addr = htonl(INADDR_ANY); /* WILDCARD get the IPV4 addres,converts it to host byte order */
  s_in.sin_port = htons((u_short)0x3333);// converts the host adress to network addres

  printsin( &s_in, "RECV_UDP", "Local socket is:"); 
  fflush(stdout); //discards any buffered data that  has been fetched from the underlying file, but has not been consumed by the application.

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));/*The actual structure passed for the addr argument will depend on the address family. 
  The only purpose of this structure is to cast the structure pointer passed in addr in order to avoid compiler warnings.*/

  for(;;) {
    fsize = sizeof(from);//defin the size of massege from the sockaddr_in from, now we cant sent it as a parameter to recvfrom()
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);//The recvfrom() call are used to receive messages from a socket.
    printsin( &from, "recv_udp: ", "Packet from:");
    printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail); 
    fflush(stdout);//discards any buffered data that  has been fetched from the underlying file, but has not been consumed by the application.
  }
  
  return 0;
}

