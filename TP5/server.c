#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define TAILLE_MAX 100

int main (void) {
	char chemin[] = "/tmp/fifo";	
	char chaine[TAILLE_MAX] = "";	
	int pipe;

	if(mkfifo(chemin, 0666) == -1)
		printf("Tube existant \n");
	else
		printf("Tube crée \n");

	if((pipe = open (chemin, O_RDONLY)) == -1)
	{
		perror("Impossible d'ouvrir la sortie du tube nommé.");
		exit(EXIT_FAILURE);
	} 
	printf("Tube ouvert en lecture \n");
	
	do {
		if (read(pipe, chaine, TAILLE_MAX) != 0)
			printf(">>> %s\n", chaine);
	} while (strcmp(chaine, "fin") != 0);	

	printf("Supression du tube \n");
	close(pipe);
	unlink(chemin);

	return EXIT_SUCCESS;

}

