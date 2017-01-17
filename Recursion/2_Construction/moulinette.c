#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"


/*
** Reading file
*/

#define MAX_FILE_SIZE (1000000) // 1 Mo

char*	readFile(char *filename, long* file_length)
{
  FILE*	file = fopen(filename, "r");
  char*	buffer = NULL;

  if (file == NULL)
    {
      fprintf(stderr, "[Algolinette] Fail to open the file.\n");      
      return (NULL);
    }
  if (file)
    {
      if (fseek(file, 0, SEEK_END) == -1 || (*file_length = ftell(file)) == -1)
	{
	  fprintf(stderr, "[Algolinette] Error : can't get file length.\n");
	  return (NULL);
	}
      rewind(file);
      if (*file_length > MAX_FILE_SIZE)
	{
	  fprintf(stderr, "[Algolinette] The File is too big (%ld bytes).\n", *file_length);
	  return (NULL);
	}
      if ((buffer = malloc(sizeof(char) * (*file_length + 1))) == NULL)
	return (NULL);
      buffer[*file_length] = 'z';
      fread(buffer, sizeof(char), *file_length + 1, file);
      if (feof(file) == 0 || ferror(file) != 0)
	{
	  fprintf(stderr, "[Algolinette] Fail file read.[%d][%d]\n", feof(file), ferror(file));
	  free(buffer);
	  return (NULL);
	}
      buffer[*file_length] = '\0';
      fclose(file);
    }
  return (buffer);
}

char*	getNewLine(char* file_output, long* index)
{
  char	*line = file_output + *index;

  if (*index > 0)
    file_output[*index - 1] = '\n';
  if (file_output[*index] == '\0')
    return (NULL);
  while (file_output[*index] != '\n' && file_output[*index] != '\0')
    ++(*index);
  if (file_output[*index] != '\0')
    {
      file_output[*index] = '\0';
      ++(*index);
    }
  return (line);
}

/*
** Tools
*/
block_t*	depile(storage_t* st)
{
  block_t*	res;

  if (st == NULL || st->pileLength == 0 || st->blockPile == NULL)
    return (NULL);
  res = st->blockPile;
  st->blockPile = res->next;
  st->pileLength -= 1;
  return (res);
}

void		empile(storage_t* st, block_t* block)
{
  if (st == NULL || block == NULL)
    return ;
  block->next = st->blockPile;
  st->blockPile = block;
  st->pileLength += 1;
}


/*
** Algolinette
*/
int	getCityNames(char* line, char* from, char* dest)
{
  size_t index = 0;
  size_t from_i = 0;
  size_t dest_i = 0;

  while (line[index] != '[' && line[index] != '\0')
    ++index;
  if (line[index] == '\0')
    return (1);
  ++index;
  while (line[index] != ']' && line[index] != '\0')
    {
      from[from_i] = line[index];
      ++from_i;
      ++index;
    }
  if (line[index] == '\0')
    return (1);
  from[from_i] = '\0';
  ++index;
  while (line[index] != '[' && line[index] != '\0')
    ++index;
  if (line[index] == '\0')
    return (1);
  ++index;
  while (line[index] != ']' && line[index] != '\0')
    {
      dest[dest_i] = line[index];
      ++dest_i;
      ++index;
    }
  if (line[index] == '\0')
    return (1);
  dest[dest_i] = '\0';
  return (0);
}

int	doUserAction(char* line, test_t* test)
{
  size_t lengthLine = strlen(line);
  char*	from_name = malloc(sizeof(char) * lengthLine);
  char*	dest_name = malloc(sizeof(char) * lengthLine);
  storage_t* from_storage;
  storage_t* dest_storage;

  if (!from_name || !dest_name)
    return (1);
  if (getCityNames(line, from_name, dest_name) != 0)
    {
      fprintf(stderr, "[Algolinette] Wrong syntax.\n\tRecieve : '%s'\n", line);
      free(from_name);
      free(dest_name);
      return (1);
    }
  if (strcmp(from_name, test->denver->name) == 0)
    from_storage = test->denver;
  else if (strcmp(from_name, test->albuquerque->name) == 0)
    from_storage = test->albuquerque;
  else if (strcmp(from_name, test->el_paso->name) == 0)
    from_storage = test->el_paso;
  else
    {
      fprintf(stderr, "[Algolinette] Unknown city name.\n");
      return (1);
    }
  if (strcmp(dest_name, test->denver->name) == 0)
    dest_storage = test->denver;
  else if (strcmp(dest_name, test->albuquerque->name) == 0)
    dest_storage = test->albuquerque;
  else if (strcmp(dest_name, test->el_paso->name) == 0)
    dest_storage = test->el_paso;
  else
    {
      fprintf(stderr, "[Algolinette] Unknown city name.\n");
      return (1);
    }
  if (dest_storage->blockPile != NULL
      && dest_storage->blockPile->block_weight < from_storage->blockPile->block_weight)
    {
      fprintf(stderr, "[Algolinette] Oh ! An heavy bloc can't be on a lighter one !\n");
      return (1);
    }
  empile(dest_storage, depile(from_storage));
  return (0);
}

void	print_result(test_t* currentTest, unsigned int currentTestNbActions)
{
  long	percent = ((currentTestNbActions - currentTest->correctionRes) * 100);

  percent /= (currentTest->maxAttempt - currentTest->correctionRes);
  percent = 100 - percent;
  fprintf(stderr, "[Algolinette] %s : %ld%% - %s\n", currentTest->testName, percent,
	  (currentTestNbActions <= currentTest->minValidation) ? "OK" : "KO");
}

int	algolinetteConstruction(char* file_output)
{
  char*	line;
  long	index = 0;
  test_t* currentTest;
  unsigned int testNb = 0;
  unsigned int currentTestNbActions;

  while ((currentTest = getTest(testNb)) != NULL)
    {
      currentTestNbActions = 0;
      while ((line = getNewLine(file_output, &index)) != NULL && line[0] != '#')
	{
	  if (doUserAction(line, currentTest) != 0)
	    return (1);
	  ++currentTestNbActions;
	}
      if (currentTest->denver->pileLength != 0 || currentTest->albuquerque->pileLength != 0)
	{
	  fprintf(stderr, "[Algolinette] %s : 0%% - KO\n", currentTest->testName);	  
	  return (1);
	}
      else
	print_result(currentTest, currentTestNbActions);
      ++testNb;
    }
  return (0);
}

int	main(int argc, char** argv)
{
  char	*file_output;
  long	file_length;

  if (argc != 2)
    {
      fprintf(stderr, "./moulinette [file_output]\n");
      return (1);
    }
  if ((file_output = readFile(argv[1], &file_length)) == NULL)
    {
      fprintf(stderr, "[Algolinette] Reading file error.\n");
      return (1);
    }
  algolinetteConstruction(file_output);
  /* write(1, file_output, file_length); */
  return (0);
}
