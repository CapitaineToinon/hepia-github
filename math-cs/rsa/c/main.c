#include <stdio.h>
#include "rsa.h"

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

  int length = (sizeof(groups) / sizeof(groups[0]));
  u_int64_t *decoded = crack_groups(groups, length, e, n);

  for (int i = 0; i < length; i++)
  {
    printf("%s", (char*)&decoded[i]);
  }
  printf("\n");
}