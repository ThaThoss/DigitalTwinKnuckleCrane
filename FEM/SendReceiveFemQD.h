#ifndef SeReQD
#define SeReQD

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>


void error(char *msg){
	error(msg);
	exit(1);
}


int connectToServer(int portnum, char *hostIP){

	int sockfd, n, check;

	struct sockaddr_in serv_addr;
	struct hostent *server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0){
		error("ERROR opening socket");
	}
/*	
	server = getaddrinfo(hostIP); 
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server -> ai_addr, (char *)&serv_addr.sin_addr.s_addr, server->ai_addrlen);
	serv_addr.sin_port = htons(portnum);
	*/

	/*This gets a server by name, might be too fancy for this program */	
/*	server = gethostbyname(hostName);
	if(server == NULL){
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	bcopy((char *)server -> h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	
*/	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	check = inet_pton(AF_INET,hostIP,&serv_addr.sin_addr);
	if(check<=0){
		printf("\nInvalid IP adress/ Address not supported \n");
		return -1;
	}
	
	check = connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(check<0){
		error("ERROR connecting to server");
		return -1;
		}
	return sockfd;
}








int ReceiveInt32(int sockfd, int *returnValue){

	int32_t ret;
	char *data = (char*)&ret;
	int left = sizeof(ret);
	int rc;
	int n=0;
	//char ready[] = "ready";
//	n = write(sockfd, ready, strlen(ready));
	do{
		rc = read(sockfd, data, left);
		if(rc<0){
			if((errno == EAGAIN) || (errno == EWOULDBLOCK)){
					error("Something wrong with read\n");
					}
					else if(errno != EINTR){
					return -1;
					}
			}
		else{
			data += rc;
			left -= rc;
		}
	}while(left>0);
	*returnValue = (int)ntohl(ret);
	//printf("Received the int %d\n",*returnValue);
	return 0;
}

int ReceiveNInts(int sockfd, int *Integers, int size){

	char *data = (char*)Integers;
	int left = size;
	int rc;
	int n=0;
	char ready[] = "ready";
	int recieved = 0;
	n = write(sockfd, ready, 5);
	
	do{
		rc = read(sockfd,data, left);
		if(rc<0){
			error("Error in ReceiveIntArray\n");
		}else if(rc==0){
			printf("prematurely broke reciveloop\n");
			return 1;
			break;
		}else{
			data += rc;
			left -= rc;
			recieved += rc;
			printf("got %d bytes",rc);
		}
	}while(left>0);
	printf("recieveNInts got %d bytes\n",recieved);
	return 0;
}

int SendInt32(int sockfd, int intToSend){

	int32_t convIntToSend = htonl(intToSend);
	char *data = (char*)&convIntToSend;
	int left = sizeof(convIntToSend);
	int rc;
	int n =0;
	char buffer[6];

	int check = 0;
//	n = read(sockfd, buffer, 5);
//	check = strncmp(buffer,"ready",5);
	if(check == 0){	
	do{
		rc = write(sockfd, data, left);
		if(rc<0){
			if ((errno == EAGAIN) || (errno == EWOULDBLOCK)){
				printf("something wrong with SendInt32\n");
			}
			else if(errno != EINTR){
				return -1;
			}
		}
		else{
			data += rc;
			left -= rc;
		}
	}while (left > 0);
	//printf("the int %d sent\n",intToSend );
	}else{
		printf("Did not recieve ready in SendInt32\n");
		return 1;
	}
	return 0;
}
int SendInt16(int sockfd, int intToSend){
	if(intToSend<-32768 || intToSend> 32768){
		printf("Error in sendint16, int to big or small");
		return -1;
	}
	int16_t int16tosend = intToSend;	
	char* data = (char*)&int16tosend;
	int left = sizeof(int16tosend);
	int rc;
	do{
		rc = write(sockfd, data, left);
		if(rc<0){
			printf("something wrong with SendInt16");
			return -1;
		}else{
			data += rc;
			left -= rc;
		}
	}while(left > 0);
	return 0;
}


int SendSeveralInt16(int sockfd, int* intToSend,int numbOfInts){
	

	int16_t intsToSend[numbOfInts];
	
      	for(int i=0; i<numbOfInts; i++){
	if(*(intToSend+i)<-32768 || *(intToSend+i)> 32768){
		printf("Error in sendint16, int to big or small");
		return -1;
	}
	
      
		intsToSend[i] = (short)*(intToSend+i);
	}	
		
	char* data = (char*)intsToSend;
	int left = numbOfInts*sizeof(int16_t);
	int rc;
	do{
		rc = write(sockfd, data, left);
		if(rc<0){
			printf("something wrong with SendSeveralInt16");
			return -1;
		}else{
			data += rc;
			left -= rc;
		}
	}while(left > 0);
	return 0;
}

int ReceiveNIntsNoBlock(int sockfd, int *Integers, int size){

	char *data = (char*)Integers;
	int left = size;
	int rc;
	int n=0;
	int recieved = 0;

	do{
		rc = read(sockfd,data, left);
		if(rc<0){
			error("Error in ReceiveIntArray\n");
		}else if(rc==0){
			printf("prematurely broke reciveloop\n");
			break;
		}else{
			data += rc;
			left -= rc;
			recieved += rc;
		}
	}while(left>0);
	//printf("recieveNInts got %d bytes\n",recieved);
	return 0;
}
int SendNIntsNoBlock(int sockfd, int *IntsToSend, int size){

	char *data = (char*)IntsToSend;
	int left = size;
	int rc;
	int sent = 0;
	do{
		rc = write(sockfd, data, left);
		if(rc<0){error("Error in SendNIntsNoBlock\n");
		}else{
			data += rc;
			left -= rc;
			sent += rc;
		}
	}while(left >0);

	//printf("Sent %d integers\n",sent);
	return 0;

}


int SendNInts(int sockfd, int *IntsToSend, int size){

	char *data = (char*)IntsToSend;
	int left = size;
	int rc;
	int sent = 0;
	int check = 0;
	char buffer[6];
	rc = read(sockfd, buffer,5);
	check = strncmp(buffer,"ready",5);
	
	if(check ==0){
	do{
		rc = write(sockfd, data, left);
		if(rc<0){error("Error in SendInts\n");
		}else{
			data += rc;
			left -= rc;
			sent += rc;
		}
	}while(left >0);
	}else{
		error("SendNInts did not recieve ready\n");
	}
	//printf("Sent %d bytes of integers\n",sent);
	return 0;

}

int ReceiveNDoubles(int sockfd, double *doublesToGet, int size){

	char *data = (char*)doublesToGet;
	int left = size;
	int rc, n;
	int sent = 0;
	char ready[] = "ready";
	
	n = write(sockfd,ready,5);
	do{
		rc = read(sockfd,data,left);
		if(rc<0){
			error("Error in receiveNDoubles\n");
		}else{
			data += rc;
			left -= rc;
			sent += rc;
		}
	}while(left>0);
	//printf("recieveNDoubles got %d bytes\n",sent);
	return 0;
}

int SendNDoubles(int sockfd, double *doublesToSend, int size){

	char *data = (char*)doublesToSend;
	int left = size;
	int rc, n;
	int sent = 0;
	int check = 0;
	char buffer[6];

	n = read(sockfd, buffer,5);
	check = strncmp(buffer,"ready",5);
	if(check == 0){
		do{
			rc = write(sockfd,data,left);
			if(rc<0){
				error("Error in sendNDoubles\n");
			}else{
				data += rc;
				left -= rc;
				sent += rc;
			}
		}while(left>0);
	}else{
		printf("sendNDoubles did not receive ready\n");
	}
	//printf("sent %d bytes of doubles\n",sent);
	return 0;
}


int SendNDoublesNoBlock(int sockfd, double *doublesToSend, int size){

	char *data = (char*)doublesToSend;
	int left = size;
	int rc;
	int sent = 0;

	do{
		rc = write(sockfd,data,left);
		if(rc<0){
			error("ERROR in SendNDoubles");
		}else{
			data += rc;
			left -= rc;
			sent += rc;
		}
	}while(left>0);

	//printf("sent %d bytes of doubles\n",sent);
	return 0;
}

int ReceiveNDoublesNoWrite(int sockfd, double *doublesToGet, int size){

	char *data = (char*)doublesToGet;
	int left = size;
	int rc;
	int sent = 0;
	
	do{
		rc = read(sockfd,data,left);
		if(rc<0){
			error("ERROR in ReceiveNDoubles");
		}else{
			data += rc;
			left -= rc;
			sent += rc;
		}
	}while(left>0);
	//printf("recieveNDoubles got %d bytes\n",sent);
	return 0;
}


int SendNChar(int sockfd, char* charToSend, int numbOfChars ){

	int left = numbOfChars*sizeof(char);
	int rc;
	do{
		rc = write(sockfd, charToSend, left);
		if(rc<0){
			printf("something wrong with SendNUnsignedChar");
			return -1;
		}else{
			charToSend += rc;
			left -= rc;
		}
	}while(left > 0);
	return 0;
}

int ReceiveNChars(int sockfd, char* dataToReceive, int numChars){

	int left = sizeof(char)*numChars;
	int rc;
	char* data = dataToReceive;
	do{
		rc = read(sockfd, data, left);
		if(rc<0){
			if((errno == EAGAIN) || (errno == EWOULDBLOCK)){
				
				error("Something wrong with read");
					}
			else if(errno != EINTR){
				return 1;
			}
		}else{
			data += rc;
			left -= rc;
		}
	}while(left>0);
	return 0;
}


void argvChecker(int argc, char *argv[], char *ip, char *name){

	int ipFlag = 0;
	int nameFlag = 0;
if(argc>1){
	for(int i=1; i<argc;i+=2){
printf("filename = %s\n",argv[i]);
		if(strcmp(argv[i],"-ip")==0){
		
			//ip=*(argv[i+1]);
			strcpy(ip,argv[i+1]);

		}
		else if(strcmp(argv[i],"-name")==0){

			//name = *(argv+(i+1));
			memcpy(name,(argv[i+1]),strlen((argv[i+1])));
			printf("filename = %s\n",name);
			printf("filename = %s\n",argv[i+1]);
			nameFlag = 1;
			
		}
		else if(strcmp(argv[i],"-help")==0){

			printf("Usage: \n     -ip ip4_Address {ip4 adress of the server, if no address is given, 127.0.0.1 will be used}\n     -name nameOfFile {File name of the fem data file, if no name is given dog3 is used}\n\n");
		exit(0);
		}

	}
}
if(ipFlag == 0){
	char dum[] = "127.0.0.1";	
	strcpy(ip,dum);
}

if(nameFlag == 0){
	char dummer[] = "dog3";
	strcpy(name,dummer);
}


}

#endif