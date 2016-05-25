#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define NB_FILS 6

int main (int argc, char* argv[])
{
	pid_t fils[NB_FILS+1];
	pid_t pere = getpid();

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





	}
	
	return 0;
}
