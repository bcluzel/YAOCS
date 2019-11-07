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
    struct user_bank connected_users;
    connected_users.num_of_users = 0;
    connected_users.users = NULL;

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
        printf("%u %u \n",message.user_id,message.data_len);
        if (!message.data_len)
        {
            // Hello process 
            process_hello(connected_users,message.user_id);

        }else{
            message.data = malloc(sizeof(char)*message.data_len);
            recive_message(fd,message.data,message.data_len);
            printf("%s",message.data);
            free(message.data);
        }
        
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

void process_hello(struct user_bank connected_users, unsigned int user_id){
    if (!search_user(connected_users,user_id))
    {
        add_user(connected_users,user_id);
    }else{
        // user already exist
    }
    
}

unsigned int search_user(struct user_bank connected_users, unsigned int user_id){
    for (int i = 0; i < connected_users.num_of_users; i++)
    {
        if (user_id == connected_users.users[i].id)
        {
            return i;
        }
        
    }
    return 0;
}

int add_user(struct user_bank connected_users, unsigned int user_id){
    struct user *next_users = malloc (sizeof(struct user) * (connected_users.num_of_users+1));
    for (int i = 0; i < connected_users.num_of_users; i++)
    {
        next_users[i] = connected_users.users[i];
    }
    free(connected_users.users);
    connected_users.num_of_users ++;
    connected_users.users = next_users;
    return 0;
}

int delete_user(struct user_bank connected_users, unsigned int user_id){
    return 0;
}

int change_user_name(struct user_bank connected_users, unsigned int user_id, char * username){
    return 0;
}
