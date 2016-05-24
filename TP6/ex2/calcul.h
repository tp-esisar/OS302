/* Exercice 2 : Communication par files de messages

Fichier qui contient les déclaration communes aux 2 programmes (client et serveur)
Permet de définir la clé de départ, et le format des message.

Etudiants : Bertrand - Bruchon
*/

#ifndef __CALCUL_H
#define __CALCUL_H

#define MSGKEY 50 			/* cle d'entree dans la table d'IPC */
#define TAILLE_MAX 100

struct msg_struct 			/* structure d'un message */
{
	long type;         		//Le type du message
	pid_t pid;			//Le pid du processus qui envoie le message
	char message[TAILLE_MAX];	//La chaine de caracère à envoyer
};

#endif
