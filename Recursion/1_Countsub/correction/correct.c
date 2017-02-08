#include <stdio.h>
#include <unistd.h>

int     countsub(char *str, char *sub);

int	test(char *str, char*sub)
{
  printf("Finding %s in %s...\n", sub, str);
  return countsub(str, sub);
}

int     main()
{
  if (test("abcdeabcdeabcdfesd;fabcde", "abcde") != 3) return 1;
  if (test("hehahahahh hahahihohaaa", "ha") != 6) return 1;
  if (test("zobzib,,;zabzzzoobbbzob", "zob") != 2) return 1;
  if (test("  acacacababacatacabacaba23432acacaba", "acaba") != 4) return 1;
  return 0;
}
