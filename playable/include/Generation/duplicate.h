#ifndef DUPLICATE_H_INCLUDED
#define DUPLICATE_H_INCLUDED

#include "preproccessor.h"

void copyGene(gene * popCurr, gene * dupCurr);
void genDisGraph(double avg, bool adding);
int * orderDistances(creature * group);
bool isInArray(int * arr, int len, int toCheck);
void duplicate(int * toDup, creature * population);
#endif // DUPLICATE_H_INCLUDED
