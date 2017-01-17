#include <stdlib.h>
#include <stdio.h>
#include "funcs.h"

#define KO "KO"
#define BAD_COORD "Could not read all coordinates, they ended too early or were formatted wrong."
#define NO_END	"Coordinates did not start from the end."
#define IN_WALL "You ran into a wall!"
#define TWICE_TILE "You went through the same tile twice!"
#define BAD_MOVE "You tried to move too fast! (you moved diagonally or skipped a block)"

int	get_next_coords(int *x, int *y)
{
  char	*line = NULL;
  size_t	len = 0;
  if (getline(&line, &len, stdin) == -1)
    {
      printf("Input ended unexpectedly\n");
      return 2;
    }
  int	idx = 0;
  if (line[idx++] != '(')
    goto badinput;
  *x = getnbr(line, &idx);
  if (*x == -1 || line[idx++] != ',')
    goto badinput;
  *y = getnbr(line, &idx);
  if (*y == -1 || line[idx++] != ')')
    goto badinput;
  free(line);
  return 0;
 badinput:
  printf("Non-exercise input: %s\n", line);
  free(line);
  return 1;
}

int	correct_map(char **map, int x, int y)
{
  int	X, Y;
  int   res;
  int   prevX, prevY;
  while ((res = get_next_coords(&X, &Y)))
    if (res == 2)
      return 1;
  if (map[Y][X] != 'E')
    {
      puts(KO);
      puts(NO_END);
      return 1;
    }
  while (X != x || Y != y)
    {
      prevX = X;
      prevY = Y;
      map[Y][X] = 'P'; // Passed through here
      if (get_next_coords(&X, &Y))
        continue;
      if ((X != prevX && Y != prevY) || (prevX != X - 1 &&
                                         prevX != X + 1 &&
                                         prevY != Y + 1 &&
                                         prevY != Y - 1))
        {
          puts(KO);
          puts(BAD_MOVE);
          return 1;
        }
      if (map[Y][X] == '#')
	{
	  puts(KO);
	  puts(IN_WALL);
	  return 1;
	}
      if (map[Y][X] == 'P')
	{
	  puts(KO);
	  puts(TWICE_TILE);
	  return 1;
	}
    }
  return 0;
}
