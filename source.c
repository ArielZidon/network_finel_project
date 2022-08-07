#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int socket_fd; 
  int massege = 0;
  struct sockaddr_in  dest; 
  struct hostent *hostptr;
  struct {int num;} msg_bufffer;
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
  bzero((char *) &dest, sizeof(dest));
  hostptr = gethostbyname(argv[1]);  
  dest.sin_family = (short) AF_INET; 
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
  dest.sin_port = atoi(argv[2]);
  printf("Send in port %d\n\n",dest.sin_port);
  while(1) {
    printf("send: %d\n", massege);
    fflush(stdout);
    massege++;
    msg_bufffer.num = massege; 
    sendto(socket_fd,&msg_bufffer,sizeof(msg_bufffer),0,(struct sockaddr *)&dest,
                  sizeof(dest));
    printf("\tsleep 1 second...\n");
    fflush(stdout);
    sleep(1);
  }
  return 0;
}