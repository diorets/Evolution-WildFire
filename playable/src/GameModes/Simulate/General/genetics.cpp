#include "GameModes/Simulate/General/genetics.h"
#include "GameModes/Simulate/SpGenetics/stickBallGenes.h"

#include <math.h>  // Fabs
#include <stdio.h> // Sprintf
#include <float.h> // DLB_MIN (smallest float)
#include "preproccessor.h"     // Structs
#include "global.h"            // Global
#include "Functional/list.h"   // Clear Genome
#include "Glut/myGlut.h"       // Glut
#include "Visual/basic.h"      // 2D, 3D, Various Shapes & Text
#include "ErrorHandle/error.h" // Quit
#include "Math/distribution.h" // Distribution Functions

#define PI 3.141592653589793

gene * createSystemGenome(int system, gene * genome) {
    switch(system) {
        case stickballE: return createStickBallGenome(genome);
        default: return NULL;
    }
}

void createSystemCreature(int system, creature * individual) {
    switch(system) {
        case stickballE: createStickBallCreature(individual); break;
        default: break;
    }
}

double skewedUnimodal(double x) {
    return pow(1 - x, 27) * sin(PI* x);
}

double selectionDistribution(int genSize) {
    return getDistribution(skewedUnimodal, genSize, 27);
}


int getRandomID(int * orderedPop, int genSize) {
    int genomeID = (int) selectionDistribution(genSize);
    genomeID = genomeID >= genSize ? genSize - 1: genomeID;
    genomeID = genomeID  < 0       ?           0: genomeID;
    return orderedPop[genomeID];
}

gene * getGenome(int genomeID, creature * population) {
    gene * genome = (gene*) malloc(sizeof(gene) * population[genomeID].genome->iData[tot]);
    gene * head = genome;
    if (genome == NULL) quit(MALLOC_ERROR);

    gene * curr = population[genomeID].genome;

    while (curr) {
        copyGene(genome, curr);

        curr  = curr->next;
        genome = genome->next;
    }
    return head;
}

void pruneAndFill(int * orderedPop, creature * population, int genSize) {
    gene ** temp = (gene**) malloc(sizeof(gene*) * genSize);

    // Fill temp with random genomes
    for (int i = 0; i < genSize; i++) {
        temp[i] = getGenome(getRandomID(orderedPop, genSize), population);
    }

    // Clear Population
    for (int i = 0; i < genSize; i++) {
        population[i].genome = clearGenome(population[i].genome);
    }

    // Populate Population
    for (int i = 0; i < genSize; i++) {
        population[i].genome = (gene*) malloc(sizeof(gene));
        if (population[i].genome == NULL) quit(MALLOC_ERROR);
        gene * replaceGene  = population[i].genome;
        gene * fillWithGene = temp[i];

        if (fillWithGene == NULL) quit(MALLOC_ERROR);

        /* Copy the Genome */
        while (fillWithGene) {
            copyGene(replaceGene, fillWithGene);

            replaceGene  = replaceGene->next;
            fillWithGene = fillWithGene->next;
        }
    }

    /* Free Temp */
    for (int i = 0; i < genSize; i++) {
        if (temp[i] != NULL) {
            temp[i] = clearGenome(temp[i]);
        }
    }
    free(temp);
    return;
}

void copyGene(gene * copyInto, gene * copyFrom) {
    /* Error Checking */
    if (copyInto == NULL) quit(MALLOC_ERROR);
    if (copyFrom == NULL) quit(MALLOC_ERROR);

    /* Copy Gene Contents */
    copyInto->start = copyFrom->start;
    for (int i = 0; i < 10; i++) { // expand for more mem in i data
        copyInto->iData[i] = copyFrom->iData[i];
        copyInto->fData[i] = copyFrom->fData[i];
    }
    copyInto->endof = copyFrom->endof;

    /* Determine Next Ptr */
    if (copyFrom->next == NULL) {
        copyInto->next = NULL;
    } else {
        copyInto->next = (gene*) malloc(sizeof(gene));
        if (copyInto->next == NULL) quit(MALLOC_ERROR);
    }
    return;
}

int * orderedDist(creature * group, int genSize) {
    ord * dist    = (ord*) malloc(sizeof(ord) * genSize);
    int * orderedIDs = (int*) malloc(sizeof(int) * genSize);

    if (dist       == NULL) quit(MALLOC_ERROR);
    if (orderedIDs == NULL) quit(MALLOC_ERROR);

    for (int i = 0; i < genSize; i++) {
        orderedIDs[i] = -1; // Error code for non-initialization
    }

/* Use Dummy so as to not overwrite */
    for (int i = 0; i < genSize; i++) {
        dist[i].unord = group[i].fitness;
    }

/* Get the Organized Array */
    for (int i = 0; i < genSize; i++) {
        for (int j = i + 1; j < genSize; j++) {
            if (dist[i].unord < dist[j].unord) {
                double a =  dist[i].unord;
                dist[i].unord = dist[j].unord;
                dist[j].unord = a;
            }
        }
    }

/* Write into ordered array */
    for (int i = 0; i < genSize; i++) {
        dist[i].order = dist[i].unord;
    }

/* Use Dummy so as to not overwrite */
    for (int i = 0; i < genSize; i++) {
        dist[i].unord = group[i].fitness;
    }

/* Record id.copie in order of highest distance */
    bool * beenRecorded = (bool*) malloc(sizeof(bool) * genSize);
    if (beenRecorded == NULL) quit(MALLOC_ERROR);

    for (int i = 0; i < genSize; i++) {
        beenRecorded[i] = false;
    }

/* Record Top Half Distances into Array */
    for (int i = 0; i < genSize; i++) {
        for (int j = 0; j < genSize; j++) {
            if ((fabs(dist[i].order - dist[j].unord) <= DBL_MIN)) {
                /* Determine if already recorded */
                if (beenRecorded[j]) continue;
                else beenRecorded[j] = true;

                orderedIDs[i] = j;

                /* Break */
                j = genSize;
            }
        }
    }

/* Error Checking and Freeing */
    for (int i = 0; i < genSize; i++) {
        if (orderedIDs[i] > genSize) {
            quit(orderedIDs[i]);
        }
        if (orderedIDs[i] < 0) {
            quit(orderedIDs[i]);
        }
    }

    if (dist != NULL) {
        free(dist);
    }
    if (beenRecorded != NULL) {
        free(beenRecorded);
    }
    return orderedIDs;
}











