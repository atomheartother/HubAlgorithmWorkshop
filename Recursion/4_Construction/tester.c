#include <stdlib.h>
#include <string.h>
#include "data_structures.h"

typedef test_t*	(*test_fnct)();


/*
** Tests tools
*/

block_t*        newBlock(int weight, block_t* next)
{
  block_t*      res = malloc(sizeof(block_t));
  res->block_weight = weight;
  res->next = next;
  return (res);
}

storage_t*      newStorage(block_t* blockPile, size_t pileLength, char* name)
{
  storage_t*    res = malloc(sizeof(storage_t));
  res->blockPile = blockPile;
  res->pileLength = pileLength;
  res->name = name;
  return (res);
}

test_t*		newTest(storage_t* denver, storage_t* albuquerque, storage_t* el_paso,
			char* name, unsigned int correction, unsigned int validation, unsigned int maxAttempt)
{
  test_t*	res = malloc(sizeof(test_t));
  res->denver = denver;
  res->albuquerque = albuquerque;
  res->el_paso = el_paso;
  res->testName = name;
  res->correctionRes = correction;
  res->minValidation = validation;
  res->maxAttempt = maxAttempt;
  return (res);
}

#include <stdio.h>
void		freeTest(test_t* test)
{
  block_t*	tmp;

  while (test->denver->blockPile)
    {
      tmp = test->denver->blockPile;
      test->denver->blockPile = test->denver->blockPile->next;
      free(tmp);
    }
  while (test->albuquerque->blockPile)
    {
      tmp = test->albuquerque->blockPile;
      test->albuquerque->blockPile = test->albuquerque->blockPile->next;
      free(tmp);
    }
  while (test->el_paso->blockPile)
    {
      tmp = test->el_paso->blockPile;
      test->el_paso->blockPile = test->el_paso->blockPile->next;
      free(tmp);
    }
  free(test->denver->name);
  free(test->albuquerque->name);
  free(test->el_paso->name);
  free(test->denver);
  free(test->albuquerque);
  free(test->el_paso);
  free(test->testName);
  free(test);
}


/*
** Tests - Add yours !
*/

test_t*	test1()
{
  block_t*	b1 = newBlock(42, NULL);
  storage_t*	denver = newStorage(b1, 1, strdup("Denver"));
  storage_t*	albuquerque = newStorage(NULL, 0, strdup("Albuquerque"));
  storage_t*	el_paso = newStorage(NULL, 0, strdup("El Paso"));
  test_t*	test = newTest(denver, albuquerque, el_paso, strdup("Basic test 1"), 1, 2, 4);
  return (test);
}

test_t*	test2()
{
  block_t*	b2 = newBlock(11, NULL);
  block_t*	b1 = newBlock(8, b2);
  storage_t*	denver = newStorage(b1, 2, strdup("Denver"));
  storage_t*	albuquerque = newStorage(NULL, 0, strdup("Albuquerque"));
  storage_t*	el_paso = newStorage(NULL, 0, strdup("El Paso"));
  test_t*	test = newTest(denver, albuquerque, el_paso, strdup("Basic test 2"), 3, 5, 10);
  return (test);
}

test_t*	test3()
{
  block_t*	b3 = newBlock(21, NULL);
  block_t*	b2 = newBlock(11, b3);
  block_t*	b1 = newBlock(8, b2);
  storage_t*	denver = newStorage(b1, 3, strdup("Denver"));
  storage_t*	albuquerque = newStorage(NULL, 0, strdup("Albuquerque"));
  storage_t*	el_paso = newStorage(NULL, 0, strdup("El Paso"));
  test_t*	test = newTest(denver, albuquerque, el_paso, strdup("Basic test 3"), 7, 11, 22);
  return (test);
}


/*
** Testing function - Edit const values to add your tests
*/
test_t*	getTest(unsigned int testNo)
{
  const	unsigned int	nbExercises = 3;
  const test_fnct	tabExercises[] = {&test1, &test2, &test3};

  if (testNo >= nbExercises)
    return (NULL);
  return (tabExercises[testNo]());
}
