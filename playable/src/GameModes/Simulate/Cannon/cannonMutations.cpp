#include "GameModes/Simulate/Cannon/cannonMutations.h"
#include "GameModes/Simulate/Cannon/cannonGenes.h"  // Various Genes

static void verifyGenome(gene * head) {

	return;
}

#include "Math/myMath.h"
void mutateCannon(creature * toMutate) {
    int numPara = toMutate->genome->iData[CE::numPara];
    for (int i = 0; i < numPara; i++) {
        if (rand() % 10 == 0) {
            if (rand() % 5 == 0) {
                toMutate->genome->next->fData[i] = randf(1.0 / fact(i*i));
            } else {
                toMutate->genome->next->fData[i] += pmRandf(toMutate->genome->next->fData[i] / 10.0);
            }
        }
    }
	return;
}
