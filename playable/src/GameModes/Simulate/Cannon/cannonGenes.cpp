#include "GameModes/Simulate/Cannon/cannonGenes.h"
#include "GameModes/Simulate/Cannon/cannonMutations.h"
#include "ErrorHandle/error.h"

static gene * infoGene() {
    gene * newInfo = (gene*) malloc(sizeof(gene));
    if (newInfo == NULL) quit(MALLOC_ERROR);

    newInfo->start = 'i';
    newInfo->iData[CE::numPara]   = 9; // num
    newInfo->iData[CE::numCannons] = 1; // #Cannon
    newInfo->iData[CE::numGenes]   = 2; // #Total
    newInfo->endof = '\0';
    newInfo->next = NULL;
    return newInfo;
}

#include <Math/myMath.h>
static gene * cannonGene(gene * head) { // Does check valid location
    gene * blockG = (gene*) malloc(sizeof(gene));
    if (blockG == NULL) quit(MALLOC_ERROR);
    blockG->start = 'c';

    /* Get Parameters */
    int numPara = head->iData[CE::numPara];
    for (int i = 0; i < numPara; i++) {
        blockG->fData[i] = randf(1.0 / fact(i));
    }
    blockG->fData[numPara] = 30.0;

    blockG->endof = '\0';
    blockG->next = NULL;
    return blockG;
}


#include "Functional/list.h"
gene * createCannonGenome(gene * head) {
	if (head != NULL) quit(GENOME_ERROR);
    head = addToBack(head, infoGene());
    head = addToBack(head, cannonGene(head));
	return head;
}


void createCannonCreature(creature * newBorn) {
    int numCannons = 0;
	gene * currentGenome = newBorn->genome;
	int numPara = newBorn->genome->iData[CE::numPara];
    //cannon * components = ((cannon*) newBorn->components);

    double initVel = 0.0;
    while (currentGenome != NULL) {
        switch (currentGenome->start) {
            case 'i': // Needs to free doesnt it?
                //components->cannon = (cannon*) eMalloc(sizeof(cannon) * currentGenome->iData[CE::numCannons]);
                break;
            case 'c':
                for (int i = 0; i < numPara; i++) {
                    ((cannon*) newBorn->components)->coeff[i] = currentGenome->fData[i];
                }
                initVel = currentGenome->fData[numPara];
                numCannons++;
                break;
            default:
                quit(CREATURE_ERROR);
        }
        currentGenome = currentGenome->next;
    }

    ((cannon*) newBorn->components)->ball.force = zero();
    for (int i = 0; i < 100; i++) {
        ((cannon*) newBorn->components)->target[i] = randf(100);
    }
    ((cannon*) newBorn->components)->targetNum = 0;
    ((cannon*) newBorn->components)->vInit = initVel;
    ((cannon*) newBorn->components)->angle = 0.0;

    ((cannon*) newBorn->components)->ball.vel = vec(0, 0, 0);
    ((cannon*) newBorn->components)->ball.loc = vec(0, 0, CE::h);
    newBorn->fitness= 0.0;
	return;
}






















