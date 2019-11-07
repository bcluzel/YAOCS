#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "main.h"
#include "utils.h"

int id_client = 0;

int main(int argc, char *argv[])
{
    printf("YAOCS launched ! \n");
    int running = 1;
    int fd = open(SERV_PIPE_NAME,O_WRONLY);
    if (fd == -1){
        perror("Pas de serv ");
        exit(EXIT_FAILURE);
        //exec delay retry
    }
    hello(fd);
    while (running)
    {
        /* code */
    }
    
    send_message_server(fd,"BABA \n");

    close(fd);
    return EXIT_SUCCESS;
}

void send_message_server(int server_fd, char *message){
    char buffer[8];
    create_header(buffer, strlen(message)+1);
    exit_if(write(server_fd,buffer,8) == -1,"hello message write");
    exit_if(write(server_fd,message,strlen(message)+1),"send serv message write");
}

void hello(int server_fd){
    char buffer[8];
    create_header(buffer, 0);
    exit_if(write(server_fd,buffer,8) == -1,"hello write");
}

void create_header(char *buffer, int message_len){
    int_to_four_char(id_client,buffer);
    int_to_four_char(message_len,&buffer[4]); // message len de 0
}