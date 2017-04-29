// Client.c code was worked on by Stephen Ballenger, Dawood Nadurath, and Auveed Saki
// has declarations used in input/output
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// has definitions for data types used in system calls.
#include <sys/types.h>
// is a header file. has definitions for structures used for sockets 
#include <sys/socket.h>
// header file. has constants and structures for inet domain addresses
#include <netinet/in.h>
#include <netdb.h> 

//error function. will be called on when the system call fails. message will be
// displayed on stderr. program will then abort.
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    //sockfd: file descriptor. stores values returned by socket sys call
    //portno: stores port # on which server accepts connections
    //n: return value for read() and write() calls
    int sockfd, portno, n;
    //serv_addr: will contain the address of the server 
    // to which we are trying to connect to
    struct sockaddr_in serv_addr;
    //server: a pointer to a struct of type hostent
    struct hostent *server;
    
    // reads chars for the socket connection into this buffer
    char buffer[256];
    // print an error if the user doesn't pass in the port no
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    // the port no on which the server will listen for connections is passed
    // as an argument, and this is passed to atoi()
    // a string of digits is convereted into an integer
    portno = atoi(argv[2]);
    // new socket is created
    // first argument: address domain of the socket, second arg: type of socket
    // third arg: protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // display an error message and exit
    if (sockfd < 0) 
        error("ERROR opening socket");
    // argv[1] contains the name of a host on the inet
    server = gethostbyname(argv[1]);
    // the system can't locate a host name if the structure is NULL
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    // set fields in serv_addr
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // use bcopy because the field server->h is a char string
    // copies length bytes from s1 to s2
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    // the client calls the connect function so that a connection to the
    // server can be established
    // first arg: socket file descriptor
    // seconnd arg: address of host which it wants to connect
    // third arg: size of address
    // 0 if successful, -1 if fails
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    // prompt user to enter a message
    printf("Please enter the message: ");
    bzero(buffer,256);
    // read message from stdin  
    fgets(buffer,255,stdin);
    // write message to the socket
    n = write(sockfd,buffer,strlen(buffer));
    // if there's an error writing to the socket
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    // read reply from socket
    n = read(sockfd,buffer,255);
    // if there's an error reading from socket
    if (n < 0) 
         error("ERROR reading from socket");
    // print reply on the screen
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}
