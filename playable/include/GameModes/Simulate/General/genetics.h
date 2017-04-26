#ifndef GENETICS_H_INCLUDED
#define GENETICS_H_INCLUDED

#include "preproccessor.h"

void mutateGenome(int system, creature * toMutate);
gene * createSystemGenome(int system, gene * genome);
void createSystemCreature(int system, creature * individual);
void copyGene(gene * popCurr, gene * dupCurr);
void pruneAndFill(int *, creature *, int genSize);
int * orderedDist(creature*, int genSize);

void saveGenome(creature * population, int gen, int index);

#endif // GENETICS_H_INCLUDED
