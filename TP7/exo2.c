/*********************************************************************
 * OS302 - TP7 - Exercice 2                                          *
 * Utilisation des segments de memoire partagee                      *
 *********************************************************************/
#include <string.h>
#include <unistd.h>
#include "segment_memoire.h"
#include <sys/wait.h>

int main() {
  
  int pid;      // le PID du processus
  char *mem;    // pointeur vers le segment memoire
  int shmid;    // l'identificateur du segment memoire
  char* nom = "exo2.c";
  char* message = "youhouuu~!";
  //Creation du segment de memoire partagee
  shmid = cree_segment(100,nom,42);
  if(shmid == -1) {
    fprintf(stderr,"erreur création segment");
    exit(EXIT_FAILURE);
  }
  
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
afficher_info_segment(shmid);
    
  }
  else {
    // je suis le pere!
    // attachement et ecriture sur le segment de memoire partagee
    printf("Pere: ecriture dans le segment partagé:\n");
    mem = shmat(shmid,NULL,0);
    memcpy(mem,message,strlen(message)*sizeof(char));
    //attendre la fin du fils + detacher le segment et le detruire
    wait(NULL);
	
    detruire_segment(shmid);
    
    
    // ...
  }  

  return 0 ;
}
