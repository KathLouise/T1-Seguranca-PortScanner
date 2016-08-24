#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <memory.h>
#include <ctype.h>
#include <time.h>

#define MAX_IP 16
#define MAX_PORT 65536

int validateIPAddr(char *ip){
    struct sockaddr_in socket;
    
    int result = inet_pton(AF_INET, ip, &(socket.sin_addr));
    
    return result;
}

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

void constructIPRange(char range_ipSeq[], char *rangeTotal[], int *numIP){
    const char delimiter[2] = "-";
    const char delimiter2[2] = ".";
    char *token, *token2, *addr;
    char *range_ip[MAX_IP] = {0};
    char *range[MAX_IP] = {0};
    char *range2[MAX_IP] = {0};
    char first_ip[MAX_IP] = {0};
    char *finalIPBand;
    char *ponto = ".";
    char ip_aux[MAX_IP] = {0};
    char ip[MAX_IP] = {0};
    char range_ips[255] = {0};
    int i = 0, j, tamInit, tamFinal, initIP, finalIP;
    
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
    
    if((!isdigit(*range[tamInit])) || (!isdigit(*range2[0]))){
        printf("Digito inicial ou final do range é desconhecido. \n");
        printf("Tente novamente, fornecendo somente numeros: XXX.XXX.XXX.XXX .\n");
        exit(1);
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
    
    *numIP = numTotalIP;
}

void constructPortRange(char range_portSeq[], int range[], int *numPort){
    int i = 0;
    char *token;
    const char delimiter[2] = "-";
    int port[2];
    int init, final, j =0;
    /* get the first token */
    token = strtok(range_portSeq, delimiter);
   
    /* walk through other tokens */
    while(token != NULL){
        if(!isdigit(*token)){
            printf("Range de porta invalida.\n");
            exit(1);
        }
        port[i] = atoi(token);
        token = strtok(NULL, delimiter);
        i++;
    }
    
    init = port[0];
    final = port[1];
    
    if(init > MAX_PORT || final > MAX_PORT){
        printf("Range de porta extrapolou o valor permitido.\n");
        exit(1);
    }

    for(i = init; i <= final; i++){
        range[j] = i;
        j += 1;
    }
    *numPort = i;
    
}

int connectIP(int port_number, char *ip){
    struct sockaddr_in server_addr;
    int sock, try_connect, recv;
    char buffer[256];

    //cria o socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(sock < 0){
        printf("Erro ao criar o socket.\n");
        exit(1);    
    }
    server_addr.sin_addr.s_addr = inet_addr(ip); //IP do server, neste caso localhost
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);

    //Conectando ao server
    int i = 0;
    try_connect = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if(try_connect < 0){
        printf("Erro ao tentar conectar. \n");
        perror("");
    }
    sleep(2);
    recv = read(sock, buffer, 255);
    if(recv < 0){
        printf("Erro na leitura");
        exit(1);
    }else{
        printf("%s\t %d\t %s", ip, port_number, buffer);
    }
    
    return sock;
}


void main(int argc, char *argv[]){
    char range_ipSeq[100];
    char range_portSeq[100];
    char *range_ip[256] = {0};
    char *isIPRange, *isPortRange;
    char first_port[MAX_PORT] = {0};
    int range_port[MAX_PORT];
    int numPort = 0, numIP = 0, i, j, isValidIP, hasIPRange = 1, hasPortRange = 1, sock;
    time_t r_time;
    struct tm *info;

    if(argc < 3){
        printf("Entrada incorreta.\n\n");
        printf("A entrada deve seguir o seguinte modelo: ./portscanner <ip ou range de ips> <porta ou range de portas>\n");
        exit(0);
    }

    strcpy(range_ipSeq, argv[1]);
    strcpy(range_portSeq, argv[2]);
 
    time(&r_time);
    info = gmtime(&r_time);
    printf("\nVarredura iniciada em %s\n", asctime(info));

    printf("IP: %s\n", range_ipSeq);
    printf("Portas: %s\n", range_portSeq);

    printf("\n---------\n\n");
    
    isIPRange = strchr (range_ipSeq, '-');
    if(isIPRange != NULL){
        constructIPRange(range_ipSeq, range_ip, &numIP);
    }else{
        if(isdigit(range_ipSeq[1])){
            if(range_ipSeq[1] != '\0'){
                range_ip[0] = range_ipSeq;
                numIP = 1;
            }
        }else{
            printf("Caracter de separação invalido, ou range desconhecido. \n");
            printf("Tente novamente, separando seu range por - .\n");
            exit(1);
        }
    }
    isPortRange = strchr (range_portSeq, '-');
    if(isPortRange != NULL){
        constructPortRange(range_portSeq, range_port, &numPort);
    }else{
        if(isdigit(range_portSeq[1])){
            if(!isdigit(range_portSeq[0])){
                printf("Porta inválida.\n");
                exit(1);
            }
            if(atoi(range_portSeq) > MAX_PORT){
                printf("Range de porta extrapolou o valor permitido.\n");
                exit(1);
            }
            range_port[0] = atoi(range_portSeq);
            numPort = 1;
        }else{
            printf("Caracter de separação invalido, ou range desconhecido. \n");
            printf("Tente novamente, separando seu range por - .\n");
            exit(1);
        }
    }
    
    for(i = 0; i < numIP; i++){
        isValidIP = validateIPAddr(range_ip[i]);
        if(isValidIP){
            for(j = 0; j < numPort; j++){
                sock = connectIP(range_port[j], range_ip[i]);
            }
        }else{
            printf("IP inválido.\n");
        }
    }
}
