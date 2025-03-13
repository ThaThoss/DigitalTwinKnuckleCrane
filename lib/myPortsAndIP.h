

typedef struct{

    char Port_Internal_FEM[5] = "8080";//Used by femqd.
    char Port_Internal_KNUCKLE[5] = "8083";//Used by Knuckle
    char Port_External_Knuckle[5] =  "8081";
    char Port_External_FEM[5] = "8082";
    char Port_Main_Server[5] = "8085";

    char ipFEM[10] = "127.0.0.1";
    char ipKnuckle[10] = "127.0.0.1";
    char ipMainServer[10] = "127.0.0.1";

}MYPORTSANDIP;