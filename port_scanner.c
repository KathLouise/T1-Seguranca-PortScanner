#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void append(char* s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}

void replacer(char* str, char* final, int n, char c)
{
    char *i = &str[0]; int j = 0;
    while(*i != '\0' && j < n){
        append(final, *i);
        i++;
        j++;
    }
    
    append(final, c);

}

int digit_to_int(char d)
{
 char str[2];

 str[0] = d;
 str[1] = '\0';
 return (int) strtol(str, NULL, 10);
}

void main(int argc, char *argv[]){
    char range_ip[100];
    char range_port[100];

    if(argc < 3){
        printf("Entrada incorreta.\n A entrada deve seguir o seguinte modelo: ./portscanner <ip ou range de ips> <porta ou range de portas>");
        exit(0);
    }

    strcpy(range_ip, argv[1]);
    strcpy(range_port, argv[2]);

    printf("range_ip:%s \n", range_ip);
    printf("range_port: %s \n", range_port);

    int tam_range_ip = strlen(range_ip);
    int tam_range_port = strlen(range_port);
    char *i, *j;
    char first_ip[13] = {0}, last_ip[13] = {0};
    char lastIP='\0', firstIP='\0';

    for(i = &range_ip[0]; *i != '\0'; i++){
        printf("character I: %c\n", *i);
        if(*i == '-'){
            for(j = &i[1]; *j != '\0'; j++){
                printf("character J: %c\n", *j);
                lastIP = *j;
            }
            break;
        }
        append(first_ip, *i);
        firstIP = *i;        
    }

    replacer(first_ip, last_ip, 13, lastIP);
    printf("first_ip:%s \n", first_ip);
    printf("digits:%c \n", lastIP);
    printf("last_ip:%s \n", last_ip);

    int init = digit_to_int(firstIP);
    int final;
    if(lastIP =='\0'){
        final = init;
    }else{
        final = digit_to_int(lastIP);
    }

    for(int a = init; a <= final; a++){
        last_ip[0] = '\0';
        replacer(first_ip, last_ip, 13, (a+'0'));
        printf("a: %d\n", a);
        printf("teste:%s \n", last_ip);
    }

}
