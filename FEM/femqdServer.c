#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "Server.h"

#define PORT_8080 "8080"


int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen, pid, whileCount;
     char buffer[256], argvForExce[50];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     
      
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(PORT_8080);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     printf("Socket on port %d bound, now listening",portno);
     fflush(stdout);          
     listen(sockfd,5);
     clilen = sizeof(cli_addr);


    while(1){
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        
        if (newsockfd < 0) 
            error("ERROR on accept");
        
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

return 0;
}
