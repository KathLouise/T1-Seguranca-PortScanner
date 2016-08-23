#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <memory.h>

#define MAX_IP 16
#define MAX_PORT 66000

char* itoa(int val, int base){
	static char buf[32] = {0};
	int i = 30;
	
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	
	return &buf[i+1];
}

void createToken(char *range[],  char string[], const char delimiter[], int *cont){
    int i = 0;
    char *token;
    /* get the first token */
    token = strtok(string, delimiter);
   
    /* walk through other tokens */
    while(token != NULL){
        range[i] = token;
        token = strtok(NULL, delimiter);
        i++;
    }
    range[i] = token;
    *cont = i;
}

void constructIPRange(char range_ipSeq[], char *range_ip[], char *rangeTotal[]){
    const char delimiter[2] = "-";
    const char delimiter2[2] = ".";
    char *token, *token2;
    char *range[MAX_IP] = {0};
    char *range2[MAX_IP] = {0};
    char first_ip[MAX_IP] = {0};
    char *finalIPBand;
    int i = 0, j, tamInit, tamFinal, initIP, finalIP;
    char *ponto = ".";
    char *addr;
    char ip_aux[MAX_IP] = {0};
    char ip[MAX_IP] = {0};
    char range_ips[255] = {0};
    
    createToken(range_ip, range_ipSeq, delimiter, &i);
    
    finalIPBand = range_ip[1];
    i = 0;
    createToken(range, range_ip[0], delimiter2, &i);
    tamInit = i-1;
    
    i = 0;
    createToken(range2, finalIPBand, delimiter2, &i);
    tamFinal = i-1;

    for(i = 0; i < MAX_IP; i++){
        if(i < tamInit){
            strcpy(ip_aux, range[i]);
            strcat(ip_aux, ponto);
            strcat(ip, ip_aux);
        }
    }
    
    initIP = atoi(range[tamInit]);
    if(range2[0] == NULL){
        finalIP = initIP;
    }else{
        finalIP = atoi(range2[tamFinal]);
    }
    
    int numTotalIP = finalIP-initIP;

    j = 0;
    if(range2[0] == NULL){
        strcat(ip, range[tamInit]);
        rangeTotal[0] = strdup(ip);
    }else{
        for(i = initIP; i <= finalIP; i++){
            if(i == 0){
                ip_aux[0] = '\0';
                strcpy(ip_aux, ip);
                strcat(ip_aux, range[tamInit]);
                rangeTotal[0] = strdup(ip_aux);
                j++;
            }else{
                ip_aux[0] = '\0';
                addr = itoa(i,10);
                strcpy(ip_aux, ip);
                strcat(ip_aux, addr);
                rangeTotal[j] = strdup(ip_aux);
                j += 1;
            }
        }
    }
    printf( "ip inicial: %d\n", initIP );
    printf( "ip final: %d\n", finalIP );
    
}

void constructPortRange(char range_ipSeq[], int range[], int *numPort){
    char *token;
    const char delimiter[2] = "-";
    int port[2];
    int init, final, i = 0, j =0;
    
    /* get the first token */
    token = strtok(range_ipSeq, delimiter);
   
    /* walk through other tokens */
    while(token != NULL){
        port[i] = atoi(token);
        token = strtok(NULL, delimiter);
        i++;
    }
    init = port[0];
    final = port[1];

    *numPort = final - init;
    for(i = init; i <= final; i++){
        range[j] = i;
        j += 1;
    }
    
}

void main(int argc, char *argv[]){
    char range_ipSeq[100];
    char range_portSeq[100];
    char *range_ip[MAX_IP] = {0};
    char *rangeTotal[256] = {0};
    char first_port[MAX_PORT] = {0};
    int range_port[MAX_PORT];
    int numPort;


    if(argc < 3){
        printf("Entrada incorreta.\n\n A entrada deve seguir o seguinte modelo: ./portscanner <ip ou range de ips> <porta ou range de portas>\n");
        exit(0);
    }

    strcpy(range_ipSeq, argv[1]);
    strcpy(range_portSeq, argv[2]);

    constructIPRange(range_ipSeq, range_ip, rangeTotal);
    constructPortRange(range_portSeq, range_port, &numPort);

}
