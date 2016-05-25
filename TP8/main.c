#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "segment_memoire.h"

#define NB_FILS 6

int descendre = 0;

void descente (int i) {
	descendre = 0;
}

int main (int argc, char* argv[])
{
	pid_t fils[NB_FILS+1];
	pid_t pere = getpid();

	//Creation du segment de memoire partagee
	shmid = cree_segment(2*sizeof(pid_t),nom,42);
	if(shmid == -1) {
		fprintf(stderr,"erreur création segment");
		exit(EXIT_FAILURE);
	}

	//Création des fils
	//Remarque : Comme le fils sort du FOR directement après sa création, l'indice i correspond à son numéro de fils.
	printf("Naissance des fils\n");
	for (i=1; i<=NB_FILS; i++) {
		fils[i] = fork();
		if (fils[i] == 0)
			break;
	}

	// --- Partie du programme pour le PERE ---
	if (pere == getpid()) { 


	}

	// --- Partie du programme pour les FILS ---
	else {
		struct sembuf operation;
		int pid = getpid();
		sleep(1);

		signal(SIGUSR1,descente);
		
		operation.sem_num = 0;
		operation.sem_op = -1;
		operation.sem_flg = 0;
		if (semop(semid, &operation, 1) == -1) {
			fprintf(stderr,"erreur de semaphore");
			exit(EXIT_FAILURE);
		}
		printf("Fils %d : Je monte avec le numero %d", i, operation.sem_num);

		if ((mem = shmat(shmid,NULL,0)) == (char*)-1) {
			fprintf(stderr,"erreur ouverture segment");
			exit(EXIT_FAILURE);
		}
		memcpy(mem + (operation.sem_num+1)*sizeof(pid), &pid, sizeof(pid));
		printf("Fils %d : J'ai ecris mon pid %d", i, pid);
		shmdt(mem);
		
		while(descendre == 0);

		operation.sem_num = 0;
		operation.sem_op = 1;
		operation.sem_flg = 0;
		if (semop(semid, &operation, 1) == -1) {
			fprintf(stderr,"erreur de semaphore");
			exit(EXIT_FAILURE);
		}
		printf("Fils %d : Je descend", i);
		
		
		
		
		
		



		

	}
	
	return 0;
}
