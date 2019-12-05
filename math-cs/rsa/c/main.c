#include <stdio.h>
#include <time.h>
#include "rsa.h"

#define TESTING_LENGTH 10000

int main()
{
  uint64_t n = 1190836873;
  uint64_t e = 1051;
  u_int64_t groups[] = {
      139625027,
      728808256,
      677396451,
      662265473,
      845995352,
      613303937,
      1033970589,
      623160996,
      1160483016,
      243815344,
      572050792,
      495107014,
      909878795,
      367266109,
      273461422,
      593918599,
      807072360,
      1029159804,
      243815344,
      710536598,
      219532422,
      339194689,
      1115370236,
      803893657,
      806790933,
      145790893,
      601459052,
      259270170,
      120667892,
      994450868,
      456798524,
      411559218,
      1033648476,
      600727524,
      40042580,
      247503992,
      913555530,
      483175114,
      22363795,
      805184996,
      581234996,
      272927971,
      584586872,
      462362840,
      643206446,
      54682859,
      691867698,
      704691074,
      1010665647,
      426396134,
      775579937,
      6952392,
  };

  crack_func functions[] = {
      crack_groups,
      crack_groups_force,
      crack_groups_force_odd};

  int groupsLength = (sizeof(groups) / sizeof(groups[0]));
  int functionsLength = (sizeof(functions) / sizeof(functions[0]));

  clock_t start_t, end_t, total_t;
  u_int64_t *decoded;

  for (int i = 0; i < functionsLength; i++)
  {
    start_t = clock();
    for (int j = 0; j < TESTING_LENGTH; j++)
    {
      decoded = (*functions[i])(groups, groupsLength, e, n);
    }
    end_t = clock();
    total_t = (end_t - start_t);
    printf("Total time taken by CPU: %dμs\n", (int)total_t);
  }

  // Print groups as utf-8
  for (int i = 0; i < groupsLength; i++)
    printf("%s", (char *)&decoded[i]);
  printf("\n");
}