#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>
#include "segment_memoire.h"

#define NB_FILS 6

int descendre = 0;

void descente (int i) {
	descendre = 0;
}


int main (int argc, char* argv[])
{
	pid_t fils[NB_FILS];
	pid_t pere = getpid();
	int semid;
	char* nom = "main.c";
	pid_t *mem;    					// pointeur vers le segment memoire
	key_t cle;
	int shmid;
	int i;

	//Creation du segment de memoire partagee
	shmid = cree_segment(2*sizeof(pid_t),nom,42);
	if(shmid == -1) {
		fprintf(stderr,"erreur création segment");
		exit(EXIT_FAILURE);
	}

	if((cle = ftok(nom,42)) == -1) {
		printf("Erreur de ftok\n");
		exit(EXIT_FAILURE);
	}
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
		i = 0;
		while(i<NB_FILS) {
			sleep(1);
			int nbClient = 2 - semctl(semid,0,GETVAL);
			i+=nbClient;
			if(nbClient > 0) {
				//on monte
				if(nbClient == 1) {
					kill(mem[1],SIGUSR1);
				}
				else if(nbClient == 2) {
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
		struct sembuf operation;
		int pid = getpid();
		int numero;
		
		signal(SIGUSR1,descente);
		sleep(3);
				
		operation.sem_num = 0;
		operation.sem_op = -1;
		operation.sem_flg = 0;
		if ((numero = semop(semid, &operation, 1)) == -1) {
			perror("erreur de semaphore\n");
			exit(EXIT_FAILURE);
		}
		//numero = semctl(semid, 0, GETVAL);
		printf("Fils %d : Je monte avec le numero %d\n", i, numero);

		if ((mem = shmat(shmid,NULL,0)) == (pid_t*)-1) {
			fprintf(stderr,"erreur ouverture segment");
			exit(EXIT_FAILURE);
		}
		memcpy(mem + numero*sizeof(pid), &pid, sizeof(pid));
		printf("Fils %d : J'ai ecris mon pid %d\n", i, pid);
		shmdt(mem);
		
		/*while(descendre == 0);

		operation.sem_num = 0;
		operation.sem_op = 1;
		operation.sem_flg = 0;
		if (semop(semid, &operation, 1) == -1) {
			fprintf(stderr,"erreur de semaphore");
			exit(EXIT_FAILURE);
		}*/
		printf("Fils %d : Je descend\n", i);
	}
	
	return 0;
}
