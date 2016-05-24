/* Exercice 1 : Files IPC CLIENT
Usage : ./client.e operande1 {+|-|*|/} operande2

le Client formule des requètes de calcul qui sont envoyé au serveur par une file IPC.
le Serveur y réponds par la même file.
Etudiants : Bertrand - Bruchon
*/


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "calcul.h"
#include <string.h>

int main(int argc, char const *argv[])
{
        int msg_id;
        struct msg_struct msg;

        if (argc != 4)
        {
                printf("Usage: %s  operande1 {+|-|*|/} operande2\n", argv[0]);
                exit(-1);
        }  

        /* il faut eviter la division par zero */
        if(strcmp(argv[2],"/")==0 && strcmp(argv[3],"0")==0) {
          fprintf(stderr,"division par 0!");
          exit(0);
        }
        
     
        /* ATTENTION : la file de messages doit avoir ete creee par
                 le serveur.
                 Il faudrait  tester la valeur de retour (msg_id) pour
                 verifier que cette creation a bien eu lieu.*/  
        msg_id = msgget(MSGKEY,0666);
        if(msg_id == -1) {
          fprintf(stderr, "erreur msgget\n");
          exit(EXIT_FAILURE);
        }
        
     printf("CLIENT %d: preparation du message contenant l'operation suivante: %d %c %d\n",
                getpid(), atoi(argv[1]), argv[2][0], atoi(argv[3]));
        
        /* On prepare un message de type 1 � envoyer au serveur
            avec les informations necessaires */        
        /* A COMPLETER */
        msg.operation = argv[2][0];
        msg.pidClient = getpid();
        msg.type = 1;
        msg.operande1 = atoi(argv[1]);
        msg.operande2 = atoi(argv[3]);
        
        
        /* envoi du message */
        int sndErr = msgsnd(msg_id, &msg, sizeof(struct msg_struct)-sizeof(long), 0);
        printf("Envoi du message au serveur, code retour: [%d]\n",sndErr);

        /* reception de la reponse */
        while(msgrcv(msg_id, &msg, sizeof(struct msg_struct)-sizeof(long), getpid(), 0) == -1);

        printf("CLIENT: resultat recu depuis le serveur : %d\n", msg.operande1);
        return 0;
}
