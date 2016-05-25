/* Fonctions de gestion des segment de mémoire partagé

Etudiants : Bertrand - Bruchon
*/

#include "segment_memoire.h"

int cree_segment(int taille, char* nom, int cle) {
	//On crée le segment avec shmget (on génère la clé avec ftok)
	return (shmget(ftok(nom, cle), taille, 0666|IPC_CREAT));
}

int afficher_info_segment(int shmid) {
	struct shmid_ds tab; //Structure qui contiendra les informations du SMP
	/* On récupère les informations sur le SMP avec shmctl et l'opération IPC_STAT*/
	if (shmctl(shmid, IPC_STAT, &tab) == -1) 
		return -1;
	printf("UID proprietaire : %d\n", tab.shm_perm.uid);
	printf("Taille en octet : %d\n", (int)tab.shm_segsz);
	printf("Heure dernier attachement : %d\n", (int)tab.shm_atime);
	printf("PID createur : %d\n", tab.shm_cpid);
	printf("PID du dernier shmat/shmdt : %d\n", tab.shm_lpid);
	printf("Nombre d'attachements actuels : %d\n", (int)tab.shm_nattch);
	return 0;
}


int detruire_segment(int shmid) {
	/*On supprime le segment avec shmctl et l'opération IPC_RMID*/
	if (shmctl(shmid, IPC_RMID, NULL) == -1)
		return -1;
	else
		return 0;
}

