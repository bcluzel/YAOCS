#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "main.h"
#include "utils.h"


int main(int argc, char *argv[])
{
    printf("Server launched ! \n");

    int runing = 1;

    if (mkfifo(SERV_PIPE_NAME, 0666) == -1)
    {
        int errsv = errno;
        if(errsv != EEXIST){
            perror("Error Mkfifo");
            exit(EXIT_FAILURE);
        }
    }
    
    int fd = open(SERV_PIPE_NAME, O_RDONLY | O_CREAT);
    exit_if(fd == -1, "fopen SERV_PIPE_NAME");

    while (runing)
    {

        struct message message = read_header(fd);
        printf("%d %d \n",message.user_id,message.data_len);
        message.data = malloc(sizeof(char)*message.data_len);
        recive_message(fd,message.data,message.data_len);
        printf("%s",message.data);
        free(message.data);
    }
    

    close(fd);
    return EXIT_SUCCESS;
}


struct message read_header(int fd){

    static char buffer [NUMBER_OF_BYTES_IN_HEADER];
    static int head=0;
    while (head<NUMBER_OF_BYTES_IN_HEADER)
    {
        int answer_read = read(fd,&buffer[head],1);
        exit_if(answer_read == -1, "read read_client");
        head += answer_read;
        /*
        if (answer_read == 0)
        {
            sleep(1);
        }
        */
    }
    head = 0;
    struct message answer;
    answer.user_id = four_char_to_int(&buffer[0]);
    answer.data_len = four_char_to_int(&buffer[4]);
    return answer;
}


void recive_message(int fd, char * data, int data_len){
    int head=0;
    while (head<data_len)
    {
    int answer_read = read(fd,&data[head],1);
    exit_if(answer_read == -1, "read read_client");
    head += answer_read;
    }
    head = 0;
}


