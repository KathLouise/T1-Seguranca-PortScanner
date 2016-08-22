#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_IP 13
#define MAX_PORT 63000

void appendChar(char* s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}

void changeLastDigit(char* str, char* final, int n, char c){
    char *i = &str[0]; int j = 0;

    while(*i != '\0' && j < n){
        appendChar(final, *i);
        i++;
        j++;
    }
    
    appendChar(final, c);
}

int char_to_int(char digit){
    char str[2];

    str[0] = digit;
    str[1] = '\0';

    return (int) strtol(str, NULL, 10);
}

int connect_IP(char* ip, int port){
    struct sockaddr_in server_addr;
    int sock, try_connect;

    //cria o socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if(sock < 0){
        printf("Erro ao criar o socket.\n");
        exit(1);    
    }
    server_addr.sin_addr.s_addr = inet_addr(ip); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    //Conectando ao ip
    int i = 0;
    try_connect = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if(try_connect < 0){
        perror("");
    }
    
    return sock;

}

void main(int argc, char *argv[]){
    char range_ipSeq[100];
    char range_portSeq[100];
    char first_ip[MAX_IP] = {0}, range_ip[MAX_IP] = {0};
    char lastIP='\0', firstIP='\0', lastPort='\0', firstPort='\0';
    char first_port[MAX_PORT] = {0};
    char *i, *j, *k, *w;
    int initIPSeq, finalIPSeq, initPortSeq, finalPortSeq, portnumber, range_port;

    if(argc < 3){
        printf("Entrada incorreta.\n A entrada deve seguir o seguinte modelo: ./portscanner <ip ou range de ips> <porta ou range de portas>");
        exit(0);
    }

    strcpy(range_ipSeq, argv[1]);
    strcpy(range_portSeq, argv[2]);

    printf("range_ip:%s \n", range_ipSeq);
    printf("range_port: %s \n", range_portSeq);  

    /*Tratando o range/numero do ip*/

    for(i = &range_ipSeq[0]; *i != '\0'; i++){
        if(*i == '-'){
            for(j = &i[1]; *j != '\0'; j++){
                lastIP = *j;
            }
            break;
        }
        appendChar(first_ip, *i);
        firstIP = *i;        
    }

    printf("first_ip:%s \n", first_ip);

    initIPSeq = char_to_int(firstIP);
    if(lastIP =='\0'){
        finalIPSeq = initIPSeq;
    }else{
        finalIPSeq = char_to_int(lastIP);
    }

    /*Tratando range/numero de porta*/

    for(k = &range_portSeq[0]; *k != '\0'; k++){
        if(*k == '-'){
            for(w = &k[1]; *w != '\0'; w++){
                lastPort = *w;
            }
            break;
        }
        appendChar(first_port, *k);
        firstPort = *k;        
    }

    printf("first_port:%s \n", first_port);

    initPortSeq = char_to_int(firstPort);
    portnumber = atoi(first_port);
    if(lastPort =='\0'){
        finalPortSeq = initPortSeq;
    }else{
        finalPortSeq = char_to_int(lastPort);
    }

    /*Loop de para scaneamento de ip + porta*/
    range_port = portnumber;
    for(int ip = initIPSeq; ip <= finalIPSeq; ip++){
       	range_ip[0] = '\0';
        changeLastDigit(first_ip, range_ip, 13, (ip +'0'));
        printf("ip Digit: %d\n", ip);
        printf("teste:%s \n", range_ip);
        
        for(int port = initPortSeq; port <= finalPortSeq; port++){
       	    printf("port : %d\n", port);
       	    printf("port number: %d\n", range_port);
            connect_IP(range_ip, range_port);
            range_port++;
        }
        range_port = portnumber;
        
    }

}
