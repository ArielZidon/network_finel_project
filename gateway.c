#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    float rand = 0;
    int socket_recv, socket_send,recv_f, rsize_f;
    struct sockaddr_in s_came, source, dest;
    struct hostent *hostptr = gethostbyname(argv[1]);
    struct {char head; u_long  body; char tail;} msg, msgbuf;
    
    // Creates the "IN" and "OUT" sockets
    socket_recv = socket(AF_INET, SOCK_DGRAM, 0);
    socket_send = socket(AF_INET, SOCK_DGRAM, 0);
    
    srandom(time(0)); 
    bzero((char *) &s_came, sizeof(s_came)); //Resetting the socket "IN"
    s_came.sin_family = (short)AF_INET; 
    s_came.sin_addr.s_addr = htonl(INADDR_ANY); 
    s_came.sin_port = atoi(argv[2]);
    bind(socket_recv, (struct sockaddr *)&s_came, sizeof(s_came));

    bzero((char *) &dest, sizeof(dest));//Resetting the socket "OUT"
    dest.sin_family = (short) AF_INET;
    bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length); 
    dest.sin_port = atoi(argv[2]) + 1;

    printf("Recv in port %d \nSend in port %d\n\n",s_came.sin_port,dest.sin_port);
    
    while(1) {
        // receive source
        rsize_f = sizeof(source);
        recv_f = recvfrom(socket_recv,&msg,sizeof(msg),0,(struct sockaddr *)&source,&rsize_f);
        printf("Received data gram from source\n");
        fflush(stdout);

        // get a random number for rand
        rand = ((float)random())/((float)RAND_MAX);
        printf("The random number is: %f\n", rand);
        fflush(stdout);

        // send data gram or block it, and wait for a new one.
        if (rand > 0.5) {
            printf("\t%f > 0.5 ,the data gram has been sent!\n\n",rand);
            fflush(stdout);
            sendto(socket_send,&msg,sizeof(msg),0,(struct sockaddr *)&dest,
                  sizeof(dest));
        }
        else {
            printf("\t%f < 0.5 ,the data gram has been blocked!\n\n",rand);
        }
    }
}