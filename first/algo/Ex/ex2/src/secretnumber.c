#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
  unsigned int min = 0, max = 0, guesses, guess, secret;
  bool playing = false;

  do
  {
    printf("Chose the max number you will guess to: ");
    (void)scanf("%u", &max);
  } while (max <= min);

  srand((unsigned int)time(NULL));       /* Initialization, should only be called once. */
  secret = (unsigned int)(rand() % max); /* Returns a pseudo-random integer between 0 and max. */

  printf("Guess the secret number (between 0 and %u): ", max);
  playing = true;
  guesses = 0;

  while (playing)
  {
    (void)scanf("%u", &guess);

    if (guess > secret)
      printf("The secret number is smaller\n");
    if (guess < secret)
      printf("The secret number is bigger\n");

    if (guess == secret)
    {
      printf("You found the secret number!\n");
      playing = false;
    }

    guesses++;
  }

  printf("You finished the game in %u guesses.\n", guesses);
  return 0;
}