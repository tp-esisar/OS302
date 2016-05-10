#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int compteur=0;

void increment (int id) {
  compteur++;
  printf("%d\n", compteur);
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

    else if (pid == 0) { //Comptage des secondes
      signal(SIGALRM, increment);
      while(1) {
	alarm(1);
	sleep(1);
	if (compteur == 60){
	  compteur = 0;
	  kill(getppid(), SIGALRM);
	}  
      }
    }

    else { //Comptage des minutes 
      signal(SIGALRM, increment);
      while(1) {		 
	if (compteur == 60){
	  compteur = 0;
	  kill(getppid(), SIGALRM);
	}
      }
    }
  }
  else { //Comptage des heures
    signal(SIGALRM, increment);
    while(1);
  }
}
