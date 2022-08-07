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
#include <stdlib.h>
//Taking from the recv_udp, its make the work of print the datagram's details.. 
void printsin(struct sockaddr_in *s, char *str1, char *str2) 
{
  printf("%s\n", str1);
  printf("%s ", str2);
  char str3[30];
  strcpy(str3, (char*)inet_ntoa((struct in_addr)s->sin_addr));
  printf("ip= %s, port= %d", str3, s->sin_port);
  printf("\n");
}

int main(int argc, char *argv[]) {
    int socket_recv, recv_f, size; 
    struct sockaddr_in  socket_in, source;
    struct {int num;} message;
    // Creates the socket
    socket_recv = socket (AF_INET, SOCK_DGRAM, 0);
    bzero((char *) &socket_in, sizeof(socket_in));
    socket_in.sin_family = (short)AF_INET;
    socket_in.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_in.sin_port = atoi(argv[1]) + 1;
    
    bind(socket_recv, (struct sockaddr *)&socket_in, sizeof(socket_in));
    //Read the datagrams
    while(1) {
        size = sizeof(source);
        recv_f = recvfrom(socket_recv,&message,sizeof(message),0,(struct sockaddr *)&source,&size);
        printsin( &socket_in, "Recv data: ", "Packet from:");
        printf("\tGot Data: %d\n\n", message.num);
        fflush(stdout);
    }
}

//sink,gatewat,source are working good.