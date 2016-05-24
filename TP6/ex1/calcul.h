/* Exercice 1 : calcul.h

Structure des messages utilisés

Etudiants : Bertrand - Bruchon
*/

#ifndef __CALCUL_H
#define __CALCUL_H

#define MSGKEY 50            /* cle d'entree dans la table d'IPC (cf. ipcs) */

struct msg_struct                   /* structure d'un message */
{
  long type;
	  struct { //la structure imbriquée n'est pas obligatoire mais permet de marquer la différence entre les attributs utilisateurs et le champ type
      int pidClient;
      char operation;
      int operande1;
      int operande2;
    };
};

#endif
