#define _XOPEN_SOURCE 500
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "get_next_line.h"

static char	*truncat(char * const str)
{
  unsigned	i;
  unsigned	j;
  char		*new;

  i = 0;
  while (str[i] && str[i] != '\n')
    i++;
  if (!(str[i]) || !(new = malloc(sizeof(char) * (i + 1))))
    return (NULL);
  j = 0;
  while (j < i)
    {
      new[j] = str[j];
      j++;
    }
  new[j] = 0;
  j = 0;
  while (str[++i])
    str[j++] = str[i];
  str[j] = 0;
  return (new);
}

static int	linebreak(char * const buffer,
			  char * const carry)
{
  unsigned	i;
  unsigned	j;

  i = 0;
  while (buffer[i])
    {
      if (buffer[i] == '\n')
	break ;
      i++;
    }
  if (!(buffer[i]))
    return (0);
  buffer[i] = 0;
  j = 0;
  while (buffer[++i])
    carry[j++] = buffer[i];
  carry[j] = 0;
  return (1);
}

char		*concat(char * const str,
			const char * const buffer)
{
  unsigned	i;
  unsigned	j;
  char		*new;

  if (!(new = malloc(sizeof(char) * ((str ? strlen(str) : 0) +
				     strlen(buffer) + 1))))
    return (NULL);
  i = 0;
  if (str)
    while (str[i])
      {
	new[i] = str[i];
	i++;
      }
  j = 0;
  while (buffer[j])
    {
      new[i + j] = buffer[j];
      j++;
    }
  new[i + j] = 0;
  if (str)
    free(str);
  return (new);
}

/*
** Resets carried data if fd changes
*/
char		*get_next_line(const int fd)
{
  char		buffer[BUFFER_SIZE + 1];
  static char	carry[BUFFER_SIZE + 1] = {0};
  ssize_t	size;
  char		*str;
  static int	lastfd = -1;

  if (lastfd == -1 || fd != lastfd)
    {
      carry[0] = 0;
      lastfd = fd;
    }
  if ((str = truncat(carry)))
    return (str);
  str = carry[0] ? strdup(carry) : NULL;
  carry[0] = 0;
  while ((size = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
      buffer[size] = 0;
      if ((linebreak(buffer, carry)) || size < BUFFER_SIZE)
	return (concat(str, buffer));
      str = concat(str, buffer);
    }
  return (str);
}
