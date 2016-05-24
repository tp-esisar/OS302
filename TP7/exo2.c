/*********************************************************************
 * OS302 - TP7 - Exercice 2                                          *
 * Utilisation des segments de memoire partagee                      *
 *********************************************************************/
/*
Usage (sans argument) : ./exo2.e

Créé un segment partagé entre des processus père et fils.
Le père écrit un message dans le segment partagé, et le fils l'affiche à l'écran.

Etudiants : Bertrand - Bruchon
*/

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "segment_memoire.h"

int main() {
  
  int pid;      					// le PID du processus
  char *mem;    					// pointeur vers le segment memoire
  int shmid;    					// l'identificateur du segment memoire
  char* nom = "exo2.c";				// Nom de fichier pour générer la clé
  char* message = "youhouuu~!";		// Message à partager

  //Creation du segment de memoire partagee
  shmid = cree_segment(100,nom,42); //Création d'un segment partagé d'une taille de 100 octets
  if(shmid == -1) {
    fprintf(stderr,"erreur création segment");
    exit(EXIT_FAILURE);
  }
  
  //Creation du processus fils
  pid = fork();
  if (pid == -1) {
    perror("impossible de creer un processu fils!");
    exit(-1);
  }

  else if (pid == 0) { // Je suis le fils !
    sleep(2);
    mem = shmat(shmid,NULL,SHM_RDONLY); //S'attacher au segment partagé en lecture seulement
    printf("Fils: affichage du segment partagé: \"%s\"\n\nInfo du SMP fils :\n",mem); //Affichage de son contenu	
	afficher_info_segment(shmid); //Affichage des informations du segment partagé
	shmdt(mem);
  }

  else { // Je suis le pere !
    printf("Pere: ecriture dans le segment partagé : %s\n", message);
	//Attachement et ecriture sur le segment de memoire partagee
    mem = shmat(shmid,NULL,0);
    memcpy(mem,message,strlen(message)*sizeof(char));
    //Attendre la fin du fils + detacher le segment et le detruire
    wait(NULL);
	printf("\nMon fils est mort !\n\nInfos du SMP pere\n");
	afficher_info_segment(shmid); //Affichage des informations du segment partagé
	shmdt(mem);
    detruire_segment(shmid);
  }  

  return 0 ;
}

