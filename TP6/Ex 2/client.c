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
        int msg_id;
        struct msg_struct msg;

        if (argc != 2)
        {
                printf("Usage: %s chaine", argv[0]);
                exit(-1);
        }  

        /* ATTENTION : la file de messages doit avoir ete creee par
                 le serveur.
                 Il faudrait  tester la valeur de retour (msg_id) pour
                 verifier que cette creation a bien eu lieu.*/  
	msg_id = msgget(4224, IPC_CREAT | 666);
	if (msg_id == -1) {
		perror("Le serveur n'est pas pret !");
		exit(-1);
	}
        
     	printf("CLIENT %d: message : %s\n", getpid(), argv[1]);
	msg.type = 1;
	msg.pid = getpid();
	strcpy(msg.message, argv[1]);
   
        /* envoi du message */
        msgsnd(msg_id, &msg, sizeof(struct msg_struct), 0);

        /* reception de la reponse */
        //msgrcv(/* A COMPLETER */);

        //printf("CLIENT: resultat recu depuis le serveur %d : %d\n",
              //  /* A COMPLETER */);
        return 0;
}
