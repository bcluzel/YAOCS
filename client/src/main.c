#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <errno.h>


#include "main.h"
#include "utils.h"

struct client_info client;
unsigned int server_fd = 0;
int running;

int main(int argc, char *argv[])
{

    srand(time(NULL)); // Initiate random sequence

    printf("\n  YAOCS launched ! \n");
    running = 1;
    int fd = open(SERV_PIPE_NAME,O_WRONLY | O_NONBLOCK);

    server_fd = fd;

    if (fd < 0){

        if (fork() == 0) {
            int fd_null = open("/dev/null", O_RDONLY);
            exit_if(fd_null == -1," open fd_null");
            int fd_log = open(SERV_LOG_NAME, O_CREAT | O_APPEND | O_WRONLY, 0777);
            exit_if(fd_log == -1," open fd_log");
            dup2(fd_null, STDIN_FILENO);
            dup2(fd_log, STDOUT_FILENO);
            dup2(fd_log, STDERR_FILENO);
            exit_if(execl("./server/bin/server","server", (char*)NULL) !=0, "Impossible de demmarer le serveur en arriere plan");
        } 

        printf("Serveur en cours de demmarage.\n");
        fd = open(SERV_PIPE_NAME, O_WRONLY | O_NONBLOCK);

        exit_if(fd < 0, "Impossible de lancer le serveur");
    }

    client = init_connection(fd);
    char buffer_out[MAX_MESSAGE_LEN+1];
    printf("  Connecté au serveur ! \n  Pour plus d'aide : /help \n\n");

    struct message message;
    switch(fork()) {
        case -1: exit_if(1, "Can't fork");
        case 0: //Son
            while (running)
                {
                    if(read_stdin(buffer_out)){
                        if (!strcmp(buffer_out,"/exit\n"))
                        {
                            printf("Exiting !\n");
                            kill(getppid(),SIGUSR1);
                            running = 0;
                        }else
                        {
                            send_message_str_server(fd,buffer_out,client.id);
                        }
                        
                    }
                }
            break;
            
        default: //Dad
            signal(SIGINT, intHandler);
            signal(SIGUSR1, cleanExit);
            while (running)
            {
                usleep(500);
                if(read_header_nb(client.fd,&message)){
                    message.data = malloc(sizeof(char)*message.data_len);
                    recive_message(client.fd,message.data,message.data_len);
                    printf(message.data);
                    free(message.data);
                }
            }
            end_of_connection();
            close(fd);
            int wstatus;
            wait(&wstatus);
            break;
    };
    return EXIT_SUCCESS;
}

struct client_info init_connection(unsigned int server_fd) {
    struct client_info client;
    int fifo_created = 0;
    char buffer[PIPE_FOLDER_PLUS_INTREP_LEN];
    client.id = 0; // TODO FIW THE ERROR FOR ID > 0xFF
    do
    {
        //client.id = id_definer();
        client.id +=1;
        path_mkfifo_client(client.id , buffer);
        if (mkfifo(buffer, 0666) == -1)
        {
            int errsv = errno;
            if(errsv == EEXIST){
                fifo_created = 0;
            }else
            {
                perror("init connection mkfifo");
            }
            
        }else
        {
            fifo_created = 1;
        }
    } while (!fifo_created);
    hello(server_fd, client.id);
    exit_if((client.fd = open(buffer,O_RDONLY | O_NONBLOCK)) == -1, "OPEN init_connection \n");
    client.connected = 1;
    return client;
}

unsigned int id_definer(void) {
    unsigned int id = rand();
    if (id == 0)
        return id_definer();
    return id;
}


void hello(int server_fd, unsigned int client_id){
    char buffer[8];
    create_header(buffer, 0, client_id);
    lock();
    exit_if(write(server_fd, buffer, 8) == -1,"hello write");
    unlock();
}

void intHandler(int dummy) {
    //signal(dummy, SIG_IGN);
    end_of_connection();
    int wstatus;
    wait(&wstatus); //wait child to end
    exit(0);
}

void cleanExit(int dummy){
    running = 0;
}

void end_of_connection(){
    char buffer_path[PIPE_FOLDER_PLUS_INTREP_LEN];
    char buffer[2];
    buffer[0] = CMD_SERVER;
    buffer[1] = END_OF_CONNECTION;
    if (client.connected){
        send_message_server(server_fd, buffer, client.id, 2);
        close(client.fd);
        path_mkfifo_client(client.id,buffer_path);
        exit_if(remove(buffer_path)==-1,"remove end of connection");
        client.connected = 0;
    }
}

void lock(void){
    while (open(LOCK_DEFAULT_FILE, O_WRONLY|O_CREAT) == -1)
    {
        if (errno != EEXIST && errno != EACCES)
        {
            perror("Error lock");
            exit(EXIT_FAILURE);
        }
        
        usleep(10);
    }
}

void unlock(void){
    exit_if(remove(LOCK_DEFAULT_FILE) == -1,"remove Unlock");
}

void send_message_str_server(int server_fd, char *message, unsigned int client_id){
    lock();
    send_message_str(server_fd,message,client_id);
    unlock();
}
void send_message_server(int server_fd, char *message, unsigned int client_id, int message_len){
    lock();
    send_message(server_fd,message,client_id,message_len);
    unlock();
}