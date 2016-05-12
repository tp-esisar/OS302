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

/* REMARQUE IMPORTANTE :
 * Le père envoie aux fils :  SIGUSER1 -> Pour annoncer au fils qu'il doit jouer
 *                            SIGUSER2 -> Pour annoncer au fils qu'il est libre
 * Le fils envoir au père :   SIGUSER1 -> Pour annoncer au père qu'il est encore vivant (car le numéro ne correspond pas)
 *                            SIGUSER2 -> Pour annoncer au père qu'il va mourrir !
 */

void rien (int i){}

/* La variable fin_jeu est initialisée à 0 au début du programme
 * Tant que qu'aucun fils n'est mort, elle reste à 0.
 * Dès qu'un fils meure, il envoie le signal SIGUSR2, ce qui passe cette variable à 1 pour le père
 * Le père va alors envoyer à tout ces FILS le signal SIGUSR2 afin que pour eux aussi cette variable passe à 1
 * Ils vont donc tous pouvoir sortir de la boucle d'attente principale
 * La fonction terminerJeu permet de faire ce changement à la reception du signal SIGUSER2 
*/

int fin_jeu = 0;

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

  //Définition des comportements
  signal(SIGUSR1,rien);
  signal(SIGUSR2,terminerJeu);

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
    int i=1;
    sleep(1);
    printf("Démarrage de la partie\n");

    //Boucle principale du jeu
    while (!fin_jeu && i<=NB_FILS) { 
      printf("Fils %d, a toi de jouer !\n", i);
      while(kill(fils[i],SIGUSR1) != 0); //On signale au fils qu'il doit jouer
      i++;
      pause(); //On attend la réponse du fils
    }

    printf("Le fils %d est mort, je vous libére ...\n", i-1);
    for (i=1; i<=NB_FILS; i++) //Libération de tout les fils
      kill(fils[i],SIGUSR2); 
  }
  
  // --- Partie du programme pour les FILS ---
  else {
    while(!fin_jeu) { //On reste dans la boucle tant que l'on est pas libre
      pause(); //Mise en pause du processus fils pour attendre qu'il joue
      if(!fin_jeu) {
        valeur = lire_valeur("file"); //Lecture du nombre dans le fichier
        printf("Je suis le fils %d et j'ai lu le numéro %d\n", i, valeur);
        if (valeur == i) { //Si le nombre correspond à notre numéro, on meure
          printf("-> BAM! Aarrghh!!!...\n");
          while(kill(pere,SIGUSR2) != 0); //Annonce au père de notre mort
          exit(-1); //On meurt !
        }
          
        else //Sinon on reste en vie
          while(kill(pere,SIGUSR1) != 0); //Et on dit au père que l'on est encore en vie
      }
    }
    
    printf("Fils %d : Je suis libre !!!\n", i);
  }
  return 0;
}

