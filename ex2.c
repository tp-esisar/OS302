/* Exercice 2 : Horloge
Usage : Lancement du programme sans argument  

Création de 3 processus, un qui gère les heures, l'autre les minutes et pour le finir les secondes.

Etudiants : Bertrand - Bruchon
*/


#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void increment_sec (int id) { //Fonction qui s'activera à la réception d'un signal pour incrémenter les secondes
  static int compteur = 0;
  compteur++;
  if (compteur == 60){
    compteur = 0;
    kill(getppid(), SIGALRM);
  }  
    printf("----%d\n", compteur);
  }

void increment_min (int id) { //Fonction qui s'activera à la réception d'un signal pour incrémenter les minutes
  static int compteur = 0;
  compteur++;
  if (compteur == 60){
    compteur = 0;
  kill(getppid(), SIGALRM);
  }
  printf("--%d\n", compteur);
}

void increment_h (int id) { //Fonction qui s'activera à la réception d'un signal pour incrémenter le heures
  static int compteur = 0;
  compteur++;
  printf("%d\n", compteur);
}

/** Remarque : On aura pu utilise une seule fonction increment mais cela compliquait l'affichage du temps (on devrait utiliser des  globales**/


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
      signal(SIGALRM, increment_sec);
      while(1) {
	alarm(1);
	sleep(1);
      }
    }

    else { //Comptage des minutes 
      signal(SIGALRM, increment_min);
      while(1);
    }
  }
  else { //Comptage des heures
    signal(SIGALRM, increment_h);
    while(1);
  }
}
