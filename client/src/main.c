#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "main.h"
#include "utils.h"

int id_client = 0;

int main(int argc, char *argv[])
{
    printf("Client launched ! \n");


    int fd = open(SERV_PIPE_NAME,O_WRONLY);
    if (fd == -1){
        perror("Pas de serv ");
        exit(EXIT_FAILURE);
        //exec delay retry
    }

    hello(fd);
    

    close(fd);
    return EXIT_SUCCESS;
}

void send_message_server(int server_fd, char *message, int message_len){
    exit_if(write(server_fd,message,message_len),"send serv message write");

}

void hello(int server_fd){
    char buffer[8];
    int_to_four_char(id_client,buffer);
    int_to_four_char(0,&buffer[4]); // message len de 0
    exit_if(write(server_fd,buffer,8) == -1,"hello write");
    printf("%d \n",id_client);
}