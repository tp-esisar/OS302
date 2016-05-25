#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define NB_FILS 6

struct 

int main (int argc, char* argv[])
{
	pid_t fils[NB_FILS];
	pid_t pere = getpid();
	char* nom = "main.c";
	pid_t *mem;    					// pointeur vers le segment memoire
	key_t cle;
	if((cle = ftok(nom,42)) == -1) {
		printf("Erreur de ftok\n");
		exit(EXIT_FAILURE);
	}
  	int semid;
  	if((semid = semget(cle,1,IPC_CREAT|0666)) == -1) {
  		printf("Erreur de semget\n");
  		exit(EXIT_FAILURE);
  	}
  	if(semctl(semid,0,SETVAL,2) == -1) {
  		printf("Erreur SETVAL à 2\n");
  		exit(EXIT_FAILURE);
  	}
	//Création des fils
	//Remarque : Comme le fils sort du FOR directement après sa création, l'indice i correspond à son numéro de fils.
	printf("Naissance des fils\n");
	for (i=0; i<NB_FILS; i++) {
		fils[i] = fork();
		if (fils[i] == 0)
			break;
	}
	
	// --- Partie du programme pour le PERE ---
	if (pere == getpid()) { 
		mem = shmat(shmid,NULL,SHM_RDONLY);
		int i = 0;
		while(i<NB_FILS) {
			sleep(1);
			int nbCient = 2 - semctl(semid,0,GETVAL);
			i+=nbClient
			if(nbClient > 0) {
				//on monte
				if(nbClient == 1) {
					kill(mem[1],SIGUSR1);
				}
				else if(nbCient == 2) {
					kill(mem[1],SIGUSR1);
					kill(mem[0],SIGUSR1);
				}
			}
			
			
		}
		shmdt(mem);
		detruire_segment(shmid);
		semctl(semid,0,IPC_RMID);
	}

	// --- Partie du programme pour les FILS ---
	else {
		
		
		
		
		
	}
	
	return 0;
}
