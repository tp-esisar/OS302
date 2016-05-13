#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define TAILLE_MAX 100

int main (int argc, char* argv[]) {
	char chemin[] = "/tmp/fifo";	
	int pipe;

	if (argc != 2) {
		perror("Usage : ./client.e \"Chaine à envoyer\"");
		exit(-1);
	}

	if((pipe = open (chemin, O_WRONLY)) == -1)
	{
		perror("Impossible d'ouvrir le tube. Le serveur est t-il bien lancé ?");
		exit(EXIT_FAILURE);
	} 
	
	write(pipe, argv[1], TAILLE_MAX);	

	close(pipe);

	return EXIT_SUCCESS;

}

