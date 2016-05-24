#include "segment_memoire.h"

int cree_segment(int taille, char* nom, int cle) {
	return (shmget(ftok(nom, cle), taille, 0666|IPC_CREAT);
}

int afficher_info_segment(int shmid) {
	struct shmid_ds tab;
	if (shmctl(shmid, IPC_STAT, &tab) == -1)
		return -1;
	printf("Cle : %d\n", tab.shm_perm.key);
	printf("UID proprietaire : %d\n", tab.shm_perm.uid);
	printf("Taille en octet : %d\n", tab.shm_segsz);
	printf("Heure dernier attachement : %d\n", tab.shm_atime);
	printf("PID createur : %d\n", tab.shm_cpid);
	printf("PID du dernier shmat/shmdt : %d", tab.shm_lpid);
	printf("Nombre d'attachements actuels : %d", tab.shm_nattch);
	return 0;
}


int detruire_segment(int shmid) {
	if (shmctl(shmid, IPC_RMID, NULL) == -1)
		return -1;
	else
		return 0;
}

