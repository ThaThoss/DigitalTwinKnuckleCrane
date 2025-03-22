#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <linux/joystick.h>
#include <iostream>

#include "./../lib/sendReceive.h"
#include "./../lib/commonDataSizes.h"
#include "./myXboxController.h"
#include "./../lib/myLinearMapping.h"

#define PORT "8085"
#define IP "127.0.0.1"

int main(int argc, char* argv[]){

/*
    if(argc<2){
		printf("Please input the ip adress\n");
	       return 1;
	}	       
*/
	int sockfd, check, portnum;
	int16_t angle[4] = {0};
    bool gogo = true;
	portnum = atoi(PORT);
	printf("Attempting to connect...?\n");
    char ip[] = IP;
    std::cout << "ip = " << ip << " end port = " << portnum << std::endl;
    char buffer[255];
    int numCharsToSend = knuckleNBytesToKnu;
    // [r ,th1, th2] + gogo + moveCrane
    unsigned char *dataToSend = (unsigned char*)malloc(numCharsToSend);
    double *rates = (double *)dataToSend;
    int16_t *gogoSignal = (int16_t *)(dataToSend + SZ_DOUBLE*3);
    int16_t *moveCraneSignal = (int16_t *)(dataToSend + SZ_DOUBLE*3 + SZ_INT16);
    *moveCraneSignal = 0;

    const char *device;
    int js;
    struct js_event event;
    struct axis_state axes[4] = {0};
    //size_t axis;

    myLinearMapping radMap(-32768,32767,0.0,10.0);
    myLinearMapping th1Map(-32768,32767,0.0,M_PI/2.0);
    myLinearMapping th2Map(-32768,32767,0.0,M_PI/2.0);


    device = "/dev/input/js0";


    sockfd = connectToServer(portnum, ip); 
	if(sockfd == -1){
		printf("Error in connecting to server");
		return -1;
	}



    read(sockfd,buffer,255);
	
	check = strncmp(buffer,"200",3);
    printf("Server connected, ansver was %s\n",buffer);
	if(check){
		printf("\nServer error with code %s\n",buffer);
		return -1;
	}
    //sleep(2);  // 2,000,000 microseconds (2 sec)
    printf("Sending Bodies");
    int bodies[2] = {4,3};
    if(SendNInts(sockfd,bodies,sizeof(bodies))){
		std::cout << "Error in SendInt32 in getFeedbackFromTwin" << std::endl;
		return 1;	
	}
    printf("Waiting for gogo");
    read(sockfd,buffer,255);
	printf("Server ready, ansver was %s\n",buffer);
	check = strncmp(buffer,"gogo",4);
	if(check){
		printf("\nServer error with code %s\n",buffer);
		return -1;
	}

    while(gogo){

        *gogoSignal = 1;
        printf("Attempting to connect to controller...\n");
        js = open(device, O_RDONLY);
        if (js == -1){
            perror("Could not open joystick");
        }else{printf("Connected");}
        
        /* This loop will exit if the controller is unplugged. */
        while (read_event(js, &event) == 0 && gogo)
        {
            switch (event.type)
            {
                //Read controller values
                case JS_EVENT_BUTTON:
                    printf("Button %u %s\n", event.number, event.value ? "pressed" : "released");
                    if(event.number == 8){
                        printf("Shutoff button pressed, shutting down...\n");
                        gogo = false;
                    }else if(event.number == 1 && event.value){
                        if(*moveCraneSignal){
                            *moveCraneSignal = 0;
                        }else{
                            *moveCraneSignal = 1;
                        }
                    }
                    //Send controller values
                    check = SendNUnsignedChar(sockfd,dataToSend,numCharsToSend);
                      //Print values to screen for control
                    printf("Radius was %f, theta %f and phi %f\n",rates[0],rates[1],rates[2]);
                    printf("gogo was %d, moveCrane %d \n\n",*gogoSignal,*moveCraneSignal);
                   
                    break;

                case JS_EVENT_AXIS:
                    //axis = get_axis_state(&event, axes);
                    get_axis_state(&event, axes);
                    
                    angle[1] = axes[0].x; //radius
                    angle[0] = axes[0].y;// theta or xy plane rotation
                    angle[2] = axes[2].x;// phi, vertical rotation
                    

                    if(angle[0]>-4000&&angle[0]<4000){angle[0]=0;}
                    if(angle[1]>-4000&&angle[1]<4000){angle[1]=0;}
                    if(angle[2]>-4000&&angle[2]<4000){angle[2]=0;}

                    //radius 
                    *(rates + 0) = radMap.map(angle[0] );
                    //th1
                    *(rates + 1) = th1Map.map(angle[1] );
                    //th2
                    *(rates + 2) = th2Map.map(angle[2] );


                    //Send controller values
                    check = SendNUnsignedChar(sockfd,dataToSend,numCharsToSend);
                

                    //Print values to screen for control
                    printf("Radius was %f, theta %f and phi %f\n",rates[0],rates[1],rates[2]);
                    printf("gogo was %d, moveCrane %d \n\n",*gogoSignal,*moveCraneSignal);
                    if(check){
                        printf("Error sending angle\n");
                    }
                break;
                default:
                    /* Ignore init events. */
                    break;
            }
            fflush(stdout);
        }
        sleep(1);
    }//while(gogo)

    //Send final controller values
    *gogoSignal = 0;
    check = SendNUnsignedChar(sockfd,dataToSend,numCharsToSend);
    printf("Shutdown sent\n");
    printf("Radius was %f, theta %f and phi %f\n",rates[0],rates[1],rates[2]);
    printf("gogo was %d, moveCrane %d \n\n",*gogoSignal,*moveCraneSignal);
    close(js);
    close(sockfd);
    return 0;

}

