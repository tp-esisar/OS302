/* Exercice 3 : Roulette Russe
Usage : Lancement du programme sans argument  

Dès qu'un fils a le même numéro que celui lu dans le fichier, il meurt.
Le fichier est déjà crée et rempli auparavant : file

Etudiants : Bertrand - Bruchon
*/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define NB_FILS 6

int fin_jeu = 0;
void rien (int i){}

void terminerJeu(int i) {
  fin_jeu = 1;
}

int lire_valeur(const char *path) //Fonction pour lire dans le fichier
{
   FILE* fichier = NULL;
   char chaine[5] = ""; // Chaîne vide de taille TAILLE_MAX
   int valeur = -1;
   fichier = fopen(path, "r");
   if (fichier != NULL)
   {
     fgets(chaine, 3, fichier); // On lit au maximum TAILLE_MAX
     valeur = atoi(chaine);
     fclose(fichier);
   }
   return valeur;
} 


int main(void) {
  int i, valeur;
  pid_t fils[NB_FILS+1];
  pid_t pere = getpid();

  signal(SIGUSR1,rien);
  signal(SIGUSR2,terminerJeu);

  printf("Naissance des fils\n");
  for (i=1; i<=NB_FILS; i++) {
    fils[i] = fork();
    if (fils[i] == 0)
      break;
  }
  
  if (pere == getpid()) {
    int i=1;
    sleep(1);
    printf("Démarrage de la partie\n");
    while (!fin_jeu && i<=NB_FILS) {
      printf("Fils %d, a toi de jouer !\n", i);
      while(kill(fils[i],SIGUSR1) != 0);
      i++;
      pause();
    }
    printf("Le fils %d est mort, je vous libére ...\n", i-1);
    for (i=1; i<=NB_FILS; i++) 
      kill(fils[i],SIGUSR2);
  }
  
  else {
    while(!fin_jeu) {
      pause();
      if(!fin_jeu) {
        valeur = lire_valeur("file");
        printf("Je suis le fils %d et j'ai lu le numéro %d\n", i, valeur);
        if (valeur == i) {
          printf("-> BAM! Aarrghh!!!...\n");
          while(kill(pere,SIGUSR2) != 0);
          exit(-1);
        }
          
        else
          while(kill(pere,SIGUSR1) != 0);
      }
    }
    
    printf("Fils %d : Je suis libre !!!\n", i);
  }
  return 0;
  
}
