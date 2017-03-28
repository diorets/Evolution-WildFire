#include "GameModes/Simulate/all.h"

#include <stdio.h>                 // sprintf
#include <math.h>                  // fabs
#include "global.h"                // Globals
#include "physics.h"               // UpdateCreature
#include "Math/myMath.h"           // Com, Cov, euc2D
#include "Generation/duplicate.h"  // OrderDistances, Duplicate
#include "Generation/generation.h" // CreateCreature
#include "Generation/mutations.h"  // MutateGenome
#include "Visual/basic.h"          // 2D, 3D, Various Shapes & Text
#include "Visual/objects.h"        // Drawing Objects
#include "Glut/myGlut.h"           // Glut
#include "ErrorHandle/error.h"     // Quit
#include "Functional/sleep.h"      // Sleep_ms
#include "GameModes/inputFunctions.h" //
#include "GameModes/mice.h"           //
#include "GameModes/Simulate/drawing.h"
double selectionPower = 27.5;

void newGenScreen();
#include "Math/distribution.h"
#define PI 3.141592653589793

double box(double x) {
    return x <= selectionPower ? 1 : 0;
}

double bimodal(double x) {
    return pow(1 - x, 27.5) * sin(PI* x) + 0.01 * exp(-500 * (x-selectionPower) * (x-selectionPower));
}

double skewedUnimodal(double x) {
    return pow(1 - x, selectionPower) * sin(PI* x);
}

double powerOfUniform(double power) {
    return (genSize * pow(randf(1.0), power)); // number from random distribution
}

double selectionDistribution() {
    return getDistribution(skewedUnimodal, genSize, selectionPower);
}


void simulationMode() {
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

    if (simTime % 1000 == 0) {
        sleep_ms(processSlowDown);
    }

    if (globalData[simPositionE].i == 0) {
        if (display && !globalData[skipE].b && !globalData[quickGenE].i) draw();
        if (!simulate())         newCreature(true);
        if (simTime == MAX_TIME) newCreature(false);
        if (id      == genSize)  newGeneration();
    } else {
        globalData[simPositionE].i = 0;
        if (dataCollection == 0) {
            newGenScreen();
        }
    }

    if (globalData[quickGenE].i > 0) {
        if (simTime == 8000) {
            glutDrawing();
            enable2D();
            glColor3f(BLACK);
            write("Generation: %d, (%%%.2f) of current, %d Generations remaining", 500, 500, gen,100 * (id) / ((double) genSize), globalData[quickGenE].i);
            reenable3D();
            glutSwapBuffers();
        }
    }
    return;
}





void newGenScreen() {
    glutDrawing();
    enable2D();

    glColor4f(RED, 0.4);
    drawSquare(wx * 0.02, wy * 0.04, wx * 0.98, wy * 0.195);
    glColor3f(RED);
    drawHeader("You've Completed a Generation!", wx / 2, wy * 0.1);
    glColor3f(BLACK);
    double last1 = writeParagraph("Now, you must decide how to pick the next generation\n"
                   "How would you like to mutate them? Can you evolve a fast creature with 6 nodes?"
                   "", wx / 2, wy * 0.13, 24);
    last1 *= 1.2;

    glColor4f(BLUE, 0.4);

    drawSquare(wx * 0.02, last1 - 12,  wx * 0.98, last1 + 24 * 14);
    drawHeader("MUTATIONS:", wx * 0.1, last1 + 24);
    drawHeader("POPULATION STATISTICS", wx * 0.5, last1 + 24);
    drawHeader("SELECT BY", wx * 0.8, last1 + 24);

    glColor3f(BLACK);
    /* Population Statistics */
    double avgNodes   = 0.0;
    double avgMuscles = 0.0;
    double avgBones   = 0.0;
    for (int i = 0; i < genSize; i++) {
        FOR_ALL(specimen[i].genome, 'n') {
            avgNodes++;
        }
        FOR_ALL(specimen[i].genome, 'm') {
            avgMuscles++;
        }
        FOR_ALL(specimen[i].genome, 'b') {
            avgBones++;
        }
    }
    avgNodes   /= (double) genSize;
    avgMuscles /= (double) genSize;
    avgBones   /= (double) genSize;
    write(" Average Number of Parts    : %.2f", wx * 0.55, last1 + 50 + 24 * 1, avgNodes + avgMuscles + avgBones);
    write("Average Number of Nodes   : %.2f",   wx * 0.55, last1 + 50 + 24 * 2, avgNodes);
    write(" Average Number of Muscles : %.2f",  wx * 0.55, last1 + 50 + 24 * 3, avgMuscles);
    write("Average Number of Bones   : %.2f",   wx * 0.55, last1 + 50 + 24 * 4, avgBones);

//    double last2 =
    writeParagraph(
                   "Shuffle Nodes\n"
                   "Relocate Nodes\n"
                   " \n"
                   "Add Nodes\n"
                   "Remove Nodes\n"
                   " \n"
                   "Add Connections\n"
                   "Remove Connections\n"
                   "Swap Connections\n"
                   "", wx * 0.1, last1 + 24 * 4, 24);
    write("%%%5.2f", wx * 0.2, last1 + 24 *  4, specimen[0].genome->fData[0]);
    write("%%%5.2f", wx * 0.2, last1 + 24 *  5, specimen[0].genome->fData[1]);
    write("%%%5.2f", wx * 0.2, last1 + 24 *  7, specimen[0].genome->fData[2]);
    write("%%%5.2f", wx * 0.2, last1 + 24 *  8, specimen[0].genome->fData[3]);
    write("%%%5.2f", wx * 0.2, last1 + 24 * 10, specimen[0].genome->fData[4]);
    write("%%%5.2f", wx * 0.2, last1 + 24 * 11, specimen[0].genome->fData[5]);
    write("%%%5.2f", wx * 0.2, last1 + 24 * 12, specimen[0].genome->fData[6]);

    for (int i = 0; i < 9; i++) {
        if ((i == 2) || (i == 5)) continue;
        int x1 = wx * 0.17;
        int x2 = wx * 0.23;
        int y =  last1 + 24 * (i + 4) - 5;
        int r = 11;

        if (bound(mousePos.x, mousePos.y, x1-r, y-r, x1+r, y+r)) {
            glColor3f(WHITE);
        } else glColor3f(RED);
        drawCircle(x1, y, r, 20);

        if (bound(mousePos.x, mousePos.y, x2-r, y-r, x2+r, y+r)) {
            glColor3f(WHITE);
        } else glColor3f(GREEN);
        drawCircle(x2, y, r, 20);
    }

    /* Normal Button */
    int x = wx * 0.35;
    int y = wy * 0.7;
    int r = 20;
    if (bound(mousePos.x, mousePos.y, x-r, y-r, x+r, y+r)) {
            glColor3f(BLACK);
    } else glColor3f(RED);
    drawCircle(x, y, r, 20);
    drawText("Normal", x, y * 0.9, true);

    /* Skip 3 Gens */
    x = wx * 0.5;
    r = 20;
    if (bound(mousePos.x, mousePos.y, x-r, y-r, x+r, y+r)) {
        glColor3f(BLACK);
    } else glColor3f(RED);
    drawCircle(x, y, r, 20);
    drawText("Skip 3 Generations", x, y * 0.9, true);

    /* Time Screen */
    x = wx * 0.65;
    r = 20;
    static int counter = 0;
    if (globalData[timeGenScreenE].b) {
        counter++;
        glColor3f(GREEN);
        if (counter % 100 == 0) {  // 100%
            globalData[simPositionE].i = 0;
            counter = 0;
        }
    } else glColor3f(RED);

    if (bound(mousePos.x, mousePos.y, x-r, y-r, x+r, y+r)) {
        glColor3f(BLACK);
    }
    drawCircle(x, y, r, 20);
    write("Time This Screen %%%.1f", x, y * 0.9, (float) counter);

    glColor3f(BLACK);
    drawText("Distance From Origin", wx * 0.85, last1 + 50 + 24, true);
    drawText("Distance Up Slope",    wx * 0.85, last1 + 50 + 48, true);
    drawText("Distance Down Slope",  wx * 0.85, last1 + 50 + 72, true);



    x = wx * 0.9;
    y = last1 + 50 - 5 + 24;
    r = 10;
    if (environment[1] < 0.5 && environment[1] > -0.5) glColor3f(GREEN); else glColor3f(RED);
    if (bound(mousePos.x, mousePos.y, x-r, y-r, x+r, y+r)) {
        glColor3f(WHITE);
    }
    drawCircle(x, y, r, 20);

    y += 24;
    if (environment[1] < 1.5 && environment[1] > 0.5) glColor3f(GREEN); else glColor3f(RED);
    if (bound(mousePos.x, mousePos.y, x-r, y-r, x+r, y+r)) {
        glColor3f(WHITE);
    }
    drawCircle(x, y, r, 20);

    y +=24;
    if (environment[1] < 2.5 && environment[1] > 1.5) glColor3f(GREEN); else glColor3f(RED);
    if (bound(mousePos.x, mousePos.y, x-r, y-r, x+r, y+r)) {
        glColor3f(WHITE);
    }
    drawCircle(x, y, r, 20);

    genDisGraph(-100, false);
    glutSwapBuffers(); // Make it all visible
    reenable3D();
}

























































void lowerCreature() { // This can be made faster (find lowest, calc dis, sub all)
    bool rightPlace = false; // needs to account for world rotation
    int len = specimen[id].genome->iData[nod];
    while (true) {
        for (int i = 0; i < len; i++) {
            specimen[id].nodes[i].loc.z -= 0.01;
            if (specimen[id].nodes[i].loc.z <= 0.75 + 0.1) {
                rightPlace = true;
            }
        }
        if (rightPlace) break;
    }
    return;
}

bool simulate() {
    if (simTime == 0) {
//        lowerCreature(); // doesnt account for world rotation
	}
    if (updateCreature(&specimen[id + 0], simTime)) return false; // Error
//    if (simTime == (int)(MAX_TIME * 0.2)) { // Give a few seconds to fall over
//        specimen[id].origin = getCom(specimen[id]);
//    }
    simTime++;
    return true;
}


double getFitness(posi com) {
    enum {planeDistance, upHill, downHill};
    int measure = environment[1];
    switch (measure) {
        case planeDistance : return euc2D(com, specimen[id].origin);
        case upHill        : return -sgn(environment[0]) * (com.x - specimen[id].origin.x);
        case downHill      : return sgn(environment[0]) * (com.x - specimen[id].origin.x);
        default : return euc2D(com, specimen[id].origin);
    }
}

void newCreature(bool error) {

    /* Get Distances */
    if (!error) specimen[id].distance = getFitness(getCom(specimen[id]));
    else {
        static int numErrors = 0; numErrors++;
        specimen[id].distance = -0.0;
        printf("Error %d\n", numErrors);
    }

    /* Go To Next Creature */
    simTime = 0;
    id++;

    if (globalData[skipE].b) {
        globalData[skipE].b = false;
    }
    return;
}
























#include <float.h>
int * orderedDist(creature * group) {
    ord * dist    = (ord*) malloc(sizeof(ord) * genSize);
    int * orderedIDs = (int*) malloc(sizeof(int) * genSize);

    if (dist       == NULL) quit(MALLOC_ERROR);
    if (orderedIDs == NULL) quit(MALLOC_ERROR);

    for (int i = 0; i < genSize; i++) {
        orderedIDs[i] = -1; // Error code for non-initialization
    }

/* Use Dummy so as to not overwrite */
    for (int i = 0; i < genSize; i++) {
        dist[i].unord = group[i].distance;
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
        dist[i].unord = group[i].distance;
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
















#include "Functional/list.h"

int getRandomID(int * orderedPop) {
    int genomeID = (int) selectionDistribution();
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

void pruneAndFill(int * orderedPop, creature * population) {
    gene ** temp = (gene**) malloc(sizeof(gene*) * genSize);

    // Fill temp with random genomes
    for (int i = 0; i < genSize; i++) {
        temp[i] = getGenome(getRandomID(orderedPop), population);
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

void recordGenFitness(creature * population) {
    FILE * fptr = fopen("../assets/fitnesses.txt", "a");

    float avg = 0.0;
    for (int i = 0; i < genSize; i++) {
        avg += population[i].distance;
    }
    avg /= genSize;
    fprintf(fptr, "%f\n", avg);
    fclose(fptr);
    return;
}

void printStats(FILE * fptr, creature * samples, double avg, int len) {
    double plusSD = 0.0;
    double minusSD = 0.0;

    /* Mean */
    double meanImpact = 0.0;
    for (int i = 0; i < len; i++) {
        double impact = samples[i].distance - avg;
        meanImpact += impact;
    }
    meanImpact /= len;

    /* +SD */
    double standardDeviation = 0.0;
    int counter = 0;
    for (int i = 0; i < len; i++) {
        if (samples[i].distance > avg) { // If increased
            double impact = samples[i].distance - avg;
            standardDeviation += pow(impact - meanImpact, 2);
            counter++;
        }
    }
    if (counter <= 1) plusSD = -1.0;
    plusSD =  sqrt(standardDeviation/(counter - 1));


    /* -SD */
    standardDeviation = 0.0;
    counter = 0;
    for (int i = 0; i < len; i++) {
        if (samples[i].distance < avg) {
            double impact = samples[i].distance - avg;
            standardDeviation += pow(impact - meanImpact, 2);
            counter++;
        }
    }
    if (counter <= 1) minusSD = -1.0;
    minusSD =  sqrt(standardDeviation/(counter - 1));

    fprintf(fptr, "%f %f %f\n", meanImpact, plusSD, minusSD);
    return;
}

bool recordGenImpact(creature * population) {
    FILE * fptr = fopen("../assets/fitnesses.txt", "a");
    static float avg = 0.0;
    if (gen % 2 == 0) {
        for (int i = 0; i < genSize; i++) {
            avg += population[i].distance;
        }
        avg /= genSize;

        fprintf(fptr, "%f ", avg);
        fclose(fptr);
        return false;
    } else {
        printStats(fptr, population, avg, genSize);
//        for (int i = 0; i < genSize; i++) {
//            if (fabs(avg - population[i].distance) > 0.000001 ) { // Creature had mutation that affected fitness. I assume this means all mutations.
//                fprintf(fptr, "%f ", population[i].distance);
//            }
//        }
        //fprintf(fptr, "\n");
        fclose(fptr);
        return true;
    }

}

#include "Generation/genes.h"
void recordEveryGenome(creature * population, int step) {
    for (int i = 0; i < genSize; i += step) {
        saveGenome(population, i, population[i].distance);
    }
    return;
}

void newGeneration() {
    if (recordGenImpact(specimen)) {
        newGameMode(simMode);
        return; // So that we dont continue to modify the new population
    }
    int * ordered = orderedDist(specimen);
    pruneAndFill(ordered, specimen);
    if (ordered != NULL) {
        free(ordered);
    }

    /* Create New Generation */
    for (int i = 0; i < genSize; i++) {
        specimen[i].distance = 0.0;
        mutateGenome(&specimen[i]);
        createCreature(&specimen[i]);
    }

    /* Go To Next Generation */
    id = 0;
    gen++;
    if (globalData[goThroughGenE].b) {
        globalData[goThroughGenE].b = false;
        setPlayBackSpeed(2);
    }
    if (globalData[simPositionE].i == 0) {
        globalData[simPositionE].i = 1;
    }
    if (globalData[quickGenE].i > 0) {
        globalData[quickGenE].i --;
        globalData[simPositionE].i = 0;
    }
    if (globalData[quickGenE].i == 0) glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    return;
}







