#include <stdlib.h>
#include <stdio.h>
#include "funcs.h"

int	correct_map(char **map, int x, int y);

int		main(int ac, char * const av[])
{
  int	i = 1;
  while (i < ac)
    {
      int	x;
      int	y;
      char	**map = get_map(av[i], &x, &y);
      if (map == NULL)
	return fprintf(stderr, "Could not read map data.\n");
      if (correct_map(map, x, y))
	{
	  delete_map(map);
	  exit(1);
	}
      printf("%s: OK\n", av[i]);
      delete_map(map);
      i++;
    }
  return 0;
}
