/* Exercice 1 : Syncronisation Père / Fils par des signaux
Usage : Lancement du programme sans argument  

Création de 2 processus, un qui compte les nombre père et un les impairs

Etudiants : Bertrand - Bruchon
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int pidFils; //permet au père de garder une trace du pid du fils pour lui envoyer un signal dans sa fonction de callback
int count; //compteur

void cbPere(int i) { //callback du père, affiche le compteur, l'incrémente de 2 et lance de callback du fils via un signal
  printf("pere: %d\n", count);
  count +=2;
  kill(pidFils,SIGUSR1);  
}

void cbFils(int i) { //callback du fils, même fonctionnement mais on verifie en plus que le compteur ne dépasse pas 100
  printf("fils: %d\n", count);
  count+=2;
  if(count <= 100)
    kill(getppid(),SIGUSR1);
  
}

int main() {
  int k = fork(); //Création du 2nd processus
  if (k == -1) {
    printf("fail de fork\n");
    exit(42);
  }
  if(k != 0) { //Si on est le père, on configure son comportement
    printf("init du père\n");
    signal(SIGUSR1,cbPere);
    pidFils = k;
    count = 1;
    sleep(1);
    kill(pidFils,SIGUSR1);
  }
  else { //Si on est le fils, on configure son comportement
    printf("init du fils\n");
    signal(SIGUSR1,cbFils);
    count = 0;
    
  }

  while(count < 100); //Boucle d'attente d'arrivée à 100

  return 0;
}
