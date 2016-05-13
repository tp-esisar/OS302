#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


int main(int argc, char* argv[]) {
  if(argc<3) {
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