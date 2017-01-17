#include <math.h>

#define ABS(x) ((x) < 0 ? -(x) : (x))

#define TRUE 0
#define FALSE 1

int inter(int x1, int y1, int r1, int x2, int y2, int r2)
{
  double d = sqrt((double)((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
  if (d <= r1 + r2 && ABS(r1 - r2) <= d)
    return (TRUE);
  return (FALSE);
}
