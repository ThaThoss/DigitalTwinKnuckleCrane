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
#ifndef SEND_RECEIVE
#define SEND_RECEIVE

// Send an error message
void myError(char *msg);

//Conect to server specified by IP and portnumber
int connectToServer(int portnum, char *hostIP);

// Receive an 32 bit int.
int ReceiveInt32(int sockfd, int *returnValue);

//Recieve number of integers specified in size
int ReceiveNInts(int sockfd, int *Integers, int size);

//Send and 32 bit int
int SendInt32(int sockfd, int intToSend);

//Send an 16 bit int
int SendInt16(int sockfd, int intToSend);

//Send amount of 16 bit ints as specified by number of ints
int SendSeveralInt16(int sockfd, int16_t *intToSend,int numbOfInts);

//Recieve amount of 16 bit ints as specified by number of ints
int ReceiveNInt16(int sockfd, int16_t *intsToRecieve, int nInts);

int SendNIntsNoBlock(int sockfd, int *IntsToSend, int size);

//Send a specified amount of unsigned chars
int SendNUnsignedChar(int sockfd, unsigned char* charToSend, int numbOfChars );

int ReceiveNUnsignedChars(int serverSocket,  unsigned char* charBuffer, int numChars);

int ReceiveNChars(int sockfd, char* charsToReceive, int numChars);

int SendNChar(int sockfd, char* charToSend, int numbOfChars );

int SendNInts(int sockfd, int *IntsToSend, int size);

int ReceiveNDoubles(int sockfd, double *doublesToGet, int size);

int ReceiveNFloats(int sockfd, float *floatsToGet, int numFloats);

int SendNDoubles(int sockfd, double *doublesToSend, int size);

int SendNFloats(int sockfd, float *FloatsToSend, int numFloats);

int SendNDoublesNoBlock(int sockfd, double *doublesToSend, int size);

int ReceiveNDoublesNoWrite(int sockfd, double *doublesToGet, int size);

void argvChecker(int argc, char *argv[], char *ip, char *name);

#endif
