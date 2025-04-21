#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <linux/joystick.h>
#include <iostream>
#include <filesystem>
#include <vector>
#include <regex>
#include <string>
#include <algorithm>
#include <signal.h>

#include "../lib/femClient.h"

#include "./../lib/sendReceive.h"
#include "./../lib/commonDataSizes.h"
#include "./myXboxController.h"
#include "./../lib/myLinearMapping.h"
#include "./../lib/rbdClient.h"

#define PORT "8085"
#define IP "127.0.0.1"

volatile sig_atomic_t shutDownSignal = 1;
void handle_sigint(int sig) {
    shutDownSignal = 0;
    printf("\nSIGINT caught, shutting down gracefully... from clientController\n");
}

int main(int argc, char* argv[]){

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; 
    sigaction(SIGINT, &sa, NULL);

    
    std::string path = "../meshFiles/";



    /*-----------------------------------Reading FEM files---------------------------*/
    int numFEMfiles = 0;
    std::string filenameRegexFEM = R"(femMesh(\d+)_([A-Za-z0-9_]+)\.dat)";
    

    std::vector<FEMFILE> filesFEM = findFemFiles(path, filenameRegexFEM);
    numFEMfiles = filesFEM.size();

    FEMDATATOSEND initialDataToSend[numFEMfiles];
    int counter = 0;
    for(const auto& file : filesFEM){
        std::cout << "Reading mesh from: " << file.filename << std::endl;
        femReadMesh(&initialDataToSend[counter], file.filename.c_str());
        counter++;

    }
    if(numFEMfiles==0){
        std::cout << "Something wrong with file readers, exiting"<<std::endl;
        return 1;
    }
    std::cout << "Done reading files ,num fem files was" <<numFEMfiles<< std::endl;
    /*-------------------------------------------------------------------------------------------------------*/



    /*-----------------------------------Reading RBD Files---------------------------*/
    std::string filenameRegexRBD = R"(^RBD.*\.dat$)";
    std::vector<std::string> filesRBD = findRBDFiles(path, filenameRegexRBD);
    if (filesRBD.empty()) {
        std::cerr << "No RBD*.dat files found.\n";
        return 1;
    }
    std::string selectedFile;

    if (filesRBD.size() == 1) {
        selectedFile = filesRBD[0];
        std::cout << "Only one file found: " << selectedFile << "\n";
    } else {
        std::cout << "Multiple RBD files found:\n";
        for (std::size_t i = 0; i < filesRBD.size(); ++i) {
            std::cout << " [" << i << "] " << filesRBD[i] << "\n";
        }

        int choice = -1;
        while (true) {
            std::cout << "Enter the index of the file to use: ";
            std::cin >> choice;

            if (std::cin.fail() || choice < 0 || choice >= static_cast<int>(filesRBD.size())) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Try again.\n";
            } else {
                selectedFile = filesRBD[choice];
                break;
            }
        }
    }
    std::cout << "RBD File " << selectedFile<< " selected" << std::endl;
    RBDDATATOSEND RBDdataToSend;
    readRBDdata(&RBDdataToSend,  selectedFile.c_str());
    std::cout << "done reading RBDdata"<< std::endl;


    /*-----------------------------------------------------------------------------------------------------*/

	int sockfd, check, portnum;
	int16_t angle[4] = {0};
    bool gogo = true;
	portnum = atoi(PORT);
	
    char ip[] = IP;
    std::cout << "ip = " << ip << " end port = " << portnum << std::endl;
    char buffer[255];
    int numCharsToSend = knuckleNBytesToKnu;
    // [r ,th1, th2] + gogo + moveCrane
    unsigned char *dataToSend = (unsigned char*)calloc(numCharsToSend,sizeof(char));
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



    printf("Attempting to connect.\n");
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
    printf("Sending Bodies\n");fflush(stdout);
    int bodies[2] = {RBDdataToSend.nBodies,numFEMfiles};
    if(SendNInts(sockfd,bodies,sizeof(int)*2)){
		std::cout << "Error in SendNInts for bodies in clientControll" << std::endl;
		return 1;	
	}

    //Send axis of rotaions
    SendNInts(sockfd,(RBDdataToSend.axisOfRotations),RBDdataToSend.nBodies*sizeof(int));
  

    int headersToSend[numFEMfiles*8] = {0};
    for(int i=0;i<numFEMfiles; i++){
        headersToSend[i*8 + 0] = initialDataToSend[i].numEl;
        headersToSend[i*8 + 1] = initialDataToSend[i].numNodPnt;
        headersToSend[i*8 + 2] = initialDataToSend[i].numMaterial;
        headersToSend[i*8 + 3] = initialDataToSend[i].PlaneStressFlag;
        headersToSend[i*8 + 4] = initialDataToSend[i].gravity_Flag;
        headersToSend[i*8 + 5] = initialDataToSend[i].numFixx;
        headersToSend[i*8 + 6] = initialDataToSend[i].numFixy;
        headersToSend[i*8 + 7] = initialDataToSend[i].numForce;
        printf("Body %d: numEl %d, numNod %d, mat: %d, flag: %d, grav: %d, fixX %d, fixY %d, and nForce %d\n",i,initialDataToSend[i].numEl,initialDataToSend[i].numNodPnt, initialDataToSend[i].numMaterial, initialDataToSend[i].PlaneStressFlag, initialDataToSend[i].gravity_Flag, initialDataToSend[i].numFixx,initialDataToSend[i].numFixy,initialDataToSend[i].numForce);
    }
    printf("Sending Headers\n");fflush(stdout);   
    if(SendNInts(sockfd,headersToSend,sizeof(int)*8*numFEMfiles)){
		std::cout << "Error in SendNInts for headers in clientControll" << std::endl;
		return 1;	
	}

    //send RBD data, including rotational axis
    sleep(2);
    SendNChar(sockfd,RBDdataToSend.data,RBDdataToSend.numBytes);



      
    for(int i=0;i<numFEMfiles; i++){
        printf("Sending Mesh %d\n",i);fflush(stdout); 
        //printElements(&initialDataToSend[i], i);

        SendNChar(sockfd,initialDataToSend[i].allTheData,initialDataToSend[i].numBytesToSend);
        sleep(1);
        

    }

    printf("Waiting for gogo");
    read(sockfd,buffer,255);
	printf("Server recieved data and ready, ansver was %s\n",buffer);
	check = strncmp(buffer,"gogo",4);
	if(check){
		printf("\nServer error with code %s\n",buffer);
		return -1;
	}
    device = "/dev/input/js0";
    *gogoSignal = 1;
    int intToRecieve = 0;
    while(gogo && shutDownSignal){

        sleep(1);
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
                    ReceiveInt32(sockfd,&intToRecieve);
                    if(!intToRecieve){
                        gogo = false;
                        printf("Recieved gogo = 0, clientController shutting down\n");
                    }
                    fflush(stdout);
                   
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
                    ReceiveInt32(sockfd,&intToRecieve);
                    if(!intToRecieve){
                        gogo = false;
                        printf("Recieved gogo = 0, clientController shutting down\n");
                    }
                    fflush(stdout);
                    break;
                default:
                    /* Ignore init events. */
                    break;
            }
        }
        
    }//while(gogo)

    //Send final controller values
    *gogoSignal = 0;
    check = SendNUnsignedChar(sockfd,dataToSend,numCharsToSend);
    printf("Shutdown sent\n");
    //If closed with ctrl-c , skip the receivers.
 if(shutDownSignal){  
    ReceiveInt32(sockfd,&intToRecieve);

    ReceiveNChars(sockfd,RBDdataToSend.data,RBDdataToSend.numBytes);
    /*Recieve final results. deformation, vonMieses, Force, force group*/
    for(int i=0;i<numFEMfiles;i++){
        ReceiveNChars(sockfd,initialDataToSend[i].finalFemDataToRecieve,initialDataToSend[i].numBytesForFinalFemReciever);
    }
}else{
    printf("Shuting down after SIGINT signal\n");
}
    printf("Saving final RBD results\n");
    fflush(stdout); 
  
    

    /*Save results*/
    saveRBDdataToFile(&RBDdataToSend,selectedFile.c_str(),numFEMfiles);
    printf("Saving final FEM\n");
    fflush(stdout); 
    for(int i=0; i<numFEMfiles;i++){
        qdClientWriter( &initialDataToSend[i], filesFEM[i].filename.c_str()); 
    }




    for(int i=0; i<numFEMfiles;i++){
        freeDataToSend(&initialDataToSend[i]);
    }
    close(js);
    close(sockfd);
    return 0;

}

