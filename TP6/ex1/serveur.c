/* Exercice 1 : Files IPC SERVEUR
Usage : ./serveur.e

Serveur de réaliation d'opérations algèbriques.
Reçoit des requètes de clients sous forme de messages de file IPC et y réponds de la même manière
la file de communication est instancié par le serveur et proprement détruite à la fin de son execution

Etudiants : Bertrand - Bruchon
*/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "calcul.h"

void raz_msg(int);                          /* routine de traitement des signaux */

int msg_id;                                     /* identificateur de la file de messages */

int main(int argc, char const *argv[])
{       
  struct msg_struct msg;
  int i_sig;
  //int result;

  /* liberer la zone de messages sur reception
     de n'importe quel signal */
  for (i_sig = 0; i_sig < NSIG; i_sig++)
  {
    signal(i_sig,raz_msg);//on place le comportement raz_msg àtous les signaux possibles
  }

  msg_id = msgget(MSGKEY,0666|IPC_CREAT); //création de la file
  if(msg_id == -1) {
    fprintf(stderr, "erreur msgget\n");
    exit(EXIT_FAILURE);
  }
  printf("SERVEUR: pret!\n");

  while(1)
  {
    /* reception */
    int nbread;
    //on attends un message d'un client
    if( (nbread = msgrcv(msg_id, &msg, sizeof(struct msg_struct)-sizeof(long), 1, 0)) == -1) {
      fprintf(stderr,"erreur type d'operation\n"); //si erreur on affiche un message à l'utilisateur
      raise(SIGINT); //on interrompt le programme proprement
    }
    printf("SERVEUR: reception d'une requete de la part de: %d\nLu %d bytes\n",msg.pidClient,nbread);
    switch(msg.operation) {//on selectionne la bonne operation et on effectue la mise à jour du message
      case '+': {
        msg.operande1 = msg.operande1 + msg.operande2;
        break;
      }
      case '-': {
        msg.operande1 = msg.operande1 - msg.operande2;
        break;
      }
      case '*': {
        msg.operande1 = msg.operande1 * msg.operande2;
        break;
      }
      case '/': {
        msg.operande1 = msg.operande1 / msg.operande2;
        break;
      }
      default: {
        fprintf(stderr,"erreur type d'operation\n");//en cas d'operation inconnu, on quite
        //remarque: au aurait pu répondre un message d'erreur spécial au client et continuer l'execution
        raise(SIGINT);
      }
    }
    msg.type = msg.pidClient;

    /* envoi de la reponse */
    msgsnd(msg_id, &msg, sizeof(struct msg_struct)-sizeof(long), 0);  
  }
  return 0;
}

void raz_msg(int signal)
{
  //struct msgid_ds tab;
  printf("Suppression de la file de message!\n");
  msgctl(msg_id, IPC_RMID, NULL);
  exit(0);
}
