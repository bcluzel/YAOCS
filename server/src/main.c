#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>


#include "main.h"
#include "utils.h"


int main(int argc, char *argv[])
{
    printf("Server launched ! \n");
    struct user_bank connected_users;
    connected_users.num_of_users = 0;
    connected_users.users = NULL;

    int running = 1;

    if (mkfifo(SERV_PIPE_NAME, 0666) == -1)
    {
        int errsv = errno;
        if(errsv != EEXIST){
            perror("Error Mkfifo");
            exit(EXIT_FAILURE);
        }
    }
    
    int fd = open(SERV_PIPE_NAME, O_RDONLY | O_CREAT | O_NONBLOCK);
    exit_if(fd == -1, "fopen SERV_PIPE_NAME");
    struct message message;
    while (running)
    {
        usleep(500);
        if(read_header_nb(fd,&message)){
            printf("Message recived id:%u len:%u \n",message.user_id,message.data_len);
            if (!message.data_len)
            {
                // Hello process 
                process_hello(&connected_users,message.user_id);
            }else{
                message.data = malloc(sizeof(char)*message.data_len);
                recive_message(fd,message.data,message.data_len);
                if (message.data_len >=2)
                {
                    if (message.data[0] == CMD_SERVER) 
                    {
                        if (message.data[1] == END_OF_CONNECTION)
                        {
                            delete_user(&connected_users, message.user_id);
                        }
                    }else if (message.data[0] == CMD_USER){
                        
                    }else
                    {
                        printf("Message data : %s ",message.data);
                        broadcast_str(&connected_users,message.data);
                    }
                }else
                {
                    printf("Message data : %s ",message.data);
                    broadcast_str(&connected_users,message.data);
                }
                
                

                free(message.data);
            }
        }
    }
    close(fd);
    return EXIT_SUCCESS;
}


void process_hello(struct user_bank * connected_users, unsigned int user_id){
    printf("Process hello \n");
    if (search_user(connected_users,user_id) == -1)
    {
        printf("Adding user \n");
        add_user(connected_users,user_id);
    }else{
        perror("User already exist");
        // user already exist
    }
    
}

int search_user(struct user_bank *connected_users, unsigned int user_id){
    
    for (int i = 0; i < connected_users->num_of_users; i++)
    {
        if (user_id == connected_users->users[i].id)
        {
            printf(">%d\n",i);
            return i;
        }
        
    }
    return -1;
}

int add_user(struct user_bank *connected_users, unsigned int user_id){
    struct user new_user;
    new_user.id = user_id;
    new_user.fd = open_client_connection(user_id);
    strcpy(new_user.name,DEFAULT_NAME);

    struct user *next_users = malloc (sizeof(struct user) * (connected_users->num_of_users+1));
    for (int i = 0; i < connected_users->num_of_users; i++)
    {
        next_users[i] = connected_users->users[i];
    }
    free(connected_users->users);
    connected_users->num_of_users ++;
    printf("User connected: %u \n",connected_users->num_of_users);
    connected_users->users = next_users;
    connected_users->users[connected_users->num_of_users-1] = new_user;

    return 0;
}

int delete_user(struct user_bank *connected_users, unsigned int user_id){
    int index_user = 0;
    int offset = 0;
    if((index_user = search_user(connected_users,user_id)) != -1){
        exit_if(close(connected_users->users[index_user].fd) == -1, "close delete_user");
        connected_users->num_of_users --;
        struct user *next_users = malloc (sizeof(struct user) * (connected_users->num_of_users));
        for (int i = 0; i < connected_users->num_of_users; i++)
        {
            if (i == index_user)
            {
                offset = 1;
            }

            next_users[i] = connected_users->users[i+offset];
        }
        free(connected_users->users);
        printf("User deconnected: %u \n",connected_users->num_of_users);
        connected_users->users = next_users;
        return 0;
    }
    return -1;
}

int change_user_name(struct user_bank *connected_users, unsigned int user_id, char * username){
    int index_user;
    if((index_user = search_user(connected_users,user_id)) != 0){
        strcpy(connected_users->users[index_user].name,username);
        return 0;
    }
    return -1;
}

int open_client_connection(unsigned int user_id){
    char buffer[PIPE_FOLDER_PLUS_INTREP_LEN];
    char * path_mkfifo = path_mkfifo_client(user_id,buffer);
    printf("Client fifo location : %s \n", buffer);
    int fd = open(path_mkfifo, O_WRONLY);
    exit_if(fd == -1, "open open_client_connection");
    return fd;
}

void broadcast_str(struct user_bank *users, char * message){
    for (int i = 0; i < users->num_of_users; i++)
    {
        printf("Broadcast to user %d\n",i);
        send_message_str(users->users[i].fd,message,0);
    }
}