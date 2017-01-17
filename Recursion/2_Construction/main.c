#include <stdlib.h>
#include <stdio.h>
#include "test.h"

void    construction(size_t nbBlocks, storage_t* location1, storage_t* location2, storage_t* location3);


int		main()
{
  test_t*	currentTest;
  unsigned int	testNb = 0;

  while ((currentTest = getTest(testNb)) != NULL)
    {
      construction(currentTest->denver->pileLength, currentTest->denver, currentTest->albuquerque,
		   currentTest->el_paso);
      printf("###\n");
      freeTest(currentTest);
      ++testNb;
    }
  return (0);
}
