#ifndef DUPLICATE_H_INCLUDED
#define DUPLICATE_H_INCLUDED

#include "preproccessor.h"

void copyGene(gene * popCurr, gene * dupCurr);
void genDisGraph(double avg, bool adding);
int * orderDistances(creature * group);
bool isInArray(int * arr, int len, int toCheck);
void pruneAndFill(int *, creature *, int genSize);
int * orderedDist(creature*, int genSize);
#endif // DUPLICATE_H_INCLUDED
