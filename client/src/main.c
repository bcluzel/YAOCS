#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <errno.h>


#include "main.h"
#include "utils.h"

unsigned int id_client = 0;
unsigned int connected = 0;
unsigned int server_fd = 0;

int main(int argc, char *argv[])
{

    signal(SIGINT, intHandler);

    srand(time(NULL)); // Initiate random sequence

    printf("YAOCS launched ! \n");
    int running = 1;
    int fd = open(SERV_PIPE_NAME,O_WRONLY);

    server_fd = fd;

    if (fd == -1){
        perror("Pas de serv ");
        exit(EXIT_FAILURE);
        //exec delay retry
    }
    struct client_info client = init_connection(fd);

    while (running)
    {
        struct message message = read_header(client.fd);
        message.data = malloc(sizeof(char)*message.data_len);
        recive_message(client.fd,message.data,message.data_len);
        printf("From %d : %s \n",message.user_id, message.data);
        free(message.data);
    }
    
    close(fd);
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
            if(errsv != EEXIST){
                fifo_created = 0;
            }else
            {
                perror("init connection mkfifo \n");
            }
            
        }else
        {
            fifo_created = 1;
        }
    } while (!fifo_created);
    printf("Client fifo location : %s \n", buffer);
    hello(server_fd, client.id);
    exit_if((client.fd = open(buffer,O_RDONLY)) == -1, "OPEN init_connection \n");
    printf("Init OK ! \n");
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
    exit_if(write(server_fd, buffer, 8) == -1,"hello write");
}

void intHandler(int dummy) {
    char  c;

     signal(dummy, SIG_IGN);
     printf("\rOUCH, did you hit Ctrl-C?\n"
            "Do you really want to quit? [Y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y') {
        char buffer[2];
        buffer[0] = CMD_SERVER;
        buffer[1] = END_OF_CONNECTION;
        if (connected)
            send_message(server_fd, buffer, id_client, 2);
        exit(0);
     } else
          signal(SIGINT, intHandler);
     getchar(); // Get new line character
}
