#include "GameModes/Simulate/General/genetics.h"
#include "GameModes/Simulate/StickBall/stickBallGenes.h"
#include "GameModes/Simulate/Turbine/turbineGenes.h"
#include "GameModes/Simulate/Cannon/cannonGenes.h"
#include "GameModes/Simulate/Cube/cubeGenes.h"

#include "GameModes/Simulate/StickBall/stickBallMutations.h"
#include "GameModes/Simulate/Turbine/turbineMutations.h"
#include "GameModes/Simulate/Cannon/cannonMutations.h"
#include "GameModes/Simulate/Cube/cubeMutations.h"

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
        case turbineE  : return createTurbineGenome(genome);
        case cannonE   : return createCannonGenome(genome);
        case cubeE   : return createCubeGenome(genome);
        default: quit(GENOME_ERROR); return NULL;
    }
}

void createSystemCreature(int system, creature * individual) {
    switch(system) {
        case stickballE: createStickBallCreature(individual); return;
        case turbineE  : createTurbineCreature(individual); return;
        case cannonE   : createCannonCreature(individual); return;
        case cubeE   : createCubeCreature(individual); return;
        default: quit(CREATURE_ERROR); return;
    }
}

void mutateGenome(int system, creature * toMutate) {
    switch(system) {
        case stickballE: mutateStickball(toMutate); return;
        case turbineE  : mutateTurbine(toMutate); return;
        case cannonE   : mutateCannon(toMutate); return;
        case cubeE     : mutateCube(toMutate); return;
        default: quit(CREATURE_ERROR); return;
    }
}



#include"Math/myMath.h"

double skewedUnimodal(double x) {
    return pow(1 - x, environment[0]) * sin(PI* x); // environment[0] -> 27
}

double boxDistribution(double x) {
    double val = 0.0;
    double sumAlphas = 0.0;
    int n = 10;
    for (int i = 0 ; i < n; i++) {
        val += (((i / (double) n) < x) && (x < (i+1) / (double) n)) ? coeffs[i] : 0;
        sumAlphas += coeffs[i];
    }
    val *= n / sumAlphas; // normalization factor
    return val;
}

double exponential(double x) {
    return 0.29 * exp(-4.1 * x);
}

double selectionDistribution(int genSize, bool trialSelectionFunction) {
//    return getDistribution(exponential, genSize, 0); // used to run simulation

    return getDistribution(skewedUnimodal, genSize, environment[0]);

    if (!trialSelectionFunction) { // Used in testing
        return getDistribution(skewedUnimodal, genSize, environment[0]);
    } else {
        return getDistribution(boxDistribution, genSize, 0);
    }
}


int getRandomID(int * orderedPop, int genSize) {
    /* Duplicate Top Half (hopefully, untested) */
//    static int i = 0;
//    static int firstCopy = 0;
//
//    int toReturn = orderedPop[i];
//    i = (i + firstCopy) % (genSize/2);
//    firstCopy = firstCopy == 0 ? 1 : 0;
////    printf("%d\n", i);
//    return orderedPop[i];

//    return orderedPop[0]; // return THE best creature
    int genomeID = (int) selectionDistribution(genSize, false); // use known
    genomeID = genomeID >= genSize ? genSize - 1: genomeID;
    genomeID = genomeID  < 0       ?           0: genomeID;
    return orderedPop[genomeID];
}

gene * getGenome(int genomeID, creature * population) {
    gene * genome = (gene*) malloc(sizeof(gene) * population[genomeID].genome->iData[0]);
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
        temp[i] = NULL;
        temp[i] = getGenome(getRandomID(orderedPop, genSize), population);
    }

    // Clear Population
    for (int i = 0; i < genSize; i++) {
        population[i].genome = clearGenome(population[i].genome);
    }



    // Populate Population
    for (int i = 0; i < genSize; i++) {
        if (population[i].genome != NULL) quit(GENOME_ERROR);
        if (temp[i]              == NULL) quit(GENOME_ERROR);

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
            if ((fabs(dist[i].order - dist[j].unord) <= 2*DBL_MIN)) {
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
            orderedIDs[i] = rand() % 200;
            puts("orderedIDs Error, a fix has been made to avoid program termination.");
//            quit(orderedIDs[i]);
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


void saveGenome(creature * population, int gen, int index, char file[256]) {
    FILE * fptr = fopen(file, "a");
    fprintf(fptr, "%d:", gen);
    fprintf(fptr, "F:%f>", population[index].fitness);
    for (gene * current = population[index].genome; current != NULL; current = current->next) {
        fprintf(fptr, "<%c|", current->start);
        switch (current->start) {
            case 'i':
                for (int i = 1; i < 5; i++) { // "1" to exclude tot because its calculated manually
                    fprintf(fptr, "%d,", current->iData[i]);
                }
                break;
            case 'n':
                for (int i = 0; i < 5; i++) {
                    fprintf(fptr, "%f,", current->fData[i]);
                }
                break;
            case 'm':
                fprintf(fptr, "%d,%d,%f,%f,", current->iData[0], current->iData[1], current->fData[0], current->fData[1]);
                break;
            case 'b':
                fprintf(fptr, "%d,%d,", current->iData[0], current->iData[1]);
                break;
            default:
                quit(GENOME_ERROR);
                break;
        }
    }
    fprintf(fptr, "<\n");
    fclose(fptr);
    return;
}








