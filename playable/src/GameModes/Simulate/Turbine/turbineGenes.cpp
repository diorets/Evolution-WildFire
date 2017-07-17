#include "GameModes/Simulate/Turbine/turbineGenes.h"
#include "GameModes/Simulate/Turbine/turbineMutations.h"
#include "ErrorHandle/error.h"

#include "Functional/list.h"
#include "Math/myMath.h"
#include <math.h>

#include <stdio.h>
#include <float.h>

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

        for (int trials = 0; trials < 100; trials++) {
            /* Generate New Random Location */
            newLoc.x = initLoc.x + 2 * radius * ((rand() % 3) ? 1 - rand() % 3:0);
            newLoc.y = initLoc.y + 2 * radius * ((rand() % 3) ? 1 - rand() % 3:0);
            newLoc.z = initLoc.z + 2 * radius * ((rand() % 3) ? 1 - rand() % 3:0);

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
                return newLoc;
            }
        }
    }
    return vec(0, 0, -10);
}

/* Genes */
static gene* infoGene(int n, int p) {
    gene * newInfo = (gene*) malloc(sizeof(gene));
    if (newInfo == NULL) quit(MALLOC_ERROR);

    newInfo->start = 'i';
    newInfo->fData[TE::radius] = 0.5;

    newInfo->iData[TE::numBlocks] = n;
    newInfo->iData[TE::numParticles] = p;
    newInfo->iData[TE::numGenes] = n + p + 1; // Total
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

    blockG->fData[TE::theta] = atan2( valid.y, valid.x);

    blockG->endof = '\0';
    blockG->next = NULL;
    return blockG;
}

#include <math.h>
static gene * blockGene(posi loc) { // Does check valid location
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

static gene * particleGene() {
    gene * particle = (gene*) malloc(sizeof(gene));
    if (particle == NULL) quit(MALLOC_ERROR);
    particle->start = 'p';

    particle->fData[TE::x] = TE::xlow;
    particle->fData[TE::y] = randf2(TE::ylow, TE::yhigh);
    particle->fData[TE::z] = randf2(TE::zlow, TE::zhigh);

    particle->fData[TE::vx] = randf(20) + 3;
    particle->fData[TE::vy] = pmRandf(2);
    particle->fData[TE::vz] = pmRandf(2);

    particle->endof = '\0';
    particle->next = NULL;

    return particle;
}


gene * createTurbineGenome(gene * head) {
    printf("Creating Genome: %d\n", rand() % 10);
	if (head != NULL) quit(GENOME_ERROR);
	int n = 100;
	int p = 1000;
    head = infoGene(n, p);

    head = addToBack(head, blockGene(vec(0.0, 0.0, 20.0)));
    for (int i = 1; i < n; i++) {
        head = addToBack(head, blockGene(head, i));
    }
    for (int i = 0; i < p; i++) {
        head = addToBack(head, particleGene());
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

void createTurbineCreature(creature * newBorn) {
    int numBlocks = 0;
    int numParticles = 0;
    gene * currentGenome = newBorn->genome;
    turbine * components = ((turbine*) newBorn->components);

    while (currentGenome != NULL) {
        switch (currentGenome->start) {
            case 'i': // Needs to free doesnt it?
                components->blocks = (block*) eMalloc(sizeof(block) * currentGenome->iData[TE::numBlocks]);
                components->particles = (node*) eMalloc(sizeof(node) * currentGenome->iData[TE::numParticles]);
                break;
            case 'b':
                components->blocks[numBlocks].loc.x = currentGenome->fData[TE::x];
                components->blocks[numBlocks].loc.y = currentGenome->fData[TE::y];
                components->blocks[numBlocks].loc.z = currentGenome->fData[TE::z];

                components->blocks[numBlocks].angle = currentGenome->fData[TE::theta];

                components->blocks[numBlocks].angSpeed = 0.0;
                components->blocks[numBlocks].torque   = 0.0;

                numBlocks++;
                break;
            case 'p':
                components->particles[numParticles].loc.x = currentGenome->fData[TE::x];
                components->particles[numParticles].loc.y = currentGenome->fData[TE::y];
                components->particles[numParticles].loc.z = currentGenome->fData[TE::z];

                components->particles[numParticles].vel.x = currentGenome->fData[TE::vx];
                components->particles[numParticles].vel.y = currentGenome->fData[TE::vy];
                components->particles[numParticles].vel.z = currentGenome->fData[TE::vz];

                components->particles[numParticles].force = zero();

                numParticles++;
                break;
            default:
                quit(CREATURE_ERROR);
        }
        currentGenome = currentGenome->next;
    }
    newBorn->fitness= 0.0;
	return;
}

































