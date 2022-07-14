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

int main(int argc, char *argv[]) {
    int socket_recv, cc, fsize; 
    struct sockaddr_in  s_in, from;
    struct {int number;} msg;

    // creates socket
    socket_recv = socket (AF_INET, SOCK_DGRAM, 0);
    bzero((char *) &s_in, sizeof(s_in));
    s_in.sin_family = (short)AF_INET;
    s_in.sin_addr.s_addr = htonl(INADDR_ANY);
    s_in.sin_port = htons((u_short)0x3334);

    //read datagram
    bind(socket_recv, (struct sockaddr *)&s_in, sizeof(s_in));

    for (;;) {
        fsize = sizeof(from);
        cc = recvfrom(socket_recv,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);
        printsin( &from, "recv_udp: ", "Packet from:");
        printf("Got Data: %d\n", msg.number);
        fflush(stdout);
    }
}

// 3 prog that work like recv udp and print the datagram