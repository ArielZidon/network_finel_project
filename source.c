#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int socket_fd; 
  struct sockaddr_in  dest; 
  struct hostent *hostptr;
  struct {int number;} msgbuf;
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
  bzero((char *) &dest, sizeof(dest));
  hostptr = gethostbyname(argv[1]);  
  dest.sin_family = (short) AF_INET; 
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
  dest.sin_port = htons((u_short)0x3333);
  int msg = 0;
  for(;;) {
    printf("send %d\n", msg);
    fflush(stdout);
    msg++;
    msgbuf.number = msg; 
    sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
                  sizeof(dest));
    printf("sleeping for 1 second\n");
    fflush(stdout);
    sleep(1);
  }
  return 0;
}