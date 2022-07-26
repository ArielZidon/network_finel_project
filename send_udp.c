#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int socket_fd; //init a varible to be the file descriptor number
  struct sockaddr_in  dest; //init a struct sockaddr_in, Structure describing an Internet socket address.
  struct hostent *hostptr; //Description of data base entry for a single host.
  struct { char head; u_long body; char tail; } msgbuf; // the struct msgbuf defining the body of the messege.

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // creat the socket and return int if the call are secsses.(AF_INIT(domain) - ipv4 internt protocol ,SOCK_DGRAM - Types of sockets.
  // , protocol (we put 0 if we want to serve a socket))
  bzero((char *) &dest, sizeof(dest)); /* They say you must do this, you can do this with memset (put "0") allso */
  hostptr = gethostbyname(argv[1]); /* The  gethostbyname() function returns a structure of type hostent for the given host name.  
  Here name is either a hostname or an IPv4 address in standard dot notation */
  dest.sin_family = (short) AF_INET; //the address family for the transport address is set to AF_INET
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);//we copy the hostptr->h_addr (host addres) into dest.sin_addr(dest addres)
  //with length of the hostptr..
  dest.sin_port = htons((u_short)0x3333);//The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.

  msgbuf.head = '<'; // build the open char of the messege
  msgbuf.body = htonl(getpid()); //getpid() returns the process ID (PID) of the calling process. /* IMPORTANT! take the procces ID and converts it into host byte order*/
  msgbuf.tail = '>'; //build the finel char of the messege

  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
                  sizeof(dest)); //The system calls sendto() are used to transmit a message to another socket.
  return 0;
}
