#include "GameModes/Simulate/Cube/cubeGenes.h"
#include "GameModes/Simulate/Cube/cubeMutations.h"
#include "ErrorHandle/error.h"

#include "Functional/list.h"
#include "Math/myMath.h"
#include <math.h>

#include <stdio.h>
#include <float.h>

static gene * springGene(gene * head, int a, int b);

static posi getValidPosition(gene * head, int currNum) {
    double radius = head->fData[TE::radius];
    while (true) {
        /* Get node to add on to */
        int i = 0;
        int addTo = currNum == 1 ? 0 : rand() % currNum;
        posi initLoc = vec(0.0, 0.0, 0.0);
        posi newLoc = vec(0.0, 0.0, 0.0);

        for (gene * curr = head; curr != NULL; curr = curr->next) {
            if (curr->start != 'b') {
                continue;
            }
            if (i == addTo) {
                initLoc.x = curr->fData[xposi];
                initLoc.y = curr->fData[yposi];
                initLoc.z = curr->fData[zposi];
            }
            i++;
        }

        // initLoc now holds the position of the attached-to node
        for (int trials = 0; trials < 100; trials++) {
            /* Generate New Random Location */
            newLoc.x = initLoc.x + 3*2 * radius * ((rand() % 3) ? 1 - rand() % 3:0);
            newLoc.y = initLoc.y + 3*2 * radius * ((rand() % 3) ? 1 - rand() % 3:0);
            newLoc.z = initLoc.z + 3*2 * radius * ((rand() % 3) ? 1 - rand() % 3:0);

            /* Determine Validity */
            bool valid = true;
            for (gene * jp = head; jp != NULL; jp = jp->next) {
                posi check = vec(jp->fData[TE::x], jp->fData[TE::y], jp->fData[TE::z]);
                if (equals(newLoc, check, 0.1)) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                head = addToBack(head, springGene(head, currNum, addTo));
                (head->iData[CUE::numSprings])++;
                for (int j = 0; j < currNum; j++) {
                    if (j == addTo) continue;
                    if (chance(10)) {
                        head = addToBack(head, springGene(head, currNum, j));
                        (head->iData[CUE::numSprings])++;
                    }
                }
                return newLoc;
            }
        }
    }
    return vec(0, 0, -10);
}

/* Genes */
static gene * infoGene(int n) {
    gene * newInfo = (gene*) malloc(sizeof(gene));
    if (newInfo == NULL) quit(MALLOC_ERROR);

    newInfo->start = 'i';
    newInfo->fData[TE::radius] = 0.5;

    newInfo->iData[TE::numBlocks] = n;
    newInfo->iData[CUE::numSprings] = 0;
    newInfo->iData[TE::numGenes] = n + 1; // Total
    newInfo->endof = '\0';
    newInfo->next = NULL;
    return newInfo;
}

static gene * blockGene(gene * head, int currNum) { // Does check valid location
    gene * blockG = (gene*) malloc(sizeof(gene));
    if (blockG == 0) quit(MALLOC_ERROR);;
    blockG->start = 'b';

    posi valid = getValidPosition(head, currNum);
    blockG->fData[TE::x] = valid.x;
    blockG->fData[TE::y] = valid.y;
    blockG->fData[TE::z] = valid.z;

    blockG->endof = '\0';
    blockG->next = NULL;
    return blockG;
}

#include <math.h>
static gene * blockGene(gene * head, posi loc) { // Does check valid location
    gene * blockG = (gene*) malloc(sizeof(gene));
    if (blockG == NULL) quit(MALLOC_ERROR);;
    blockG->start = 'b';

    blockG->fData[TE::x] = loc.x;
    blockG->fData[TE::y] = loc.y;
    blockG->fData[TE::z] = loc.z;

    blockG->endof = '\0';
    blockG->next = NULL;
    return blockG;
}

static gene * springGene(gene * head, int a, int b) { // Does check valid location
    gene * springG = (gene*) malloc(sizeof(gene));
    if (springG == NULL) quit(MALLOC_ERROR);;
    springG->start = 's';

    springG->iData[0] = a;
    springG->iData[1] = b;

    springG->endof = '\0';
    springG->next = NULL;
    return springG;
}

gene * createCubeGenome(gene * head) {
    printf("Creating Genome: %d\n", rand() % 10);
	if (head != NULL) quit(GENOME_ERROR);
	int n = 20;
    head = infoGene(n);

    head = addToBack(head, blockGene(head, vec(0.0, 0.0, 20.0)));
    for (int i = 1; i < n; i++) {
        head = addToBack(head, blockGene(head, i));
    }

    for (gene * i = head; i != NULL; i=i->next) {
        if (i->start == 'b') {
            if (equals(vec(i->fData[TE::x], i->fData[TE::y], i->fData[TE::z]), zero(), 0.001)) {
                quit(GENOME_ERROR);
            }
        }
    }
	return head;
}

void createCubeCreature(creature * newBorn) {
    int numBlocks = 0;
    int numSprings = 0;
    gene * currentGenome = newBorn->genome;
    cube * components = ((cube*) newBorn->components);

    while (currentGenome != NULL) {
        int a;
        int b;
        switch (currentGenome->start) {
            case 'i': // Needs to free doesnt it?
                components->blocks = (node*) eMalloc(sizeof(node) * currentGenome->iData[TE::numBlocks]);
                components->springs = (muscle*) eMalloc(sizeof(muscle) * currentGenome->iData[CUE::numSprings]);
                break;
            case 'b':
                components->blocks[numBlocks].loc.x = currentGenome->fData[TE::x];
                components->blocks[numBlocks].loc.y = currentGenome->fData[TE::y];
                components->blocks[numBlocks].loc.z = currentGenome->fData[TE::z];
                components->blocks[numBlocks].mass = 1;
                components->blocks[numBlocks].vel = zero();

                numBlocks++;
                break;
            case 's':
                a = components->springs[numSprings].a = currentGenome->iData[0];
                b = components->springs[numSprings].b = currentGenome->iData[1];
                numSprings++;
                break;
            default:
                quit(CREATURE_ERROR);
        }
        currentGenome = currentGenome->next;
    }
    for (int i = 0; i < numSprings; i++) {
        int a = components->springs[i].a;
        int b = components->springs[i].b;
        components->springs[i].origLength = euc(components->blocks[a].loc, components->blocks[b].loc);
    }

    newBorn->genome->iData[CUE::numSprings] = numSprings;
    newBorn->fitness= 0.0;
    components->origin = getCom(*newBorn);
    printf("%d\n", ((cube*) newBorn->components)->springs[4].a);
	return;
}

































