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
    
    int fd = open(SERV_PIPE_NAME,O_RDONLY | O_CREAT);
    exit_if(fd == -1, "fopen SERV_PIPE_NAME");

    while (runing)
    {

        struct message message = read_header(fd);
        printf("%d %d \n",message.user_id,message.data_len);
        message.data = malloc(sizeof(char)*message.data_len);
        
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
        int answer = read(fd,&buffer[head],1);
        exit_if(answer == -1, "read read_client");
        head += answer;
    }
    head = 0;
    struct message answer;
    answer.user_id = four_char_to_int(&buffer[0]);
    answer.data_len = four_char_to_int(&buffer[4]);
    return answer;
}


// void read_client(int fd){
//     struct message recived_message;
//     int answer = read(fd,&recived_message.user_id,sizeof(recived_message.user_id));
//     printf("%d\n", answer);
//     if (answer != sizeof(recived_message.user_id))
//     {
//         return;
//     }  
//     exit_if(answer == -1, "read read_client");
//     answer = read(fd,&recived_message.data_len,sizeof(recived_message.data_len));
//     if (answer != sizeof(recived_message.data_len))
//     {
//         return;
//     }
//     exit_if(answer == -1, "read read_client");
//     printf("%d",recived_message.data_len);

// }