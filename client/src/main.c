#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include "main.h"
#include "utils.h"

unsigned int id_client = 0;
unsigned int connected = 0;
unsigned int server_fd = 0;

int main(int argc, char *argv[])
{

    signal(SIGINT, intHandler);

    srand(time(NULL)); // Initiate random sequence

    id_client = id_definer();
    printf("Client id %u \n",id_client);
    printf("YAOCS launched ! \n");
    int running = 1;
    int fd = open(SERV_PIPE_NAME,O_WRONLY);

    server_fd = fd;

    if (fd == -1){
        perror("Pas de serv ");
        exit(EXIT_FAILURE);
        //exec delay retry
    }

    exit_if(init_connection(fd, id_client) == -1, ERR_CLIENT_TIMEOUT);

    while (running)
    {
        /* code */
    }
    
    send_message_server(fd,"BABA \n", id_client);

    close(fd);
    return EXIT_SUCCESS;
}

int init_connection(unsigned int server_fd, unsigned int client_id) {
    hello(server_fd, client_id);

    int r = 0;
    int fildes[2];

    struct message msg;

    do {

        pipe(fildes);
        close(fildes[1]);

        char buffer[14];
        buffer[0] = CMD_SERVER;
        buffer[1] = FILE_DESCRIPTOR_TX;
        int_to_four_char(fildes[0], &buffer[2]);
        // printf("Filedes %u \n",fildes[0]);
        //         for (int i = 0; i < 6; i++)
        // {
        //     printf("! %u ",buffer[i]);
        // }
        // printf("\n");
        send_message(server_fd, buffer, id_client, 6);
        msg = read_header(fildes[0]);
        printf("Message len %d", msg.data_len);
        recive_message(fildes[0], msg.data, msg.data_len);
        for (int i = 0; i < msg.data_len; i++)
        {
            printf("%d ",msg.data[i]);
        }
        
        //if (msg.data == "")

        ++r;
    } while (r < MAX_RETRY || connected == 1);

    return 0;
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
     printf("OUCH, did you hit Ctrl-C?\n"
            "Do you really want to quit? [y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y') {
        char buffer[2];
        buffer[0] = CMD_SERVER;
        buffer[1] = END_OF_CONNECTION;
        if (connected)
            send_message_server(server_fd, buffer, id_client);
        exit(0);
     } else
          signal(SIGINT, intHandler);
     getchar(); // Get new line character
}
