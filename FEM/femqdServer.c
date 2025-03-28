#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include "Server.h"

#define PORT_8080 "8080"

volatile sig_atomic_t stop = 0;

void handle_sigint(int sig) {
    stop = 1;
    printf("\nSIGINT caught, shutting down gracefully... from femServer\n");
}


int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen, pid, whileCount;
     char buffer[256], argvForExce[50];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     struct sigaction sa;
     sa.sa_handler = handle_sigint;
     sigemptyset(&sa.sa_mask);
     sa.sa_flags = 0; 
     sigaction(SIGINT, &sa, NULL);
            
      
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
         error("ERROR opening socket");
     
     // Allow address reuse (avoids "Address already in use" after crash)
     int opt = 1;
     if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
         error("ERROR setting SO_REUSEADDR");
     }
     
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(PORT_8080);  // Make sure PORT_8080 is defined or a string
     
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
         error("ERROR on binding");
     
     printf("Socket on port %d bound, now listening\n", portno);
     fflush(stdout);
     
     listen(sockfd, 5);
     clilen = sizeof(cli_addr);
     printf("Socket on port %d bound, now listening\n", portno);
     


    while(!stop){
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        
        if (newsockfd < 0) {
            if (errno == EINTR) {
                printf("[!] accept() interrupted by signal.\n");
                break;
            } else {
                perror("accept");
                break;
            }
        }
        
        pid = fork();
        whileCount++;
        if (pid<0){error("error on fork()");}

        if(pid == 0){
        close(sockfd);
        sprintf(argvForExce,"%d",newsockfd);
        char *args[]={"./qd",argvForExce, NULL};

        execv(args[0],args);
        
        exit(0);
        }else close(newsockfd);

    }
    close(sockfd);
    printf("femServer shutting down\n\n");

return 0;
}
