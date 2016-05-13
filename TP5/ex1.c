/* Exercice 1 : Communication entre processus père et fils et redirection des E/S 
Usage : (exemple) ./ex1.e "ls" "grep ex"

Recréation du comportement de l'opérateur | du shell,
création d'un fils, et modification du stdin et stdout père/fils via un pipe
pour chainer les deux processus

Etudiants : Bertrand - Bruchon
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char* argv[]) {
  if(argc<3) { //verification des arguments
    fprintf(stderr,"Usage: ./ex1.e cmd1 cmd2\n");
    exit(42);
  }
  int p[2];
  pipe(p);
  if(fork() == 0) {
    //fils
    dup2(p[0],STDIN_FILENO);
    system(argv[2]);
  }
  else {
    //père
    dup2(p[1],STDOUT_FILENO);
    system(argv[1]);
  }
  
    
  return 0;
}