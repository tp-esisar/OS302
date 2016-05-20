#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "calcul.h"


void raz_msg(int);                          /* routine de traitement des signaux */

int msg_id;                                     /* identificateur de la file de messages */

int main(int argc, char const *argv[])
{       
        struct msg_struct msg;

        int i_sig;

        /* liberer la zone de messages sur reception
           de n'importe quel signal */
        for (i_sig = 0; i_sig < NSIG; i_sig++)
        {
             signal(i_sig, raz_msg);
        }

        msg_id = msgget(4224, IPC_CREAT | 666);
	if (msg_id == -1)  {
		perror("File de message");
		exit(-1);
	}
        printf("SERVEUR: pret!\n");

        while(1)
        {
             int cle2, i, msg_rep;
	     /* reception */
             if (msgrcv(msg_id, &msg, sizeof(struct msg_struct), 0,0) != -1) {
		     printf("SERVEUR: reception d'une requete de la part de: %d\n", msg.pid);
	 
		     for (i=0; i<strlen(msg.message); i++)
			msg.message[i] = toupper(msg.message[i]);

		     cle2 = ftok("serveur.c", msg.pid);
		     msg_rep = msgget(cle2, IPC_CREAT | 666);
		     msgsnd(msg_rep, &msg, sizeof(struct msg_struct), 0);  
		     close(msg_rep);
	     }
        }
        return 0;

}

void raz_msg(int signal)
{
        printf("Suppression de la file de message!\n");
        msgctl(msg_id, IPC_RMID, NULL);
        exit(0);
}
