#include <stdio.h>

int main()
{
  int age = 0; /* On initialise la variable à 0 */
  printf("Quel age avez-vous ? ");
  (void)scanf("%d", &age); /* On demande d'entrer l'age avec scanf */
  printf("Ah ! Vous avez donc %d ans ! \n\n", age);
  return 0;
}