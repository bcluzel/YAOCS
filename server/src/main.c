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


