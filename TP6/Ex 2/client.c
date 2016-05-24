/* Exercice 2 : Communication par files de messages
Usage : ./client.e "Chaine de caractère à envoyer"

Le client ouvre la file de message permettant d'envoyer sa chaine au serveur. (clé = MSGKEY). il envoie alors la chaine à traduire.
La réponse du serveur se fait sur une autre file de message qui est identifié par le pid du client. On ouvre donc cette file de message pour receptionner la réponse.

Etudiants : Bertrand - Bruchon
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "calcul.h"
#include "string.h"


int main(int argc, char const *argv[])
{
        int msg_id;			//Id de la file pour l'envoie des messages au serveur
        struct msg_struct msg;		//Structure de donnée qui contiendra les messages (reçu et émis)

        if (argc != 2) {		//On vérifie le nombre d'argument
                printf("Usage: %s chaine", argv[0]);
                exit(-1);
        }  

/** PARTIE ENVOIE DE LA REQUETE AU SERVEUR **/
	//On récupère l'id de la file de message principale (Envoie des chaines de caractère au serveur)
	msg_id = msgget(MSGKEY, 0666);
	if (msg_id == -1) {
		perror("Le serveur n'est pas pret !");
		exit(-1);
	}
        //On affiche et on construit le message
     	printf("CLIENT %d: message : %s\n", getpid(), argv[1]);
	msg.type = 42;
	msg.pid = getpid();
	strncpy(msg.message, argv[1], TAILLE_MAX); //On utilise strNcpy qui permet de copier au maximum TAILLE_MAX caractère et donc permet d'éviter les débordements.
   
        //Envoi du message (et vérification de l'envoie)
        if (msgsnd(msg_id, &msg, sizeof(struct msg_struct), 0) == -1) {
       		perror("Impossible d'envoyer le message");
                exit(-1);
        }  
	
/** PARTIE ATTENTE PUIS RECEPTION DE LA REPONSE DU SERVEUR **/
	//On récupère l'id de la file par lequel le serveur va nous envoyer la réponse
	msg_id = msgget(getpid(), 0666|IPC_CREAT);
	if (msg_id == -1) {
		perror("Problème file de réponse !");
		exit(-1);
	}
        //Reception de la reponse
        msgrcv(msg_id, &msg, sizeof(struct msg_struct), 42, 0);
	//Affichage de la reponse
        printf("CLIENT: resultat recu depuis le serveur %d :\n\t %s\n", msg.pid, msg.message);
	//On supprime la file utilisé pour la réponse
	msgctl(msg_id, IPC_RMID, NULL);

        return 0;
}

