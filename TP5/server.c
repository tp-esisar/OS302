/* Exercice 2 : Communication client / serveur par PIPE
Usage (sans arguments) : ./server.e

Réalisation d'un modèle client/serveur avec deux processus en utilisant un tube nommé
pour la Communication, ainsi que les fonctions read et write

Etudiants : Bertrand - Bruchon
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define TAILLE_MAX 100 //Taille maximale du tableau où l'on met le contenu du pipe

int main (void) {
	char chemin[] = "/tmp/fifo1";	//Chemin d'accès au pipe
	char chaine[TAILLE_MAX] = "";	//Chaine pour récupèrer le contenu du pipe
	int pipe; //Référence au Pipe


	//On crée le pipe s'il n'existe pas
	if(mkfifo(chemin, 0666) == -1)
		printf("Tube existant \n");
	else
		printf("Tube crée \n");

	//On ouvre le pipe en lecture uniquement
	//Remarque : On va resté bloquer à cette instruction jusqu'à l'arrivée du premier client (écrivain)
	if((pipe = open (chemin, O_RDONLY)) == -1)
	{
		perror("Impossible d'ouvrir la sortie du tube nommé.");
		exit(EXIT_FAILURE);
	} 
	printf("Tube ouvert en lecture \n");
	
	//Boucle principale du serveur qui lit en permanance ce qu'il y a dans le tube et l'affiche
	//Dès que l'on voit le mot "fin" le serveur s'arrête.
	do {
		if (read(pipe, chaine, TAILLE_MAX) != 0)
			printf(">>> %s\n", chaine);
	} while (strcmp(chaine, "fin") != 0);	

	printf("Supression du tube \n");
	close(pipe); //Ferme notre connexion au tube
	unlink(chemin); //On supprime le tube

	return EXIT_SUCCESS;

}

