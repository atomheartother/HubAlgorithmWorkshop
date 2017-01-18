#include <stdio.h>
#include <stdint.h>

int     speech(int n);

/*
** This solution is not meant to be understood by you.
** You're also not supposed to be reading this.
*/
int     countbits(uint32_t i)
{
  i = i - ((i >> 1) & 0x55555555);
  i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
  return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

int     test(uint32_t x)
{
  if (countbits(x) == speech(x))
    {
      printf("Testing %d... OK\n", x);
      return 0;
    }
  printf("Testing %d... KO!\n", x);
  return 1;
}

int     main()
{
  if (test(0)) return 1;
  if (test(2)) return 1;
  if (test(-5)) return 1;
  if (test(0x7FFFFF)) return 1;
  if (test(-121238712387)) return 1;
  if (test(2147483647)) return 1;
  return 0;
}
