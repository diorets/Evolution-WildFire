#include "GameModes/gameModes.h"

#include "Glut/basic.h"              // KeyOperations
#include "global.h"                  // GameMode
#include "GameModes/Simulate/all.h"  // Mode
#include "GameModes/MainMenu/menu.h" // Mode

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


#include "Glut/myGlut.h"           // Glut
#include "Visual/basic.h"          // 2D, 3D, Various Shapes & Text
#include "Visual/objects.h"        // Drawing Objects
#include <stdio.h>
void drawDetails(int genSize, int gen, int id, int simTime, int maxTime) {
    /* General Stats */
    glColor3f(BLACK);
    char str[20];
    drawText("Press: 1 -> Toggle Help, 2 -> Toggle Creature Data, 3-> Toggle Graph",0,20,false);
    sprintf(str, "Generation:  %d", gen);
    int height = 20;
    drawText(str, 2, height += 20, false);
    sprintf(str, "Creature ID: %d / %d (%.2f%%)", id, genSize, 100 * id / (double) genSize);
    drawText(str, 5, height += 20, false);
    if (simTime < 1000000) {
        sprintf(str, "Time: %d / %d (%.2f%%)", simTime, maxTime, 100 * simTime / (double) maxTime);
    } else {
        sprintf(str, "Time: Error, value too large.");
    }
    drawText(str, 5, height += 20, false);
    sprintf(str, "Playback Speed: %dx", playBackSpeed);
    drawText(str, 5, height += 20, false);


}

#include "Functional/sleep.h"
void drawSystem(creature input, int genSize, int gen, int id, int simTime, int maxTime) {
    /* Clear, Reset, Camera */
    glutDrawing();

    /* 3D Drawing */
    reenable3D();
    drawGround();
    drawSun();
    drawCreature(input, false);

    /* 2D drawing */
    enable2D();
    drawDetails(genSize, gen, id, simTime, maxTime);
    //drawDisGraph(false, false);
    glutSwapBuffers(); // Make it all visible
    return;
}




#include "Generation/genes.h"
#include "Generation/generation.h"
#include "Functional/list.h"
#include "ErrorHandle/error.h"

#include <stdlib.h>
#include "Generation/mutations.h"
creature * initGenome(int a, int b, int c, int d) {
    creature * sample = (creature*) malloc(sizeof(creature));
    if (sample == NULL) {
        quit(GENOME_ERROR);
    }
    (*sample).genome = clearGenome((*sample).genome);
    if ((*sample).genome != NULL) {
        quit(GENOME_ERROR);
    }

    (*sample).genome = createGenome((*sample).genome, a, b, c, d);
    createCreature(sample);
    return sample;
}

#include "Math/myMath.h"
double getFitness(creature individual) {
    enum {planeDistance, upHill, downHill};
    int measure = environment[1];
    posi com = getCom(individual);
    posi origin = ((stickball*) individual.components)->origin;
    switch (measure) {
        case planeDistance : return euc2D(com, origin);
        case upHill        : return -sgn(environment[0]) * (com.x - origin.x);
        case downHill      : return sgn(environment[0]) * (com.x - origin.x);
        default : return euc2D(com, origin);
    }
}

void initPsuedoGlobal() {
    static bool initializing = true;
    if (initializing) {
        globalData = (generic*) malloc(sizeof(generic) * 10); // Increment me!
        if (globalData == NULL) quit(MALLOC_ERROR);
        globalData[skipE].b          = false;
        globalData[debugE].b         = false;
        globalData[instructionsE].b  = false; // Controls
        globalData[graphE].b         = false;
        globalData[howToE].i         = 0; //1 for intro
        globalData[goThroughGenE].b  = false;
        globalData[simPositionE].i   = 0; // 0 Sim, 1 Mut
        globalData[timeGenScreenE].b = false;
        globalData[displaySkinE].b   = false;
        globalData[quickGenE].i      = 0;
        initializing = false;
    }
    return;
}

creature * initPop(creature * population, int genSize, int gen, int id, int simTime, int allocationSize) {
    if (population == NULL || ((gen == 0) && (id == 0) && (simTime == 0))) {
        if (population == NULL) {
            population = (creature*) malloc(sizeof(creature) * genSize);
            if (population == NULL) quit(MALLOC_ERROR);

            for (int i = 0; i < genSize; i++) {
                population[i].fitness = 0.0;
                population[i].genome = NULL;

                population[i].components = (void*) malloc(allocationSize);
                if (population[i].components == NULL) {
                    quit(MALLOC_ERROR);
                }
            }
        }

        /* Clear Genomes from potentially previous runs */
        for (int i = 0; i < genSize; i++) {
            population[i].genome = clearGenome(population[i].genome);
        }

        for (int i = 0; i < genSize; i++) {
            if (population[i].genome != NULL) {
                quit(GENOME_ERROR);
            }
            population[i].genome = createGenome(population[i].genome, 8, 4, 4, 0);
            population[i].fitness = 0.0;
            createCreature(&population[i]);
        }
    }
    return population;
}
#include "physics.h"
#include "Generation/mutations.h"
#include "generation/duplicate.h"
#include "GameModes/inputFunctions.h"
void SIMULATION_MODE() {
    const int genSize = 100;
    const int maxTime = 10000;
    static creature * population = NULL;
    static int id = 0;
    static int gen = 0;
    static int simTime = 0;

    /* One-Time Initializations */
    initPsuedoGlobal();
    population = initPop(population, genSize, gen, id, simTime, sizeof(stickball)); /**/

    /* Graphics */
    if (playBackSpeed <= 0) {
        if (playBackSpeed-- == -10) {
            playBackSpeed = 0;
            drawSystem(population[id], genSize, gen, id, simTime, maxTime);
        }
        return;
    }
    //if ((simTime % 5 == 0) && !globalData[skipE].b && globalData[graphE].b) drawDisGraph(simTime == 0, true);
    if (display && !globalData[skipE].b) {
        if (!(simTime % playBackSpeed)) {
            drawSystem(population[id], genSize, gen, id, simTime, maxTime);
        }
    }



    simTime++;
    /* Evalute Fitness Function & Handle Creature Iteration */
    bool error = updateCreature(&population[id], simTime++);
    if (error) {
        population[id].fitness = -0.0;
        simTime = 0;
        id++;
    } else if (simTime % maxTime == 0) {
        population[id].fitness = getFitness(population[id]);

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
            createCreature(&population[i]);
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














