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
    int socket_recv, socket_send;
    int cc, fsize;
    struct sockaddr_in s_in, from, dest;
    float random_number;
    struct hostent *hostptr = gethostbyname(argv[1]);
    struct {char head; u_long  body; char tail;} msg, msgbuf;
    
    // creates sockets
    socket_recv = socket(AF_INET, SOCK_DGRAM, 0);
    socket_send = socket(AF_INET, SOCK_DGRAM, 0);
    
    srandom(time(0)); 

    bzero((char *) &s_in, sizeof(s_in));  
    s_in.sin_family = (short)AF_INET; 
    s_in.sin_addr.s_addr = htonl(INADDR_ANY); 
    s_in.sin_port = htons((u_short)0x3333);
    bind(socket_recv, (struct sockaddr *)&s_in, sizeof(s_in));

    bzero((char *) &dest, sizeof(dest)); 
    dest.sin_family = (short) AF_INET;
    bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length); 
    dest.sin_port = htons((u_short)0x3334);
    
    for(;;) {
        // receive from socket_recv
        fsize = sizeof(from);
        cc = recvfrom(socket_recv,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);
        printf("received data gram\n");
        fflush(stdout);

        // sample a random number
        random_number = ((float)random())/((float)RAND_MAX);
        printf("the number is: %f\n", random_number);
        fflush(stdout);

        // send data gram or discard it and wait for another
        if (random_number > 0.5) {
            printf("numer is greater than 0.5, send data gram\n");
            fflush(stdout);
            sendto(socket_send,&msg,sizeof(msg),0,(struct sockaddr *)&dest,
                  sizeof(dest));
        }
        else {
            printf("Number is smaller than 0.5, discarded data gram\n");
        }
    }
}