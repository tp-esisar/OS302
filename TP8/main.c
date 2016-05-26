/* TP8 : Syncronisation par sémaphore
Usage (sans arguments) : ./main.e

Ce programme permet de simuler un ascenseur (de 2 personnes) sachant que N ouvriers veulent monter.
Les ouvriers sont représenté par des processus fils et le père gère l'ascenseur.

Pour cela nous avons utilisé un sémaphore de valeur 2, qui ne laisse donc passer que 2 processus fils.
Afin de savoir qui est monté, on utilise un segment de mémoire partagé sur lequel les 2 clients dans l'ascenseur vont pour noter leur pid.
Le père pourra alors leur envoyer un signal via ce pid.
Problème : Comme ils sont 2, comment ne pas écrire au même endroit dans le SMP. Pour cela nous avons utiliser un second SMP d'une valeur de 1.  
	Le premier qui décroche ce sémaphore écrit dans la première case du SMP 
	Le second ne pourra pas avoir ce semaphore, et comme l'opération est non bloquante, il saura qu'il faut écrire dans la seconde case mémoire.

Afin de simuler les durées déplacements et de respecter l'ordre des montées et des descentes, nous avons utiliser des "sleep".
Pour les SMP, nous avons utilisés les fonctions du TP précédent.
Pour les sémaphores nous avons défini une fonction permettant de réaliser (de manière sécurée) les opérations sur les sémaphores

Etudiants : Bertrand - Bruchon
*/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>
#include "segment_memoire.h"

#define NB_FILS 7			//Constante définissant le nombre d'ouvrier à créer

int descendre = 0;			//Variable globale qui permet d'indiquer a l'ouvrier quand il peut descendre de l'ascenseur.
void descente (int i) {			//Comportement à adopter si l'on recoit le signal SIGUSR1 : On peut descendre de l'ascenseur.
	descendre = 1;			//On modifie la valeur de la variable descendre qui fera sortir le programme de sa boucle d'attente.
}

/*Fonction permettant d'effectuer les opérations sur les sémaphores*/
//Elle prend en paramètre le contenu de cette opération puis la réalise et vérifie qu'elle se soit bien déroulée.
//S'il y a une erreur, le programme se ferme, hormis qui l'opération et demandée non bloquante (flag = IPC_NOWAIT).
int sem_op(int semid, int num, int op, int flag) {
	struct sembuf operation;
	operation.sem_num = num;
	operation.sem_op = op;
	operation.sem_flg = flag;
	if (semop(semid, &operation, 1) == -1) {
		if (flag == IPC_NOWAIT)
			return -1;
		fprintf(stderr,"erreur de semaphore");
		exit(EXIT_FAILURE);
	}
	return 0;
}

int main (int argc, char* argv[])
{
	pid_t fils[NB_FILS], pere = getpid(); 		//Variable qui stocke le pid de chaque processus
	int semid, shmid;				//Identifiant des semaphores et segments de mémoire partagé
	char* nom = "main.c";				//Chaine de caractère pour générer la clé
	key_t cle;					//La clé qui permet de générer l'id du semaphore
	pid_t *mem;    					//Pointeur vers le segment memoire partagé (qui contiendra la pid des fils)
	int i;

	/**** Creation du segment de memoire partagee ****/
	shmid = cree_segment(2*sizeof(pid_t),nom,42);
	if(shmid == -1) {
		fprintf(stderr,"erreur création segment");
		exit(EXIT_FAILURE);
	}

	/**** Création de l'ensemble des 2 semaphores ****/
	if((cle = ftok(nom,44)) == -1) {		//Génération de la clé
		printf("Erreur de ftok\n");
		exit(EXIT_FAILURE);
	}
  	if((semid = semget(cle,2,IPC_CREAT|0666)) == -1) {	//Création de l'ensemble de semaphore
  		perror("Erreur de semget");
  		exit(EXIT_FAILURE);
  	}
  	if(semctl(semid,0,SETVAL,2) == -1) {		//On défini pour l'ensemble 0 la valeur 2 pour le semaphore 
  		printf("Erreur SETVAL à 2\n");		//Permet de régurer le nombre de client dans l'ascenseur
  		exit(EXIT_FAILURE);
  	}
	if(semctl(semid,1,SETVAL,1) == -1) {		//On défini pour l'ensemble 1 la valeur 1 pour le semaphore 
  		printf("Erreur SETVAL à 1\n");		//Permet de gérer les écriture sur le segment de mémoire partagé
  		exit(EXIT_FAILURE);
  	}

	/**** Création des fils ****/
	printf("Naissance des fils\n");
	for (i=0; i<NB_FILS; i++) {
		fils[i] = fork();
		if (fils[i] == 0)
			break; //Comme le fils sort du FOR directement après sa création, l'indice i correspond à son numéro de fils.
	}
	
/********** Partie du programme pour le PERE **********/
	if (pere == getpid()) { 
		signal(SIGUSR1,SIG_IGN);		//On ignore le signal SIGUSR1
		/**** On se lie au segment de mémoire partagé en lecture seule***/
		if ((mem = shmat(shmid,NULL,SHM_RDONLY)) == (pid_t*)-1) {
			perror("erreur ouverture segment");
			exit(EXIT_FAILURE);
		}
		i = 0;
		printf("Je suis le pere, pid : %d\n\n", getpid());

		/**** Boucle qui attend que tout les fils soient monté avec de fermer la programme ****/
		while(i<NB_FILS) {
			sleep(1);
			int nbClient = 2 - semctl(semid,0,GETVAL); //On récupère le nombre de client qui sont montés
			i+=nbClient;		//Et on incérmente le nombre de client total

			/**** Montée de l'ascenseur ****/
			if(nbClient > 0) {
				printf("===== L'ascenseur monte ! ==========");
				if(nbClient == 1) { //Si on a qu'un seul client dans l'ascenseur, on ne fait descendre que lui
					printf("===> Le pid %d peut descendre \n", mem[0]);
					while(kill(mem[0],SIGUSR1)==-1);
				}
				else if(nbClient == 2) { //Sinon on fait descendre les 2
					printf("===> Les pids %d et %d peuvent descendre\n", mem[1], mem[0]);
					while(kill(mem[0],SIGUSR1)==-1);
					while(kill(mem[1],SIGUSR1)==-1);
				}
				sleep(1);
				printf("===== L'ascenseur redescend ! ==========\n");
				sleep(2);
			}
			
			
			
		}
		sleep(1);
		
		/**** Libération des SMP et de l'ensemble de sémaphore avant de terminer le programme ****/
		shmdt(mem);
		detruire_segment(shmid);
		semctl(semid,0,IPC_RMID);
	}

/********** Partie du programme pour les FILS **********/
	else {
		int ordre=1, pid = getpid();
		sleep(3); 						//Syncronisation avec le père
			
		/**** On demande le sémaphore pour monter dans l'ascenseur ***/
		//Cette opération est bloquante, tant que l'on ne dispose pas du sémaphore.
		sem_op(semid,0, -1, 0);	 				//On fait une opération P, pour prendre le sémaphore
		printf("----------- Fils %d ----------> \t", i);	//On monte dans l'ascenseur
		signal(SIGUSR1,descente); //Définition du comportement pour le signal SIGUSR1 : appeler la fonction qui permet de descendre

		/**** On se lie au segment de mémoire partagé ***/
		if ((mem = shmat(shmid,NULL,0)) == (pid_t*)-1) {
			perror("erreur ouverture segment");
			exit(EXIT_FAILURE);
		}
		//On utilise le second sémaphore pour savoir si on pu écrire notre pid dans la première case ou dans la seconde.
		if (sem_op(semid,1, -1, IPC_NOWAIT)!=-1)
			memcpy(&(mem[0]), &pid, sizeof(pid));
		else {
			memcpy(&(mem[1]), &pid, sizeof(pid));
			ordre=2;
		}
		printf("%d\n", pid); 					//Affichage du pid du processus
		shmdt(mem);						//Détachement du SMP
		
		while(descendre == 0);					//On attend que l'ascenseur soit arrivé en haut pour descendre

		printf("<---------- Fils %d ----------- \n", i);	//On descend

		if (ordre == 1)						//Si on a pris le semaphore pour écrire dans le 1er emplacement de SMP
			sem_op(semid,1, 1, 0);				// on le rend.
		sleep(1);
		sem_op(semid,0, 1, 0);					//On libère le semaphore de l'ascenseur
		
	}
	
	return 0;
}
