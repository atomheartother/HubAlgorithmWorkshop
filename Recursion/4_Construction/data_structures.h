#ifndef DATA_STRUCTURES
# define DATA_STRUCTURES

# include <stdlib.h>

typedef struct		block_s
{
  int			block_weight;
  struct block_s*	next;
}			block_t;

typedef struct	storage_s
{
  block_t*	blockPile;
  size_t	pileLength;
  char*		name;
}               storage_t;

typedef struct	test_s
{
  storage_t*	denver;
  storage_t*	albuquerque;
  storage_t*	el_paso;
  char*		testName;
  unsigned int	correctionRes;
  unsigned int	minValidation;
  unsigned int	maxAttempt;
}		test_t;

#endif /* !DATA_STRUCTURES */
