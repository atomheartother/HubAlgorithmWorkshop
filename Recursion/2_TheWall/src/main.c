#include <stdio.h>
#include "findpath.h"
#include "funcs.h"

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
      findpath(map, x, y);
      delete_map(map);
      i++;
    }
  return 0;
}
