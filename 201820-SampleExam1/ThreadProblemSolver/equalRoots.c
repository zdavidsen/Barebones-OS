#include "./equalRoots.h"

void findEqualRoots(int a, int b, int c) {
  double d, root;

  d = b * b - 4 * a * c;
  printf("==================================================\n");
  printf("Quadratic equation = %dx^2%+dx%+d = 0\n", a, b, c);
  if (d == 0) {
    printf("Has equal roots.\n");
    root = -b / (2.0 * a);
    printf("Root of quadratic equation is: %.3f\n", root);
  } else {
    printf("Sorry, but the roots are not equal.\n");
  }
  printf("==================================================\n");
}
