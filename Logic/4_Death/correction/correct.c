/*
** It's ok to look at this file
** because you're curious of how I
** correct this. However you should know
** this file contains my solution to this 
** puzzle for now.
** 
** You can still read it, but I don't think
** it would be fun for anyone :c
**
** - Thomas Navennec
*/

int     runFast(char** map);

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BAD_CHAR 'x'

struct s_node;

// Represents a link from a node to another
typedef struct  s_link
{
  struct s_node *dest;
  struct s_link *next;
  unsigned      moves;
}               t_link;

// Represents a node
typedef struct  s_node
{
  t_link        *links;
  struct s_node *next;
  unsigned      x;
  unsigned      y;
  char          id;
  char          busy;
  char          shortest;
}               t_node;

typedef struct s_tile
{
  struct s_tile *next;
  unsigned      x;
  unsigned      y;
  char          val;
}               t_tile;

char g_display;

int    add_node(t_node **start, unsigned x,
                unsigned y, char id, char begin)
{
  t_node        *node = malloc(sizeof(t_node));
  if (!node)
    return 1;
  node->x = x;
  node->y = y;
  node->id = id;
  node->busy = 0;
  node->shortest = 0;
  node->links = NULL;
  if (!*start)
    {
      node->next = NULL;
      *start = node;
    }
  else if (begin)
    {
      node->next = *start;
      *start = node;
    }
  else
    {
      node->next = (*start)->next;
      (*start)->next = node;
    }
  return 0;
}

void    delete_links(t_link *link)
{
  t_link        *temp;

  while (link)
    {
      temp = link->next;
      free(link);
      link = temp;
    }
}

void    delete_nodes(t_node *list)
{
  t_node        *temp;
  while (list)
    {
      temp = list->next;
      delete_links(list->links);
      free(list);
      list = temp;
    }
}

// Counts lines
// Since we're reading every char individually, we also take this opportunity to find nodes
unsigned        count_lines(const char * const str, t_node **start)
{
  FILE *f = fopen(str, "r");
  if (!f)
    return 0;
  unsigned y = 0, x = 0;
  while (!feof(f))
    {
      char c = fgetc(f);
      if (c == -1)
        break ;
      else if (c == '\n')
        {
          x = 0;
          y++;
        }
      else
        {
          if (c == '0')
            {
              if (add_node(start, x, y, c, 1))
                return 0;
            }
          else if (c != '.' && c != '#')
            if (add_node(start, x, y, c, 0))
              return 0;
          x++;
        }
    }
  fclose(f);
  return y;
}

void    delete_map(char **map)
{
  unsigned ii = 0;
  while (map[ii])
    free(map[ii++]);
}

void    check_res(unsigned *res, unsigned val)
{
  if (val && (!*res || *res > val))
    *res = val;
}

t_tile*    create_tile(char **map, unsigned x, unsigned y)
{
  t_tile *list = malloc(sizeof(t_tile));

  list->x = x;
  list->y = y;
  list->val = map[y][x];
  map[y][x] = BAD_CHAR;
  list->next = NULL;
  return list;
}

void    append_to_list(t_tile **newlist, t_tile *new)
{
  if (*newlist)
    new->next = (*newlist);
  *newlist = new;
}

unsigned        check_depth(char **map, t_tile *current, t_tile **newlist,
                            t_tile *oldlist, t_node *node)
{
  // Done with this depth
  if (!current)
    return 0;

  unsigned res = 0;

  // Found match
  if (current->val != '.' && current->val != node->id)
    res = 1;

  // Only '.' and nodes get added
  if (current->val != '#' && current->val != BAD_CHAR)
    {
      // Add tiles to new list
      if (map[current->y][current->x + 1] != '#' && map[current->y][current->x + 1] != BAD_CHAR)
        append_to_list(newlist, create_tile(map, current->x + 1, current->y));
      if (map[current->y][current->x - 1] != '#' && map[current->y][current->x - 1] != BAD_CHAR)
        append_to_list(newlist, create_tile(map, current->x - 1, current->y));
      if (map[current->y + 1][current->x] != '#' && map[current->y + 1][current->x] != BAD_CHAR)
        append_to_list(newlist, create_tile(map, current->x, current->y + 1));
      if (map[current->y - 1][current->x] != '#' && map[current->y - 1][current->x] != BAD_CHAR)
        append_to_list(newlist, create_tile(map, current->x, current->y - 1));
    }
  return res + check_depth(map, current->next, newlist, oldlist, node);
}

void    delete_tiles(char **map, t_tile *list)
{
  t_tile *temp;
  while (list)
    {
      map[list->y][list->x] = list->val;
      temp = list->next;
      free(list);
      list = temp;
    }
}

void    join_lists(t_tile **l1, t_tile *l2)
{
  t_tile *temp = l2;
  if (!temp)
    return ;
  while (temp->next)
    temp = temp->next;
  temp->next = *l1;
  *l1 = l2;
}

char            end_condition(t_link *links, t_node *nodes)
{
  unsigned ncount = 0;
  while (nodes)
    {
      ncount++;
      nodes = nodes->next;
    }
  unsigned lcount = 0;
  while (links)
    {
      lcount++;
      links = links->next;
    }
  return lcount == ncount - 1;
}

unsigned        get_map_len(char* * map)
{
  unsigned len = 0;
  unsigned i = 0;
  while (map[i])
    len += strlen(map[i++]);
  return len;
}

void    show_map(char **map)
{
  const unsigned len = get_map_len(map);
  char pmap[len + 1];
  pmap[0] = 0;
  unsigned i = 0;
  while (map[i])
    {
      strcat(pmap, map[i]);
      i++;
    }
  write(1, pmap, len);
  usleep(10000);
}

void        bfs_link(char **map, t_node *node, t_node *start)
{
  // This contains the current values
  t_tile        *list = create_tile(map, node->x, node->y);
  // This contains the next values
  t_tile *newlist = NULL;
  // This contains the previous values
  t_tile *oldlist = NULL;

  unsigned      dist = 0;

  while (42)
    {
      unsigned matches = check_depth(map, list, &newlist, oldlist, node);
      t_tile *l = list;
      // Browse the current list to find matches
      if (matches)
        {
          while (matches && l)
            {
              // we found a value that matches
              if (l->val != '.')
                {
                  // Look for the node n we matched
                  t_node *n = start;
                  while (n)
                    {
                      if (n->id == l->val)
                        {
                          // Fill link information
                          t_link *t = malloc(sizeof(t_link));
                          t->dest = n;
                          t->moves = dist;
                          t->next = node->links;
                          node->links = t;
                          break ;
                        }
                      n = n->next;
                    }
                  matches--;
                }
              l = l->next;
            }
          if (end_condition(node->links, start))
            break ;
        }
      join_lists(&oldlist, list); // oldlist = list + oldlist
      list = newlist;
      newlist = NULL;
      dist++;
      if (list == NULL)
        fprintf(stderr, "Could not find all nodes starting from %c!\n", node->id);
      if (g_display)
        show_map(map);
    }
  delete_tiles(map, oldlist);
  delete_tiles(map, list);
  delete_tiles(map, newlist);
}

// Start from each node and find the path to every other node
void    fill_links(char **map, t_node *start)
{
  t_node        *node = start;
  while (node)
    {
      bfs_link(map, node, start);
      node = node->next;
    }
}

int     fill_map(char **map, const char * const str,
                 unsigned lines)
{
  FILE *f = fopen(str, "r");
  if (!f)
    return 1;
  unsigned i = 0;
  size_t line_width = 0;
  while (i < lines)
    {
      // Saves allocs, assuming the map is a rectangle
      if (!line_width)
        map[i] = NULL;
      else
        map[i] = malloc(line_width);
      if (getline(&map[i++], &line_width, f) == -1)
        {
          map[i] = NULL;
          delete_map(map);
          fclose(f);
          return 1;
        }
    }
  map[i] = NULL;
  fclose(f);
  return 0;
}

void    show_node(t_node *node)
{
  printf("%c (%u, %u)\n", node->id, node->x, node->y);
  t_link        *link = node->links;
  while (link)
    {
      printf("\t->%c: %u\n", link->dest->id, link->moves);
      link = link->next;
    }
}

// Go recursively into each link and find the shortest path. ez.
unsigned    solve(t_node *start, unsigned moves)
{
  if (!start || start->busy)
    return 0;
  start->busy = 1;
  t_link *l = start->links;
  unsigned      temp = 0, res = 0;
  while (l)
    {
      if ((temp = solve(l->dest, l->moves)) && (!res || res > temp))
        {
          res = temp;
          start->shortest = l->dest->id;
        }
      l = l->next;
    }
  start->busy = 0;
  return moves + res;
}

void    get_option(char *str)
{
  if (!strcmp("--display", str))
    g_display = 1;
}

int     main(int ac, char * const av[])
{
  g_display = 0;
  if (ac < 2)
    {
      fprintf(stderr, "usage: ./%s [--display] <map file>\n", av[0]);
      return 1;
    }
  unsigned idx = ac == 2 ? 1 : 2;
  if (idx == 2)
    get_option(av[1]);
  // We count lines first so we can allocate the right number of pointers
  // We also take this opportunity to get node positions
  t_node *start = NULL;
  unsigned lines = count_lines(av[idx], &start);
  if (!lines)
    {
      fprintf(stderr, "Something went wrong while opening %s\n", av[idx]);
      return 1;
    }
  // Allocate map then fill it
  char **map = malloc(sizeof(char*) * (lines + 1));
  if (!map)
    return 1;
  int res = 0;
  if (!fill_map(map, av[idx], lines))
    {
      fill_links(map, start);
      res = (solve(start, 0) == runFast(map)) ? 0 : 1;
      delete_map(map);
    }
  else
    fprintf(stderr, "Could not fill map\n");
  delete_nodes(start);
  free(map);
  return res;
}
