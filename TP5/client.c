/* Exercice 2 : Tubes nommés (FIFO) CLIENT
Usage : (exemple) ./client.e "j'aime les rascasses"
									./client.e "fin" //ferme le serveur proprement

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

#define TAILLE_MAX 100 //taille maximum du buffer d'ecriture

int main (int argc, char* argv[]) {
	char chemin[] = "/tmp/fifo1";	//chemin du fifo temporaire 
	//le 1 sert à le différencier des potentiels autres fichiers des autres groupes sur le PC de test du prof
	//pour eviter tout problème de droit si les autres groupes oublient de le supprimer avec unlink.
	int pipe;

	if (argc != 2) {
		perror("Usage : ./client.e \"Chaine à envoyer\"\n\"fin\" pour fermer le serveur (le pipe est /tmp/fifo1)");
		exit(-1);
	}

//Ouverture du Pipe en mode écriture (opération non bloquante)
	if((pipe = open (chemin, O_WRONLY|O_NDELAY)) == -1)
	{
		perror("Impossible d'ouvrir le tube. Le serveur est t-il bien lancé ?");
		exit(EXIT_FAILURE);
	} 
	
	write(pipe, argv[1], TAILLE_MAX);	

	close(pipe);

	return EXIT_SUCCESS;

}

