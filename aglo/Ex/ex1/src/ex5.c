#include <stdio.h>

int main()
{
  float radius = 0, surface = 0, perimeter = 0;
  float pi = 3.14;

  printf("Enter the radius of the circle [m]: ");
  (void)scanf("%f", &radius);

  surface = pi * radius * radius;
  perimeter = 2.0f * pi * radius;

  printf("surface: %f m^2\n\r", surface);
  printf("surface: %f m", perimeter);

  return 0;
}