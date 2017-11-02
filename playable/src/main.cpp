/*****************************************************************
Evolution WildFire, A Unique 3D Evolution Simulator.

Nawar Ismail
July 2016
******************************************************************/
#include <stdlib.h> // standard definitions
#include <stdio.h>  // standard I/O
#include "Glut/initialize.h"       // Initialize
#include "Glut/myGlut.h"           // GlutMainLoop
#include "preproccessor.h"         // Defintions
#include "global.h"                // Globals
#include "Math/myMath.h"           // vec, zero

/** Global State Variables **/
// Settings
int dataCollection = 5; // none, parameterTest, impactTest, initial population, multiWorld
int data[] = {5,3,3,0, 10, 500}; // GENSIZE MYST Be 500
// initPopNodes, initPopMuscles, initPopBones, initPopNeurons, srand, genSize

bool person     = false;
bool lights     = true;
bool display    = true;
bool fullscreen = true;
int  gameMode   = startMode; //startMode
//int  genSize    = data[5];

// window size (obtained updated during runtime)
int  wx         = 0;
int  wy         = 0;

// Speeds
int    playBackSpeed = 1; // 1
double playerSpeed   = 1.2;// / playBackSpeed; // Should scale with overhead
double cameraSpeed   = 0.04;// / playBackSpeed;
int processSlowDown = 0;

// Camera position
posi cameraPos = vec(0.0, -40.0, 1.8);
posi cameraMov = vec(0.0,   0.0, 1.8);
posi cameraDir = vec(0.0,   1.0, 0.0);
posi cameraAng = zero();
posi cameraDel = zero();

// Mouse
posi mousePos   = zero();
posi xDragStart = zero();
bool isDragging = false;

// Key Hold
bool* keyStates = new bool[256];

// Variables
double * environment = NULL;
generic  * globalData = NULL;
button * buttons = NULL;
GLuint * textures;

double coeffs[10];

void graphTest();

#define ALPHAS 10
typedef struct selFunct_struct {
    double coeff[ALPHAS];
    double fitness;
} selFunct;

int compUp (const void * elem1, const void * elem2) { // last is highest
    double f = *((double*)elem1);
    double s = *((double*)elem2);

    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}
int compDown (const void * elem1, const void * elem2) {
    double f = *((double*)elem1);
    double s = *((double*)elem2);

    if (f < s) return  1;
    if (f > s) return -1;
    return 0;
}

#include "../include/GameModes/Simulate/General/genetics.h"
void select(double * pop, int genSize) { // get a creature according to distribution
    double * tmp = (double*) malloc(sizeof(double) * genSize);
    for (int i = 0; i < genSize; i++) {
        int genomeID = (int) selectionDistribution(genSize, false); // This should change based on evolve or not
        genomeID = genomeID >= genSize ? genSize - 1: genomeID;
        genomeID = genomeID  < 0       ?           0: genomeID;
        tmp[i] = pop[genomeID];
    }
    for (int i = 0; i < genSize; i++) {
        pop[i] = tmp[i];
    }

    free(tmp);
}



void selFunctSelect(selFunct * pop, int genSize) { // get a creature according to distribution
    selFunct * tmp = (selFunct*) malloc(sizeof(selFunct) * genSize);

    for (int i = 0; i < genSize; i++) {
        int genomeID = (int) selectionDistribution(genSize, false); // use a known selection function
        genomeID = genomeID >= genSize ? genSize - 1: genomeID;
        genomeID = genomeID  < 0       ?           0: genomeID;
        tmp[i].fitness = pop[genomeID].fitness;
        for (int j = 0; j < ALPHAS; j++) {
            tmp[i].coeff[j]= pop[genomeID].coeff[j];
        }
    }
    for (int i = 0; i < genSize; i++) {
        pop[i].fitness = tmp[i].fitness;
        for (int j = 0; j < ALPHAS; j++) {
            pop[i].coeff[j]= tmp[i].coeff[j];
        }
    }

    free(tmp);
}


const char* getfield(char* line, int num) {
    const char* tok;
    for (tok = strtok(line, " ");
            tok && *tok;
            tok = strtok(NULL, " \n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}



#define IMPACT_SIZE 150000
double getImpact(double fitness) {
    static char ** impacts = NULL;
    static double * levels = NULL;
    static int numLevels = 0;


    /* Get levels from file */
    if (levels == NULL) {
        FILE * f = fopen("../assets/mutationDatabase.txt", "r");
        char * str = (char*) malloc(sizeof(char) * IMPACT_SIZE);
        if (str == NULL)  {puts("Cannot allocate"); exit(-1);}

        while (fgets(str, IMPACT_SIZE, f) != NULL) {
            impacts = (char**) realloc(impacts, sizeof(char*) * (numLevels+1));
            impacts[numLevels] = (char*) malloc(sizeof(char) * IMPACT_SIZE);
            strcpy(impacts[numLevels], str);

            char level[50];
            char dummy[100];
            sscanf(str, "%s | %s", dummy, level);

            levels = (double*) realloc(levels, sizeof(double) * (numLevels+1));
            levels[numLevels] = atof(level);
            numLevels++;
        }
        free(str);
        fclose(f);
    }

    /* Determine line number that impact should be pulled from */
    int line = 0;
    for (int i = 0; i < numLevels; i++) {
        if (fitness > levels[numLevels - 1]) {
            line = numLevels - 1;
            break;
        }
        if ((fitness > levels[i]) && (fitness < levels[i+1])) {
            line = i;
            break;
        }
    }

    /* Get impact from line "line" in file */
    int numImpacts = 0;
    int len = strlen(impacts[line]);
    for (int i = 0; i < len; i++) {
        if (impacts[line][i] == ' ') numImpacts++;
    }

    char* tmp = strdup(impacts[line]);

    double index = randi2(5, numImpacts - 5 - 1); //random number from 5 to numImpacts
    double impact = atof(getfield(tmp, index));

    free(tmp);
    double correction = 2 * 0.25 * (fitness - levels[line]);
    return impact - correction;
}

void mutate(double * pop, int genSize) {
    for (int i = 0; i < genSize; i++) {
        double impact = getImpact(pop[i]);
        pop[i] += impact;
    }
}


double * initPop(int genSize) {
    double * pop = (double*) malloc(sizeof(double) * genSize);
    FILE* f = fopen("../assets/initialGeneration.txt", "r");
    char str[4000];
    while(fgets(str, 4000, f));

    for (int i = 0; i < genSize; i++) {
        char numberStr[100];
        sscanf(str, "%s,", numberStr);
        pop[i] = atof(numberStr);
    }
    fclose(f);
    return pop;
}

double getAverage(double * pop, int genSize) {
    double avg = 0.0;
    for (int i = 0; i < genSize; i++) {
        avg += pop[i];
    }
    return avg / (double) genSize;
}


double getSelFunctAverage(selFunct * pop, int genSize) {
 double avg = 0.0;
    for (int i = 0; i < genSize; i++) {
        avg += pop[i].fitness;
    }
    return avg / (double) genSize;
}

int selFunctCompare(const void * elem1, const void * elem2) { // last is highest
   selFunct * f1 = (selFunct*) elem1;
   selFunct * f2 = (selFunct*) elem2;

    if (f1->fitness < f2->fitness) return  1;
    if (f1->fitness > f2->fitness) return -1;
    return 0;
}


void mutateSelFunct(selFunct * pop, int genSize) {
    for (int i = 0; i < genSize; i++) {
        for (int j = 0; j < ALPHAS; j++) {
            if (chance(10)) {
                pop[i].coeff[j] = randf(1);
            }
            if (chance(20)) {
                // smaller change
            }
        }
    }

}

void evaluateFitness(selFunct * pop, int selFunctGenSize) {
    int genSize = 300;
    int maxGens = 150;
    for (int i = 0; i < selFunctGenSize; i++) {
        /* Use creatures selection function */
        for (int j = 0; j < 10; j++) {
            coeffs[j] = pop[i].coeff[j];
        }

        /* Simulate Generation */
        double * population = initPop(genSize);
        for (int gen = 0; gen < maxGens; gen++) {
            qsort (population, genSize, sizeof(population[0]), compDown);
            select(population, genSize);
            mutate(population, genSize);
        }

        /* Find the average the selection function produced */
        double average = getAverage(population, genSize);
        pop[i].fitness = average;
    }
}




#include <math.h>
#include <time.h>
int main(int argc, char **argv) {
//    graphTest();
//    exit(-1);
    fclose(fopen("../assets/fitnesses.txt", "w")); // Clear File

    environment = (double*) malloc(sizeof(double) * 2);
    if (environment == NULL) exit(34523);
    environment[0] = 5;
    environment[1] = 0;

    bool model = false;
    bool evolve = false; // Dont forget to change "select" function in main**** & genetics.cpp -> environemtn[0] to 27
    srand(time(NULL));
    if (model) {
        while (true) { // number of instances
            environment[0] = 5;
            for (int p = 0; p < 20; p++) { // range of selection function parameter
                int genSize = 300; // 300
                int maxGens = 300; // 300
                double * population = initPop(genSize);
                for (int gen = 0; gen < maxGens; gen++) {
                    qsort (population, genSize, sizeof(population[0]), compDown);
                    select(population, genSize);
                    mutate(population, genSize);

//                    printf("%f\n", getAverage(population, genSize)); // fitness over generation
                }
                double average = getAverage(population, genSize);
                printf("%f %f\n", environment[0], average);


                FILE* f = fopen("../assets/model.txt", "a");
                fprintf(f, "%f %f\n", environment[0], average);
                fclose(f);

                free(population);
                environment[0] += 5;
            }
        }
    } else if (evolve) {
        int numMembers = 100;
        selFunct * members = (selFunct*) malloc(sizeof(selFunct) * numMembers);

        /* Initialize */
        for (int i = 0; i < numMembers; i++) {
            members[i].fitness = 0;
            for (int j = 0; j < ALPHAS; j++) {
                members[i].coeff[j] = randf(1);
            }
        }

        /* Evolve */
        for (int gen = 0; gen < 1000; gen++) {
            evaluateFitness(members, numMembers);
            qsort(members, numMembers, sizeof(selFunct), selFunctCompare);
            selFunctSelect(members, numMembers);
            mutateSelFunct(members, numMembers);

            printf("Population %d Average is %f\n", gen, getSelFunctAverage(members, numMembers));
            printf("(best ) %f had alphas: ", members[0].fitness);
            for (int i = 0; i < ALPHAS; i++) {
                printf("%f ", members[0].coeff[i]);
            }
            printf("\n(worse) %f had alphas: ", members[numMembers - 1].fitness);
            for (int i = 0; i < ALPHAS; i++) {
                printf("%f ", members[numMembers - 1].coeff[i]);
            }
            printf("\n");
        }

    } else {
        initiateSimulator(&argc, argv, "Evolution WildFire v6.0");
        glutMainLoop();
    }
	return 0;
}




#include "Math/distribution.h"
#define PI 3.1415926
double testFunction(double x) {
    #define n 5
    double val = 0.0;
    double a[n] = {1.2, 2.3, 5.2, 0.5, 1.5};
    for (int i = 0 ; i < n; i++) {
        val += (((i / (double) n) < x) && (x < (i+1) / (double) n)) ? a[i] : 0;
    }

    return val;
    #undef n
}
void graphTest() {
    srand(time(NULL));
    for (int i = 0; i < 10000; i++) {
        printf("%d\n", getDistribution(testFunction, 100, 0));
    }
    return;
}
























































