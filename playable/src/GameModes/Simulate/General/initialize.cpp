#include "GameModes/Simulate/General/initialize.h"
#include "ErrorHandle/error.h"
#include "Functional/list.h"
#include "global.h"

#include <stdio.h>
creature * initPop(creature * population, int genSize, int allocationSize) {
    if (population == NULL) {
        population = (creature*) malloc(sizeof(creature) * genSize);
        if (population == NULL) quit(MALLOC_ERROR);

        for (int i = 0; i < genSize; i++) {
            population[i].fitness = 0.0;
            population[i].genome = NULL;

            population[i].components = (void*) eMalloc(allocationSize);

            /* Clear Genomes from potentially previous runs */ // This doesnt make sense, because pop has to be null...
            population[i].genome = clearGenome(population[i].genome);
            if (population[i].genome != NULL) quit(GENOME_ERROR);
        }
    }
    return population;
}


void initPsuedoGlobal() {
    static bool initializing = true;
    if (initializing) {
        globalData = (generic*) malloc(sizeof(generic) * 13); // Increment me!
        if (globalData == NULL) quit(MALLOC_ERROR);
        globalData[skipE].b          = false;
        globalData[debugE].b         = false;
        globalData[instructionsE].b  = false; // Controls
        globalData[graphE].i         = 0;
        globalData[howToE].i         = 0; //1 for intro
        globalData[goThroughGenE].b  = false;
        globalData[simPositionE].i   = 0; // 0 Sim, 1 Mut
        globalData[timeGenScreenE].b = false;
        globalData[displaySkinE].b   = false;
        globalData[quickGenE].i      = 0;
        globalData[creatureFitnessE].g.points = NULL;
        globalData[creatureFitnessE].g.numEntries = 0;
        globalData[creatureFitnessE].g.display = false;

        globalData[generationFitnessE].g.points = NULL;
        globalData[generationFitnessE].g.numEntries = 0;
        globalData[generationFitnessE].g.display = false;

        // Dont forget to increment the allocated memory!
        initializing = false;
    }
    return;
}
















