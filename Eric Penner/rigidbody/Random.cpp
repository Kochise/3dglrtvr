#include ".\random.h"

#include <stdlib.h>



float CRandom::RandomFloat(float fStart,float fEnd)
{
	return (float)(  (rand() / (double) RAND_MAX) * (fEnd-fStart) + fStart );
}