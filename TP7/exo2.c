/*********************************************************************
 * OS302 - TP7 - Exercice 2                                          *
 * Utilisation des segments de memoire partagee                      *
 *********************************************************************/
/*
Exercice 2
Usage: ./exo2.e

Créé un segment partagé et des processus père et fils.
Le père écrit un message dans le segment partagé, et le fils l'affiche à l'écran.

*/
#include <string.h>
#include <unistd.h>
#include "segment_memoire.h"

int main() {
  
  int pid;      // le PID du processus
  char *mem;    // pointeur vers le segment memoire
  int shmid;    // l'identificateur du segment memoire
  char *nom = (char *) malloc(15*sizeof(char));
  nom = "exo2.c";
  char* message = "youhouuu~!\n"
  // a completer : creation du segment de memoire partagee
  shmid = cree_segment(100,nom,42);
  if(shmid == -1) {
    fprintf(stderr,"erreur création segment");
    exit(EXIT_FAILURE);
  }
  afficher_info_segment(shmid);
  // creation du processus fils
  pid = fork();
  if (pid == -1) {
    perror("impossible de creer un processu fils!");
    exit(-1);
  }
  else if (pid == 0) {
    sleep(2);
    // je suis le fils!
    // a completer : s'attacher au segment et affichage de son contenu
    mem = shmat(shmid,NULL,0);
    printf("Fils: affichage du segment partagé: \"%s\"\n",mem);
    
  }
  else {
    // je suis le pere!
    // a completer : attachement et ecriture sur le segment de memoire partagee
    printf("Pere: ecriture dans le segment partagé:\n");
    mem = shmat(shmid,NULL,0);
    memcpy(mem,message,strlen(message)*sizeof(char));
    // a completer : attendre la fin du fils + detacher le segment et le detruire
    int* tab;
    wait(tab);
    detruire_segment(shmid);
    
    // ...
  }
        
        
  free(nom);  
  return 0 ;
}
