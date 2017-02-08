#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int intersection(int x1, int y1, int r1, int x2, int y2, int r2);
int inter(int x1, int y1, int r1, int x2, int y2, int r2);

#define TRUE 0
#define FALSE 1

#define NB_TESTS 6

int main(int ac, char **av)
{
  srand(time(NULL));
  int tab[NB_TESTS][6] = {
    {1, 1, 1, 2, 2, 1},
    {1, 1, 10, 1, 1, 2},
    {0, 9, 1, 0, 3, 1},
    {3, 2, 4, 7, -2, 1},
    {3, 2, 4, 7, -2, 2},
    {7, -2, 1, 7, -2, 2},
  };
  int i = 0;
  int f = 0;

  for (i = 0; i < 6; ++i)
    {
      if (inter(tab[i][0], tab[i][1], tab[i][2], tab[i][3], tab[i][4], tab[i][5]) !=
	  intersection(tab[i][0], tab[i][1], tab[i][2], tab[i][3], tab[i][4], tab[i][5]))
	{
	  ++f;
	  printf("KO\n");
	}
      else
	printf("OK\n");
      i++;
    }
  if (f == 0)
    return (0);
  return (-1);
}
