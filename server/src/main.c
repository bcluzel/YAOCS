/** 
 * @file 
 * @brief Main du server YAOCS
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include "main.h"
#include "utils.h"

int running;
int main(int argc, char *argv[])
{
    switch(fork()) { //daemon
        case -1: exit_if(1, "Can't fork");
        case 0: //Son
            if (setsid() < 0)
                exit(EXIT_FAILURE);
            break;        
        default: //Dad
            exit(EXIT_FAILURE);
            break;
    };
    signal(SIGINT, intHandler);

    printf("Server launched ! \n");
    struct user_bank connected_users;
    connected_users.num_of_users = 0;
    connected_users.users = NULL;

    running = 1;

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
                        printf("UserCmd %s",message.data +1);

                        // TODO /help /users /msg /changename
                        if (strstr(message.data,"/users") != NULL)
                        {
                            process_list_users(&connected_users, message);
                        }else if (strstr(message.data,"/changename") != NULL)
                        {
                            process_change_name(&connected_users, message);
                        }else if (strstr(message.data,"/help") != NULL)
                        {
                            process_help(&connected_users, message);
                        }else if (strstr(message.data,"/msg") != NULL)
                        {
                            process_msg(&connected_users, message);
                        }

                    }else
                    {
                        printf("Message data : %s",message.data);
                        broadcast_str_excluding(&connected_users,message.data,message.user_id);
                    }
                }else
                {
                    printf("Message data : %s",message.data);
                    broadcast_str_excluding(&connected_users,message.data,message.user_id);
                }
                
                free(message.data);
            }
        }
    }
    exit_if(close(fd) == -1, "Close main \n");
    end_of_connection();
    printf("Server closed !\n");
    return EXIT_SUCCESS;
}

void intHandler(int dummy) { //n'est plus utilisé
    end_of_connection();
    exit(0);
}

void end_of_connection(){
    exit_if(remove(SERV_PIPE_NAME)==-1,"remove end of connection");
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
            return i;
        }
        
    }
    return -1;
}

void add_user(struct user_bank *connected_users, unsigned int user_id){
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
        if (connected_users->num_of_users == 0)
        {
            printf("STOPPPING\n");
            stopserver();
        }
        return 0;
    }
    return -1;
}

int change_user_name(struct user_bank *connected_users, unsigned int user_id, char * username){
    int index_user;
    if((index_user = search_user(connected_users,user_id)) != -1){
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

void broadcast_str_excluding(struct user_bank *users, char * message, int excluded_id){
    for (int i = 0; i < users->num_of_users; i++)
    {
        if (users->users[i].id != excluded_id)
        {
            printf("Broadcast to user %d\n",i);
            char buffer[50];

            time_t rawtime;
            struct tm * timeinfo;

            time ( &rawtime );
            timeinfo = localtime ( &rawtime );

            sprintf(buffer, "[%d:%d:%d] >> %s -> %s", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, users->users[search_user(users, excluded_id)].name, message);
            send_message_str(users->users[i].fd,buffer,0);
        }
    }
}


void process_list_users(struct user_bank *connected_users, struct message message){
    char buff[MAX_USER_NAME_LEN + 10];
    int client_fd = connected_users->users[search_user(connected_users,message.user_id)].fd;
    send_message_str(client_fd,"UTILISATEURS:\n",ID_SERVER);
    for (int i = 0; i < connected_users->num_of_users; i++)
    {
        strcpy(buff,"> ");
        strcat(buff, connected_users->users[i].name);
        strcat(buff, "\n");
        send_message_str(client_fd,buff,ID_SERVER);
    }
    
}
void process_change_name(struct user_bank *connected_users, struct message message){
    int name_len;
    int index_user = search_user(connected_users,message.user_id);
    int client_fd = connected_users->users[index_user].fd;
    char *name = strchr(message.data,' ');
    if (name == NULL)
    {
        send_message_str(client_fd,"Pas d'argument !\n",ID_SERVER);
        return;
    }else
    {
        name = name + 1; // on enléve le ' '
    }
    char *end = strchr(name,'\n');
    if (end != NULL)
    {
        end[0] = '\0';
    }else
    {
        send_message_str(client_fd,"Pas d'argument !\n",ID_SERVER);
        return;
    }
    
    printf("Name : %s\n", name);
    name_len = strlen(name);
    if (name_len > MAX_USER_NAME_LEN)
    {
        send_message_str(client_fd,"Nom trop long !\n",ID_SERVER);
    }else if (name_len<2){
        send_message_str(client_fd,"Nom trop court !\n",ID_SERVER);
    }else{
        change_user_name(connected_users,message.user_id,name);
        send_message_str(client_fd,"Nom changé!\n",ID_SERVER);
    }
    
}
void process_msg(struct user_bank *connected_users, struct message message){
    //int index_user = search_user(connected_users,message.user_id);
    //int client_fd = connected_users->users[index_user].fd;
}
void process_help(struct user_bank *connected_users, struct message message){
    int index_user = search_user(connected_users,message.user_id);
    int client_fd = connected_users->users[index_user].fd;
    send_message_str(client_fd,"Liste les commandes :\n",ID_SERVER);
    send_message_str(client_fd,"> /changename <nom>\n",ID_SERVER);
    send_message_str(client_fd,"> /users - liste les utilisateurs connectés\n",ID_SERVER);
    send_message_str(client_fd,"> /msg <nom client> <message> - envoi un message privé\n",ID_SERVER);
    send_message_str(client_fd,"> /exit\n",ID_SERVER);
}

void stopserver(){
    running = 0;
}