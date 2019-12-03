#include <stdio.h>
#include "prime.h"
#include "bezout.h"

typedef unsigned char BYTE;

void print_bytes(u_int64_t value)
{
  for (int i = 0; i < 4; i++)
  {
    BYTE b = ((value >> (8 * i)) & 0xFF); //extract byte
    if (b > 0x00)
      printf("%02X", b);
  }
}

u_int64_t decode(u_int64_t message, u_int64_t e, u_int64_t n, u_int64_t d)
{
  u_int64_t r = 1, b = message;
  e = d;

  while (e > 0)
  {
    if (e % 2 == 1)
    {
      r = (r * b) % n;
    }
    b = (b * b) % n;
    e = e / 2;
  }

  return r;
}

int main()
{
  uint64_t n = 1190836873ULL;
  uint64_t e = 1051ULL;
  uint64_t size = 0;

  u_int64_t max = (u_int64_t)floor(sqrt(n));
  u_int64_t *flags = SieveOfEratosthenes(max, &size);

  u_int64_t p, q;
  get_prime_factors(flags, n, &p, &q);

  u_int64_t fn = (p - 1) * (q - 1);
  u_int64_t gcd;
  int64_t d, f;
  get_gcd_bezout(e, fn, &gcd, &d, &f);

  // ensure d isn't negative
  if (d < 0)
    d = d % fn;

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

  int Length = (sizeof(groups) / sizeof(groups[0]));
  for (int i = 0; i < Length; i++)
  {
    uint64_t decoded = decode(groups[i], e, n, d);
    print_bytes(decoded);
  }
}