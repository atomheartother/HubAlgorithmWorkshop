#include <stdio.h>
#include <unistd.h>

int     counting(int n);

int     test(int num)
{
  static int test = 1;
  int   sum = 0, val = 0;
  int   res;
  while (val < num)
    sum += ++val;
  if ((res = counting(num)) == sum)
    printf("Test #%d: %d\tOK\n", test, num);
  else
    printf("KO - counting(%d) != %d\n", num, res);
  test++;
  return (res == sum);
}

int     main()
{
  if (!test(1)) return 1;
  if (!test(4)) return 1;
  if (!test(17)) return 1;
  if (!test(42)) return 1;
  if (!test(0)) return 1;
  return 0;
}
