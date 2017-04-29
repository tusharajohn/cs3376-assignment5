/*/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

void dostuff(int); /* function prototype */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     signal(SIGCHILD, SIG_IGN);
     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
   int n;
   char buffer[256];
      
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");
   printf("Here is the message: %s\n",buffer);
   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
}/*

// server_functions.c  was worked on by Carla Macias and Tushara Johnson
//
// Server continuously accepts new UDP and TCP client connections for message transmission,
// 	while handling 'zombie processes' that might occur from a large load. User specifies
// 	port number via command line at execution.
//
// Usage Format: "./server_functions <portno>"
// Sample usage: "./server_functions 51720"
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <errno.h>
#include <iostream>
#include <algorithm>

void dostuff(int); /* function prototype */
socket_type
network_accept_any(socket_type fds[], unsigned int count,
                   struct sockaddr *addr, socklen_t *addrlen);
void error(const char *msg)
{
	perror(msg);
	exit(1);
}
int main(int argc, char *argv[])
{
	int udpfd, sockfd, newsockfd, maxfd, ready, portno, pid, length;
	socklen_t clilen;
	ssize_t n;
	fd_set fdset;
	struct sockaddr_in serv_addr, cli_addr;
	char buf[1024];

	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	// create TCP socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,
			 sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	listen(sockfd,5);
	
	udpfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (udpfd < 0)
		error("Opening socket");
	length = sizeof(serv_addr);
	bzero(&serv_addr,length);

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(atoi(argv[1]));

	if (bind(udpfd,(struct sockaddr *)&serv_addr,length)<0)
		error("binding");

	clilen = sizeof(struct sockaddr_in);
	
	signal(SIGCHLD,SIG_IGN);
	FD_ZERO(&fdset);
	maxfd = std::max(sockfd, udpfd) + 1;
	
	socket_type 	
	network_accept_any(socket_type fds[], unsigned int count,
                   struct sockaddr *addr, socklen_t *addrlen)
{
    fd_set readfds;
    socket_type maxfd, fd;
    unsigned int i;
    int status;

    FD_ZERO(&readfds);
    maxfd = -1;
    for (i = 0; i < count; i++) {
        FD_SET(fds[i], &readfds);
        if (fds[i] > maxfd)
            maxfd = fds[i];
    }
    status = select(maxfd + 1, &readfds, NULL, NULL, NULL);
    if (status < 0)
        return INVALID_SOCKET;
    fd = INVALID_SOCKET;
    for (i = 0; i < count; i++)
        if (FD_ISSET(fds[i], &readfds)) {
            fd = fds[i];
            break;
        }
    if (fd == INVALID_SOCKET)
        return INVALID_SOCKET;
    else
        return accept(fd, addr, addrlen);
}
	/*while (1) {

		FD_SET(sockfd, &fdset);
		FD_SET(udpfd, &fdset);
		if((ready = select(maxfd, &fdset, NULL, NULL, NULL)) < 0){
			if(errno == EINTR)
				continue;
			else
				error("error choosing sock");
		}

		if(FD_ISSET(sockfd, &fdset)){
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd,
							   (struct sockaddr *) &cli_addr, &clilen);
			if (newsockfd < 0)
				error("ERROR on accept");
			pid = fork();
			if (pid < 0)
				error("ERROR on fork");
			if (pid == 0)  {
				close(sockfd);
				dostuff(newsockfd);
				exit(0);
			}
			else close(newsockfd);
		}

		if(FD_ISSET(udpfd, &fdset)){
			clilen = sizeof(struct sockaddr_in);
			n = recvfrom(udpfd,buf,1024,0,(struct sockaddr *)&cli_addr,&clilen);
			if (n < 0) error("recvfrom");
			write(1,"Received a datagram: ",21);
			write(1,buf,n);
			n = sendto(udpfd,"Got your message\n",17,
					   0,(struct sockaddr *)&cli_addr,clilen);
			if (n  < 0)
				error("sendto");
		}
		
	} */ end of while */*/
	close(sockfd);
	return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
	int n;
	char buffer[256];
	bzero(buffer,256);

	n = read(sock,buffer,255);

	if (n < 0) error("ERROR reading from socket");
	printf("Here is the message: %s\n",buffer);

	n = write(sock,"I got your message",18);
	if (n < 0) error("ERROR writing to socket");
}


/*******************iaddedthis*****************
socket_type
network_accept_any(socket_type fds[], unsigned int count,
                   struct sockaddr *addr, socklen_t *addrlen)
{
    fd_set readfds;
    socket_type maxfd, fd;
    unsigned int i;
    int status;

    FD_ZERO(&readfds);
    maxfd = -1;
    for (i = 0; i < count; i++) {
        FD_SET(fds[i], &readfds);
        if (fds[i] > maxfd)
            maxfd = fds[i];
    }
    status = select(maxfd + 1, &readfds, NULL, NULL, NULL);
    if (status < 0)
        return INVALID_SOCKET;
    fd = INVALID_SOCKET;
    for (i = 0; i < count; i++)
        if (FD_ISSET(fds[i], &readfds)) {
            fd = fds[i];
            break;
        }
    if (fd == INVALID_SOCKET)
        return INVALID_SOCKET;
    else
        return accept(fd, addr, addrlen);
}
