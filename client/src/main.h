/**
 * @brief Rassemble les information utiles d'un client.
 * 
 * Cette structure permet d'augementer la lisibilitée du code
 * 
 */
struct client_info
{
    unsigned int id; /** Id du client*/
    int fd; /** File descriptor du server auquel le client est connecté*/
    int connected; /** Variable servant à savoir si le client est connecté*/
};


/**
 * @brief Fonction générant un id aléatiore différent de 0 (id server)
 * 
 * @return unsigned int correspondant à l'id client
 */
unsigned int id_definer(void);
/**
 * @brief Initialise la connection à un server
 * 
 * @param server_fd File descriptor du server ouvert
 * @return struct client_info : Retourne les infos du client connecté
 */
struct client_info init_connection(unsigned int server_fd);
/**
 * @brief Envoi une trame hello au server
 * 
 * @param server_fd File descriptor du server ouvert
 * @param id_client Id du client envoyant le message
 */
void hello(int server_fd, unsigned int id_client);
/**
 * @brief Permet d'effectuer toutes les tâches de fin de connection lors d'un arrét client
 * 
 */
void end_of_connection();
/**
 * @brief Apelle `send_message_str` en appelant `lock` avant et `unlock` aprés.
 */
void send_message_str_server(int server_fd, char *message, unsigned int client_id);
/**
 * @brief Apelle `send_message` en appelant `lock` avant et `unlock` aprés.
 */
void send_message_server(int server_fd, char *message, unsigned int client_id, int message_len);
/**
 * @brief Empéche l'interleaving, la fonction crée un fichier `lock` et exit si il n'y à pas déja un fichier lock 
 * 
 */
void lock(void);
/**
 * @brief Suprime le fichier `lock` précedement créé par la fonction `lock()`
 * 
 */
void unlock(void);
/**
 * @brief Handdle the Ctrl+c
 * 
 * @param dummy 
 */
void intHandler(int dummy);
/**
 * @brief Handdle the SIGUSR1 (called on cleanexit)
 * 
 * @param dummy 
 */
void cleanExit(int dummy);
