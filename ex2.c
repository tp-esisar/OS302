#include <stdlib.h>
#include <stdio.h>

int compteur=0;

void increment (int id) {
	compteur++;
}

int main (void)
{
	pid_t pid;

	pid = fork();
	if (pid == -1) {
		perror ("Erreur de fork");
		exit(2);
	}
	else if (pid == 0) {
	  	pid = fork();
		if (pid == -1) {
		  perror ("Erreur de fork");
		  exit(2);
		}

		else if (pid == 0) {
		  signal(SIGALRM, fonction(increment));
		  while(1) {
		    if (compteur == 60){
		      compteur = 0;
		      kill(getppid(), SIGALRM);
		    }  
		  }

		  else {
		    signal(SIGALRM, fonction(increment));
		    while(1) {		 
		      if (compteur == 60){
			compteur = 0;
			kill(getppid(), SIGALRM);
		      }
		    }
		  }
	}
	else {
	  signal(SIGALRM, fonction(increment));
	  while(1);
	}
}
