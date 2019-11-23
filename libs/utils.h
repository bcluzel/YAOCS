#ifndef _UTILS_H
#define _UTILS_H

#define ID_SERVER 0

#define SERV_PIPE_NAME "/tmp/chat"
#define SERV_LOG_NAME  "/tmp/server_log"
#define PIPE_DEFAULT_FOLDER "/tmp/"
#define LOCK_DEFAULT_FILE "/tmp/lock"
#define PIPE_FOLDER_PLUS_INTREP_LEN 18
#define MAX_USER_NAME_LEN 20
#define MAX_MESSAGE_LEN 254
#define CMD_SERVER 0x2
#define CMD_USER '/'
#define END_OF_CONNECTION  0x1

#define DEFAULT_NAME "Bobby"

#define FILE_DESCRIPTOR_RECIVED 0xA0

/**
 * Permet de stocker toutes les information concrenant un utilisateur connecté au server
 */
struct user {
    unsigned int id; /** Id de l'utilisateur */
    char name[MAX_USER_NAME_LEN]; /** Nom de l'utilisateur */
    int fd; /** File descriptor ouvert du pipe utilisateur */
};
/**
 * Permet de stocker le header ainsi que le message recu correspondant au header
 */
struct message {
    unsigned int user_id; /** Id de l'utilidateur envoyant le message*/
    unsigned int data_len; /** Taille du message envoyé */
    char *data; /** Message envoyé */
};

#define NUMBER_OF_BYTES_IN_HEADER 8
/**
 * @brief Exit printing error prefixed by `prefix` if `condition` is true (non zero)
 */
void exit_if(int condition, const char *prefix);
/**
 * @brief Converti un tableau de 4 char en un entier de 32 bits
 */
unsigned int four_char_to_int(char *bytes);
/**
 * @brief Converti un entier non signé de 32 bits en un tableau de 4 char
 */
void int_to_four_char(unsigned int n, char *bytes);

/**
 * @brief Fonction permetant de réaliser une lecture bloquante tant qu'un header n'est pas reçu
 * @param fd File descriptor ouvert
 */
struct message read_header(int fd);
/**
 * @brief Fonction permetant de réaliser une lecture de struct header non bloquante 
 * @return Retourne TRUE quand le header est lu
 * @param fd File descriptor ouvert
 * @param answer Pointeur vers une structure message ou sera écrit le nouveau header
 */
int read_header_nb(int fd, struct message *answer);
/**
 * @brief Fonction permetant de réaliser une lecture bloquante sur un message
 * @param fd File descriptor ouvert
 * @param data Pointeur vers un tableau de char de taille data_len
 * @param data_len Taille du tableau data
 */
void recive_message(int fd, char * data, int data_len);
/**
 * @brief Fonction permetant d'envoyer une trame compléte dans un server_fd 
 * 
 * Une trame compléte est composé d'un header (id_client plus message_len) suivi d'un message 
 * @param server_fd File descriptor ouvert
 * @param message Pointeur vers un tableau de char à envoyer de taille message_len
 * @param id_client Id de l'envoyeur de la trame
 * @param message_len Taille du tableau message
 */
void send_message(int server_fd, char *message, unsigned int id_client, int message_len);
/**
 * @brief Fonction permetant d'envoyer une trame compléte dans un server_fd à partir d'une chaine de caractére 
 * 
 * Une trame compléte est composé d'un header (id_client plus message_len) suivi d'un message
 * Ici la taille du message est calculé automatiquement en calculant la longeur du message passé en argument.
 * @param server_fd File descriptor ouvert
 * @param client_id Id de l'expéditeur de la trame
 * @param message Pointeur vers un chaine de caractére
 */
void send_message_str(int server_fd, char *message, unsigned int client_id);
/**
 * @brief Fonction permettant de créer un header en concaténant messag_len et id_client dans un tableau buffer
 * @param buffer Pointeur vers un tableau de 8 char
 * @param message_len Taille du message qui sera envoyé aprés le header
 * @param client_id Id de l'expéditeur de la trame
 */
void create_header(char *buffer, int message_len, unsigned int id_client);
/**
 * @brief Fonction permettant de créer le path de la fifo client en fonction de son id
 * 
 * Le path de la fifo client est alors `PIPE_DEFAULT_FOLDER` + `Id_client` conveti en chaine de caractére
 * @param id_client Id client
 * @param buffer Buffer de taille `PIPE_FOLDER_PLUS_INTREP_LEN`
 * 
 * @return Retourne un pointeur sur un tableau de char, equivalent à celui de buffer
 */
char * path_mkfifo_client(unsigned int id_client, char *buffer);

/**
 * @brief Fonction lisant l'entrée standard de maniére non bloquante
 * 
 * 
 * @param buffer Pointeur vers un Tableau de char de longeur `MAX_MESSAGE_LEN`
 * 
 * @return La fonction retourne TRUE si l'utilisateur rentre le caractére entrée ou la taille maximale est atteinte.
 */
int read_stdin(char *buffer);

#endif
