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

void error(const char *msg){
    perror(msg);
    exit(1);
}
volatile sig_atomic_t gogo = 1;
void handle_sigint(int sig) {
    gogo = 0;
    printf("\nSIGINT caught in femMainServer\n");
}

int main(int argc, char *argv[])
{
    printf("Socket on port bound, now listening\n");  
    fflush(stdout);  // Ensure immediate print

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; 
    sigaction(SIGINT, &sa, NULL);
    
    int sockfd, newsockfd, portno, pid, whileCount = 0;
    char buffer[256], argvForExce[50];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    // Allow address reuse (avoids "Address already in use" after crash)
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        error("ERROR setting SO_REUSEADDR");
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));  // Use memset instead of bzero
    portno = 8085;  // Direct assignment
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    printf("Socket on port %d bound, now listening\n", portno);  
    fflush(stdout);

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (gogo) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");

        pid = fork();
        if (pid < 0) 
            error("ERROR on fork");

        if (pid == 0) { // Child process
            close(sockfd);
            sprintf(argvForExce, "%d", newsockfd);
            char *args[] = {"./server.out", argvForExce, NULL};

            execv(args[0], args);
            
            // If execv fails
            error("ERROR in execv");
            exit(1);
        } else {
            close(newsockfd); // Parent closes the new socket
        }
        whileCount++;
    }
    close(sockfd);
    printf("Main Server shutting down gracefully\n");

    return 0;
}
