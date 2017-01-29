#ifndef HYPERTILE_H
#define HYPERTILE_H

#include "vector.h"

long HyperTile_InitHW(long window);
void HyperTile_Init(int size);
void HyperTile_Draw(mat3 &m, float scale, float morph, int drop);

#endif
