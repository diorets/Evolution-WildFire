#include "GameModes/Simulate/Cube/cubeMutations.h"
#include "GameModes/Simulate/Cube/cubeGenes.h"  // Various Genes

static void verifyGenome(gene * head) {
    if (head == NULL) return; // WS
	return;
}

void mutateCube(creature * toMutate) {
    if (toMutate == NULL) return; // WS
    verifyGenome(toMutate->genome);
	return;
}
