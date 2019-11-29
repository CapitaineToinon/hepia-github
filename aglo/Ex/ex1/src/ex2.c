#include <stdio.h>

int main()
{
  int i;  /* i : variable de type entier */
  int j;  /* j : variable de type entier */
  i = 22; /* i vaut 22 */
  j = i;  /* on recopie la valeur de i dans j */
  /* donc j vaut aussi 22 à présent */
  printf("i vaut %d\n", i);       /* Affiche la valeur de i */
  printf("i+j vaut %d\n", i + j); /* Affiche la valeur de i+j */

  return 0;
}