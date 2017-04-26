#include "GameModes/gameModes.h"

#include "Glut/basic.h"              // KeyOperations
#include "global.h"                  // GameMode
#include "GameModes/MainMenu/menu.h" // Mode
#include "GameModes/Simulate/General/initialize.h"

void SIMULATION_MODE();

#include "Glut/myGlut.h"
#include <stdio.h>

void mainLoop() {
    callKeyboard('\0', true);
    switch (gameMode) {
        case startMode:
            startUpMode();
            break;
        case simMode:
            SIMULATION_MODE();
            break;
        default: break;
    }
    return;
}

void renderScene(void) {
    if (!display) {
        mainLoop();
    } else {
        const int msPerFrame = 1000.0 / 60.0;
        int init = glutGet(GLUT_ELAPSED_TIME);

        mainLoop();

        int total = glutGet(GLUT_ELAPSED_TIME);
        while (total - init < msPerFrame) {
            mainLoop();
            total = glutGet(GLUT_ELAPSED_TIME);
        }
        // this last call resets init to the current number of elapsed miliseconds.
        //init = glutGet(GLUT_ELAPSED_TIME);
    }
    return;
}


#include "Functional/sleep.h"





#include "GameModes/Simulate/StickBall/stickBallGenes.h"
#include "Functional/list.h"
#include "ErrorHandle/error.h"


#include "GameModes/inputFunctions.h"

#include "GameModes/Simulate/General/drawing.h"
#include "GameModes/Simulate/General/genetics.h"
#include "GameMOdes/Simulate/General/physics.h"

#include "GameModes/Simulate/StickBall/stickBallPhysics.h"
#include "GameModes/Simulate/StickBall/stickBallMutations.h"
#include "GameModes/Simulate/StickBall/stickBallPhysics.h"
#include <stdio.h>
#include <time.h>
#include "Visual/objects.h"
static creature * initializePop(creature * population, const unsigned int * sizes, int system, int genSize) {
     /* One-Time Initializations */
    initPsuedoGlobal();
    if (population == NULL) {
        population = initPop(population, genSize, sizes[system]);
        for (int i = 0; i < genSize; i++) {
            if (population[i].genome != NULL) quit(GENOME_ERROR);
            population[i].genome = createSystemGenome(system, population[i].genome);
            createSystemCreature(system, &population[i]);
            drawGround(50, 10, 20);
            // So rand from seed(0) not affected by tree drawing
        }
    }
    return population;
}

static void graphics(int system, creature * pop, int genSize, int gen, int id, int simTime, int maxTime) {
    if (playBackSpeed <= 0) {
        if (playBackSpeed-- == -10) {
            playBackSpeed = 0;
            drawSystem(system, pop, genSize, gen, id, simTime, maxTime);
        }
        return;
    }
    //if ((simTime % 5 == 0) && !globalData[skipE].b && globalData[graphE].b) drawDisGraph(simTime == 0, true);
    if (display && !globalData[skipE].b) {
        if (!(simTime % playBackSpeed)) {
            drawSystem(system, pop, genSize, gen, id, simTime, maxTime);
        }
    }
    return;
}

static void creatureIteration(bool error, creature * individual, int system, int * simTime, int * id, int maxTime) {
    if (error) {
        printf("Error\n");
        individual->fitness = -1.0;
        *simTime = 0;
        (*id)++;
    } else if ((*simTime) == maxTime) {
        individual->fitness = getFitness(system, *individual);

        if (globalData[skipE].b) {
            globalData[skipE].b = false;
        }
        *simTime = 0;
        (*id)++;
    }

}

static void applyGeneticOperators(int system, creature * population, int id, int genSize) {
    if (id == genSize) {
        srand(time(NULL));
        puts("New Generation");

        double avg = 0.0;
        for (int i = 0; i < genSize; i++) {
            avg += population[i].fitness;
        }
        avg /= genSize;

        int numEntries = globalData[generationFitnessE].g.numEntries;
        globalData[generationFitnessE].g.points = (double*) realloc(globalData[generationFitnessE].g.points, sizeof(double) * (1+numEntries));
        globalData[generationFitnessE].g.points[numEntries] = avg;
        globalData[generationFitnessE].g.numEntries++;

        /* Selection Function */
        int * ordered = orderedDist(population, genSize);
        pruneAndFill(ordered, population, genSize);
        if (ordered != NULL) {
            free(ordered);
        }

        /* Mutations */
        for (int i = 0; i < genSize; i++) {
            mutateGenome(system, &population[i]);
            createSystemCreature(system, &population[i]);
        }
    }
    return;
}

static void generationIteration(int genSize, int * id, int * gen) {
    if (*id == genSize) {
        if (globalData[goThroughGenE].b) {
            globalData[goThroughGenE].b = false;
            setPlayBackSpeed(2);
        }

        (*id) = 0;
        (*gen)++;
    }
    return;
}

void SIMULATION_MODE() {
    static const int genSize = 300;
    static const int maxTime = 9000;
    static creature * population = NULL;
    static int id = 0;
    static int gen = 0;
    static int simTime = 0;

    static const unsigned int creatureSizes[] = {sizeof(stickball), sizeof(turbine), sizeof(cannon)};
    static const int system = stickballE;

    population = initializePop(population, creatureSizes, system, genSize);
    graphics(system, population, genSize, gen, id, simTime, maxTime);

    if (playBackSpeed <= 0) return;

    creatureIteration(updateSystem(system, &population[id], &simTime), &population[id], system, &simTime, &id, maxTime);
    applyGeneticOperators(system, population, id, genSize);
    generationIteration(genSize, &id, &gen);
    return;
}














