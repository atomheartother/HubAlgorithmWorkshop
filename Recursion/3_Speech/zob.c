#include <stdio.h>
#include <stdint.h>

int     speech(uint32_t n)
{
  return (!n ? 0 : (n & 0b1) + speech(n >> 1));
}
