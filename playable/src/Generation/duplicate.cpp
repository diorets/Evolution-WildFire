#include "Generation/duplicate.h"

#include <math.h>  // Fabs
#include <stdio.h> // Sprintf
#include "preproccessor.h"     // Structs
#include "global.h"            // Global
#include "Functional/list.h"   // Clear Genome
#include "Glut/myGlut.h"       // Glut
#include "Visual/basic.h"      // 2D, 3D, Various Shapes & Text
#include "ErrorHandle/error.h" // Quit
#include "Functional/sleep.h"

void genDisGraph(double avg, bool adding) {
    /* Initialize */
    static double *dis = NULL;
    static double maxDis = 0.0;
    if (dis == NULL) {
        dis  = (double*) malloc(sizeof(double));
    } else if (adding) {
        dis  = (double*) realloc(dis, sizeof(double) * (gen + 1));
    }
    if (dis == NULL) quit(MALLOC_ERROR);

    /* Get Max */
    if (adding) {
        dis[gen] = avg;
        if (fabs(dis[gen]) > maxDis) {
            maxDis = fabs(dis[gen]);
        }
    } else {

        /* Draw Background */
        glColor3f(BLACK);
        drawSquare(0, wy * 0.79, wx, wy);

        /* Draw Lines */
        char str[20];
        glColor3f(WHITE);
        glBegin(GL_LINES);
        glVertex2f(0, 0.9 * wy);
        glVertex2f(wx, 0.9 * wy);

        for (int i = 0; i < gen; i++) {
            if (gen == 1) {
                double x = 0;
                double X = 1;
                double y = 5 * dis[0];
                double Y = 5 * dis[0];
                glVertex2f(wx * x, 0.1 * wy * (-0.2*y/maxDis + 9));
                glVertex2f(wx * X, 0.1 * wy * (-0.2*Y/maxDis + 9));
            } else {
                double x = (i + 0) / (double) (gen - 1);
                double X = (i + 1) / (double) (gen - 1);
                double y = 5 * dis[i + 0];
                double Y = 5 * dis[i + 1];
                glVertex2f(wx * x, 0.1 * wy * (-0.2*y/maxDis + 9));
                glVertex2f(wx * X, 0.1 * wy * (-0.2*Y/maxDis + 9));
            }
        }
        glEnd();

        /* Measures */
        int numDiv = 8;
        for (int i = 0; i <= numDiv; i++) {
            sprintf(str, "%d(%d%%)", gen * i / numDiv, 100 *  i / numDiv);
            drawText(str, 0.95 * wx * i / numDiv, wy * 0.92, false);
        }
        glColor3f(BLACK);
        drawText("Distance vs Generation Graph", 0.5, wy * 0.78, false);
        glColor3f(WHITE);
        sprintf(str, "Max Dis: %0.2f", maxDis);
        drawText(str, 0.5, wy * 0.818, false);
    }

    return;
}

void normalRecord(float avg) {
    FILE * fptr = fopen("../assets/fitnesses.txt", "a");
    fprintf(fptr, "%f\n", avg);
    fclose(fptr);
    return;
}

void initialPopTest(ord * dist) {
    FILE * fptr = fopen("../assets/fitnesses.txt", "a");
    for (int i = 0; i < genSize; i++) {
        fprintf(fptr, "%f\n", dist[i].unord);
    }
    fclose(fptr);
    quit_(0000, __LINE__, __FILE__);
    return;
}

void parameterTest(ord * dist) {
    /* Record fitness' of all creatures */
    FILE * fptr = fopen("../assets/fitnesses.txt", "a");

    if (gen == (int) (ceil(log(genSize)/log(2)) + 1)) {
        fprintf(fptr, "Generation %d\n", gen);
        for (int i = 0; i < genSize; i++) {
            //fprintf(fptr, "%f\n", dist[i].order);

            fprintf(fptr, "%f\n", dist[i].unord);
            if (gen == 0) { // Only print first creature in gen 0 (cause all the same)
                break;
            }
        }
    }
    fclose(fptr);
    if (gen == (int) (ceil(log(genSize)/log(2)) + 1)) {
        quit_(0000, __LINE__, __FILE__);
    }
    return;
}

void impactTest(ord * dist) {
    /* Record fitness' of all creatures */
    FILE * fptr = fopen("../assets/fitnesses.txt", "a");
    fprintf(fptr, "New Generation\n");
    for (int i = 0; i < genSize; i++) {
        fprintf(fptr, "%f\n", dist[i].order);
        if (gen == 0) {
            break;
        }
    }
    fclose(fptr);
    if (gen == 1) {
        quit_(0000, __LINE__, __FILE__);
    }
}

#include <math.h>
int * orderDistances(creature * group) {
    ord * dist    = (ord*) malloc(sizeof(ord) * genSize);
    int * topHalf = (int*) malloc(sizeof(int) * genSize);

    for (int i = 0; i < genSize; i++) {
        topHalf[i] = rand() % genSize; // BAND-AID for accidental non-initialization
    }

    if (dist    == NULL) quit(MALLOC_ERROR);
    if (topHalf == NULL) quit(MALLOC_ERROR);

/* Use Dummy so as to not overwrite */
    for (int i = 0; i < genSize; i++) {
        dist[i].unord = group[i].distance;
    }
    if (dataCollection == 1) {
        parameterTest(dist);
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
    for (int i = 0; i < genSize; i++)
        dist[i].unord = group[i].distance;



/* Record id.copie in order of highest distance */
    bool * beenRecorded = (bool*) malloc(sizeof(bool) * genSize);
    if (beenRecorded == NULL) quit(MALLOC_ERROR);

    for (int i = 0; i < genSize; i++) {
        beenRecorded[i] = false;
    }

/* Record Top Half Distances into Array */
    double avg = 0.0;
    for (int i = 0; i < genSize; i++) {
        for (int j = 0; j < genSize; j++) {
            // These values are duplicates in different orders, they should be ==, but 1e-100 for some tolerance. (does precision even work?)
            if ((fabs(dist[i].order - dist[j].unord) <= 1e-10)) { // 1-e4 caused error
                /* Determind if already recorded */
                if (beenRecorded[j]) continue;
                else beenRecorded[j] = true;

                avg += dist[i].order;

                if (i < genSize / 2)
                    topHalf[i] = j;

                /* Break */
                j = genSize;
            }
        }
    }
    for (int i = 0; i < genSize / 2; i++) {
        if (topHalf[i] > genSize) {
            quit(topHalf[i]);
        }
        if (topHalf[i] < 0) {
            quit(topHalf[i]);
        }
    }
    avg /= genSize;

    genDisGraph(avg, true); // Gen Graph

/* Modify Array to let Slower Creatures Through*/
//    for (int i = 0; i < genSize; i++) {
//        if (chance(20)) {
//            topHalf[i] = rand() % genSize;
//        }
//    }

/* Record Averages */
//    FILE * fptr = fopen("test.txt", "a");
//    fprintf(fptr, "%.2f\n", avg);
//    fclose(fptr);

    if (dataCollection == 2) {
        impactTest(dist);
    }

    if (dataCollection == 3) {
        initialPopTest(dist);
    }

    if (dataCollection == 4) {
        normalRecord(avg);
    }

    if (dist != NULL) {
        free(dist);
    }
    if (beenRecorded != NULL) {
        free(beenRecorded);
    }
    return topHalf;
}

bool isInArray(int * arr, int len, int toCheck) {
    for (int i = 0; i < len; i++) {
        if (arr[i] == toCheck) {
            return true;
        }
    }
    return false;
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

void duplicate(int * toDup, creature * population) {
    /* Get the Organized Array */
    for (int i = 0; i < genSize / 2; i++) {
        for (int j = i + 1; j < genSize / 2; j++) {
            if (toDup[i] > toDup[j]) {
                int a    = toDup[i];
                toDup[i] = toDup[j];
                toDup[j] = a;
            }
        }
    }
    /* Clear Genomes of Creatures that wont be Duplicated */
    for (int i = 0; i < genSize; i++) {
        if (!isInArray(toDup, genSize / 2, i)) {
            population[i].genome = clearGenome(population[i].genome);
        }
    }

    /* Copy Genomes From Copy list into cleared genomes */
    int dupPos = 0;
    for (int popPos = 0; popPos < genSize; popPos++) {
        if (!isInArray(toDup, genSize / 2, popPos)) {
            /* Duplicate the Genome */
            population[popPos].genome = (gene*) malloc(sizeof(gene));
            if (population[popPos].genome == NULL) quit(MALLOC_ERROR);
            gene * popCurr = population[popPos].genome;
            gene * dupCurr = population[toDup[dupPos]].genome;
            if (dupCurr == NULL) quit(MALLOC_ERROR);

            /* Copy the Genome */
            while (dupCurr) {
                copyGene(popCurr, dupCurr);

                popCurr = popCurr->next;
                dupCurr = dupCurr->next;
            }
            dupPos++;
        }
    }
    return;
}










