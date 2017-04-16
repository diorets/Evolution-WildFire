#include "GameModes/gameModes.h"

#include "Glut/basic.h"              // KeyOperations
#include "global.h"                  // GameMode
#include "GameModes/MainMenu/menu.h" // Mode
#include "GameModes/Simulate/General/initialize.h"

void SIMULATION_MODE();

void renderScene(void) {
    callKeyboard('\0', true);
    switch (gameMode) {
        case startMode:
            startUpMode();
            return;
        case simMode:
            SIMULATION_MODE();
            return;
        default : return;
    }
}


#include "Functional/sleep.h"





#include "GameModes/Simulate/SpGenetics/stickBallGenes.h"
#include "Functional/list.h"
#include "ErrorHandle/error.h"


#include "GameModes/inputFunctions.h"
#include "GameModes/Simulate/General/drawing.h"
#include "GameModes/Simulate/General/genetics.h"
#include "GameMOdes/Simulate/General/physics.h"

#include "GameModes/Simulate/SpPhysics/stickBallPhysics.h"
#include "GameModes/Simulate/SpGenetics/stickBallMutations.h"
#include "GameModes/Simulate/SpPhysics/stickBallPhysics.h"
#include <stdio.h>
void SIMULATION_MODE() {
    const int genSize = 200;
    const int maxTime = 10000;
    static creature * population = NULL;
    static int id = 0;
    static int gen = 0;
    static int simTime = 0;

    static const unsigned int creatureSizes[] = {sizeof(stickball)};
    static const int system = stickballE;

    /* One-Time Initializations */
    initPsuedoGlobal();
    if (population == NULL || ((gen == 0) && (id == 0) && (simTime == 0))) {
        population = initPop(population, genSize, creatureSizes[system]);
        for (int i = 0; i < genSize; i++) {
            population[i].genome = createSystemGenome(system, population[i].genome);
            createSystemCreature(system, &population[i]);
        }
    }

    /* Graphics */
    if (playBackSpeed <= 0) {
        if (playBackSpeed-- == -10) {
            playBackSpeed = 0;
            drawSystem(system, population[id], genSize, gen, id, simTime, maxTime);
        }
        return;
    }
    //if ((simTime % 5 == 0) && !globalData[skipE].b && globalData[graphE].b) drawDisGraph(simTime == 0, true);
    if (display && !globalData[skipE].b) {
        if (!(simTime % playBackSpeed)) {
            drawSystem(system, population[id], genSize, gen, id, simTime, maxTime);
        }
    }
    simTime++;
    /* Evalute Fitness Function & Handle Creature Iteration */
    bool error = updateSystem(system, &population[id], simTime++); /**/
    if (error) {
        population[id].fitness = -0.0;
        simTime = 0;
        id++;
    } else if (simTime % maxTime == 0) {
        population[id].fitness = getFitness(system, population[id]); /**/

        if (globalData[skipE].b) {
            globalData[skipE].b = false;
        }
        simTime = 0;
        id++;
    }

    /* Apply Genetic Operators & Handle Generation Iteration */
    if (id == genSize - 1) {
        /* Selection Function */
        int * ordered = orderedDist(population, genSize);
        pruneAndFill(ordered, population, genSize);
        if (ordered != NULL) {
            free(ordered);
        }

        /* Mutations */
        for (int i = 0; i < genSize; i++) {
            mutateGenome(&population[i]);
            createSystemCreature(system, &population[i]);
        }

        /* Generation Handling */
        if (globalData[goThroughGenE].b) {
            globalData[goThroughGenE].b = false;
            setPlayBackSpeed(2);
        }
        id = 0;
        gen++;
    }
    return;
}














