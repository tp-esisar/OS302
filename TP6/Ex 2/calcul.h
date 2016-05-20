#ifndef __CALCUL_H
#define __CALCUL_H

#define MSGKEY 50            /* cle d'entree dans la table d'IPC (cf. ipcs) */
#define TAILLE_MAX 100

struct msg_struct                   /* structure d'un message */
{
	long type;         
	pid_t pid;
	char message[100];
};

#endif
