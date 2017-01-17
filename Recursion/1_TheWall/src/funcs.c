#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

void	delete_map(char **map)
{
  unsigned	i = 0;
  while (map[i])
    free(map[i++]);
  free(map);
}

int	getnbr(const char *str, int *idx)
{
  int	n = 0;
  int	ignored = 0;
  if (!idx)
    idx = &ignored;
  while (str[*idx])
    {
      if (str[*idx] < '0' || str[*idx] > '9')
	break ;
      n = (n * 10) + (str[*idx] - '0');
      *idx += 1;
    }
  return n;
}

static int	get_next_num(FILE *stream)
{
  char	*s = NULL;
  size_t len = 0;
  if (getline(&s, &len, stream) == -1)
    return -1;
  int	res = getnbr(s, NULL);
  free(s);
  return res;
}

static int	fill_map(FILE* stream, char **map, const int height)
{
  int	i = 0;
  while (i < height)
    {
      map[i] = 0;
      size_t len = 0;
      if (getline(&(map[i]), &len, stream) == -1)
	{
	  delete_map(map);
	  return 1;
	}
      i++;
    }
  map[i] = 0;
  return 0;
}

char	**get_map(const char *fname, int *x, int *y)
{
  FILE* stream = fopen(fname, "r");
  if (!stream)
    return NULL;
  const int height = get_next_num(stream);
  if (height != -1)
    {
      char	**map = malloc(sizeof(char*) * (height + 1));
      if (map)
	{
	  *x = get_next_num(stream);
	  *y = get_next_num(stream);
	  if (*x != -1 && *y != -1)
	    {
	      if (!fill_map(stream, map, height))
		{
		  fclose(stream);
		  return map;
		}
	    }
	  else
	    free(map);
	}
    }
  fclose(stream);
  return NULL;
}
