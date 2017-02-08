#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>

#define MAX_FILE_SIZE (1000000) // 1 Mo
#define DEFAULT_FOLDER ("Tests")

void	CIAdecoder(char* text);

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

char*	my_concat(const char *s1, const char *s2)
{
  const size_t len_s1 = strlen(s1);
  const size_t len_s2 = strlen(s2);
  char *result = malloc(sizeof(*result) * (len_s1 + len_s2 + 1));

  if (result == NULL)
    return (NULL);
  memcpy(result, s1, len_s1);
  memcpy(result + len_s1, s2, len_s2 + 1);
  return (result);
}

int	evaluation(char* original, char* user, long file_length)
{
  long		i = 0;
  long		score = 0;

  if (original == NULL || user == NULL)
    return (0);
  while (original[i])
    {
      if (!user[i])
	{
	  score -= file_length - i;
	  break;
	}
      if (user[i] == original[i])
	score += 1;
      ++i;
    }
  return ((score <= 0) ? 0 : (score * 100 / file_length));
}

char*	randomize(char* text)
{
  char	tab[26] = {0};
  int	i = 0;
  int	nb = 0;
  int	j;

  while (i < 26)
    {
      j = 0;
      nb = (rand() % (26 - i)) + 1;
      while (nb > 0 && j < 26)
	{
	  if (tab[j] == 0)
	    --nb;
	  ++j;
	}
      if (nb != 0 && j == 26)
	{
	  fprintf(stderr, "[Algolinette] Fail to randomize the file.\n");
	  return (NULL);
	}
      tab[j - 1] = i + 65;
      ++i;
    }
  long index = 0;
  while (text[index])
    {
      if (text[index] >= 'A' && text[index] <= 'Z')
	text[index] = tab[text[index] - 65];
      else if (text[index] >= 'a' && text[index] <= 'z')
	text[index] = tab[text[index] - 65 - 32] + 32;
      ++index;
    }
  return (text);
}

int	launch_test(char* filepath)
{
  char*	file_content;
  long	file_length;
  char*	encoded_file;
  char*	user_file;
  int	score_percent;

  if ((file_content = readFile(filepath, &file_length)) == NULL)
    return (2);
  if ((user_file = malloc(sizeof(*user_file) * (file_length + 1))) == NULL)
    return (2);
  (void)strcpy(user_file, file_content);
  CIAdecoder(randomize(user_file));
  score_percent = evaluation(file_content, user_file, file_length);
  free(file_content);
  free(user_file);
  if (score_percent > 80) // 80% to succeed
    {
      printf("\tYEAH ! You succeed at %d%% !\n", score_percent);
      return (0);
    }
  printf("\tOh no ! You only have %d%% correct ! :'( Try again !\n", score_percent);
  return (1);
}

int	main(int argc, char** argv)
{
  DIR*	dir = NULL;
  struct dirent* dir_info = NULL;
  char*	tmp;
  char*	pathname;
  int	result = 0;
  int	tmpRes;
  int	testNumber = 1;

  if (argc < 1)
    {
      fprintf(stderr, "./moulinette [test_folder:Defaut='Tests']\n");
      return (1);
    }
  srand(time(NULL));
  if ((dir = opendir((argv[1]) ? argv[1] : DEFAULT_FOLDER)) == NULL)
    {
      fprintf(stderr, "[Algolinette] Fail to open the folder.\n");
      return (1);
    }
  while ((dir_info = readdir(dir)) != NULL)
    {
      if (dir_info->d_name[0] != '.')
	{
	  tmp = my_concat((argv[1]) ? argv[1] : DEFAULT_FOLDER, "/");
	  pathname = my_concat(tmp, dir_info->d_name);
	  free(tmp);
	  printf("Test n°%d:\n", testNumber);
	  if ((tmpRes = launch_test(pathname)) == -2)
	    return (1);
	  if (tmpRes != 0)
	    result = 1;
	  free(pathname);
	  ++testNumber;
	}
    }
  printf((result) ? "\n-> FAILED\n\n" : "\n-> Succeed ! You're a genius !\n\n");
  closedir(dir);
  return (0);
}
