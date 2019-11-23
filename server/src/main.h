#include "utils.h"
/**
 * @brief Structure coté server stockant les utilisateurs connectés
 * 
 */
struct user_bank {
    int num_of_users; /** Nombre d'utilisateurs dans le tableau users*/
    struct user * users; /** Tableau alloué dynamiquement*/
};
/**
 * @brief Appelé lors de la reception d'un hello : enregiste un nouvel utilisateur dans la structure user_bank
 * 
 * @param connected_users Structure contenant les utilisateurs connectés
 * @param user_id Id de l'utilisateur ayant envoyé le hello
 */
void process_hello(struct user_bank *connected_users, unsigned int user_id);
/**
 * @brief Recherche un utilisateur dans la structure user_bank
 * 
 * @param connected_users Structure contenant les utilisateurs connectés
 * @param user_id Id de l'utilisateur à trouver
 * @return int : Retourne l'index de l'utilisateur dans le tableau de la structure `connected_users` 
 * et -1 si il n'est pas trouvé
 */
int search_user(struct user_bank *connected_users, unsigned int user_id);
/**
 * @brief Ajoute un utilisateur à la structure user_bank
 * 
 * @param connected_users  Structure contenant les utilisateurs connectés
 * @param user_id Id de l'utilisateur à ajouter
 */
void add_user(struct user_bank *connected_users, unsigned int user_id);
/**
 * @brief Suprime un utilisateur à la structure user_bank
 * 
 * @param connected_users  Structure contenant les utilisateurs connectés
 * @param user_id Id de l'utilisateur à retirer
 * 
 * @return int : -1 si l'utilisateur n'existe pas, 0 sinon
 */
int delete_user(struct user_bank *connected_users, unsigned int user_id);
/**
 * @brief Change le nom d'un utilisateur sovké dans la structure user_bank
 * 
 * @param connected_users  Structure contenant les utilisateurs connectés
 * @param user_id Id de l'utilisateur
 * @param username Nouveau pseudo
 * 
 * @return int : -1 si l'utilisateur n'existe pas, 0 sinon
 */
int change_user_name(struct user_bank *connected_users, unsigned int user_id, char * username);
/**
 * @brief Ouvre le pipe d'un client
 * 
 * @param user_id Id du client
 * @return int : File desciptor du pipe appartenant au client
 */
int open_client_connection(unsigned int user_id);
/**
 * @brief Envoi à tous les utilisateurs connectés un message
 * 
 * @param users Structure contenant les utilisateurs connectés
 * @param message 
 */
void broadcast_str(struct user_bank *users, char * message);
/**
 * @brief Envoi à tous les utilisateurs connectés un message sauf à l'utilisateur passé en argument
 * 
 * @param users Structure contenant les utilisateurs connectés
 * @param message 
 * @param excluded_id Id de l'utilisateur exclu
 */
void broadcast_str_excluding(struct user_bank *users, char * message, int excluded_id);
/**
 * @brief Handdle the Ctrl+c
 * 
 * @param dummy 
 */
void intHandler(int dummy);
/**
 * @brief Permet d'effectuer toutes les tâches de fin de connection lors d'un arrét server
 * 
 */
void end_of_connection();
/**
 * @brief Traite la comande client /listusers : retourne les noms des utilisateurs connectés
 * 
 * @param connected_users 
 * @param message 
 */
void process_list_users(struct user_bank *connected_users, struct message message);
/**
 * @brief Traite la comande client /changename : change le nom de l'utilisateur
 * 
 * @param connected_users 
 * @param message 
 */
void process_change_name(struct user_bank *connected_users, struct message message);
/**
 * @brief [NON IMPLÉMENTÉ] Traite la comande client /msg <X> <msg>: envoi un message(msg) privé à un utilisateur X
 * 
 * @param connected_users 
 * @param message 
 */
void process_msg(struct user_bank *connected_users, struct message message);
/**
 * @brief Traite la comande client /help : retourne la liste des commandes possibes
 * 
 * @param connected_users 
 * @param message 
 */
void process_help(struct user_bank *connected_users, struct message message);
/**
 * @brief Permet d'arréter proprement le server
 * 
 */
void stopserver();