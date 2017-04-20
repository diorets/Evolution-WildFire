#ifndef STICKBALLMUTATIONS_H_INCLUDED
#define STICKBALLMUTATIONS_H_INCLUDED

#include "preproccessor.h"
// This looks terrible... Find a better style

/** Applies a set of mutations to a genome **/
void mutateStickball(creature * toMutate);

/** Validation / Retrival Functions
        // * indicates the function assumes a valid and "likely" result ie. cant handle the contrary **/
posi getValidPosition(gene * head); // *
    /*if no valid location exists returns the original values */
posi getValidShift(gene * head, const double cx, const double cy, const double cz, double amount);
conn goodConnection(gene * head); // *
bool typeConnectionExists(gene * head, char type, int a, int b);
bool connectionExists(gene * head, int a, int b);

/** Possible Mutations
        // Multiple indicates the function affects multiple nodes **/
void relocateNodes(gene * head, double shiftChance); // * Multiple
void shiftNodes(gene * head, double shiftChance, double amount); //Multiple
void addNode(gene * head, double nodeChance, double muscleChance, double boneChance); // *
void removeNode(gene * head, double removeChance);

void addConnection(gene * head, double addChance);
void removeConnection(gene * head, double addChance);
void swapConnection(gene * head, double swapChance);

void verifyGenome(gene * head);

void removeStrandedNodes(gene * head);

#endif // STICKBALLMUTATIONS_H_INCLUDED
