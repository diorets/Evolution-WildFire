#include "GameModes/Simulate/Turbine/turbineMutations.h"
#include "GameModes/Simulate/Turbine/turbineGenes.h"  // Various Genes

static void verifyGenome(gene * head) {
    if (head == NULL) return; // WS
	return;
}

void mutateTurbine(creature * toMutate) {
    if (toMutate == NULL)  return; // WS
	verifyGenome(toMutate->genome);
	return;
}
