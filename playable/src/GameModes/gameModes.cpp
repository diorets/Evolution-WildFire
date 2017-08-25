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
        printf("Done Initializing System %d\n", system);
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
//            sleep_ms(30);
//            drawSystem(system, pop, genSize, gen, id, simTime, maxTime);
//            drawSystem(system, pop, genSize, gen, id, simTime, maxTime);
//            drawSystem(system, pop, genSize, gen, id, simTime, maxTime);
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





#include "GameModes/Simulate/General/genetics.h"
#include "GameModes/Simulate/StickBall/stickBallDrawing.h"
//gene * parseGenome(gene * heads) {
//    /* Get Genome String */
//    std::string s = \
//"175:F:461.809197><i|7,6,3,0,17,<n|-18.043422,11.998212,6.169694,1.243904,0.561245,<n|-0.830763,8.910807,19.193227,3.405103,0.142579,<n|-13.082766,1.142906,18.102023,2.034333,0.661394,<n|-6.837053,-16.020219,5.419527,1.223151,0.648991,<n|14.935844,14.958995,18.563778,4.551988,0.458849,<b|0,2,<b|3,0,<m|2,1,<b|4,0,<b|3,2,<m|4,2,<b|3,4,<n|-19.848927,-5.083370,8.135112,4.541490,0.525697,<m|2,5,<n|-1.000000,3.000000,14.293590,4.740715,0.763546,<b|2,6,<";
//    std::fstream myfile("../assets/genomes.txt");
//    //for (int i = 0; (i != trials) && getline(myfile, s); i++);
//    myfile.close();
//
//    //std::cout << trials << std::endl;
//    /* Fill Genome */
//    std::string geneDelim = "<";
//    std::string dataDelim = ",";
//    std::string geneStr;
//    size_t genePos = 0;
//    while ((genePos = s.find(geneDelim)) != std::string::npos) {
//        geneStr = s.substr(0, genePos);
//        s.erase(0, genePos + geneDelim.length());
//        char geneType = geneStr[0];
//        if (isdigit(geneType)) { // ignores first entry
//            continue;
//        }
//        geneStr = geneStr.substr(2, std::string::npos);
//        size_t dataPos = 0;
//        std::string dataStr;
//
//        generic datums[10];
//        int i = 0;
//        while ((dataPos = geneStr.find(dataDelim)) != std::string::npos) {
//            dataStr = geneStr.substr(0, dataPos);
//            if (geneType == 'n') {
//                datums[i].f = atof(dataStr.c_str());
//            } else {
//                datums[i].i = atoi(dataStr.c_str());
//            }
//            geneStr.erase(0, dataPos + dataDelim.length());
//            i++;
//        }
//
//        switch (geneType) {
//            case 'i':
//                head = infoGene(datums[0].i, datums[1].i, datums[2].i, datums[3].i);
//                break;
//            case 'n':
//                head = addToBack(head, nodeGene(datums[0].f, datums[1].f, datums[2].f, datums[3].f, datums[4].f));
//                break;
//            case 'b':
//                head = addToBack(head, boneGene(datums[0].i, datums[1].i));
//                break;
//            case 'm':
//                head = addToBack(head, muscleGene(datums[0].i, datums[1].i));
//                break;
//            default:
//                quit(GENOME_ERROR);
//        }
//    }
//    head = addToBack(head, NULL); // Can likely remove
//    return head;
//}
//
//creature * loadGeomes() {
//    FILE* genomes = fopen("../assets/genomes.txt", "r");
//
//    return pop;
//}


void SIMULATION_MODE() {
    static const int genSize = 3000;
    static const int maxTime = 1500;
    static creature * population = NULL;
    static int id = 0;
    static int gen = 0;
    static int simTime = 0;

    static const unsigned int creatureSizes[] = {sizeof(stickball), sizeof(turbine), sizeof(cannon), sizeof(cube)};
    static const int system = 0;

    population = initializePop(population, creatureSizes, system, genSize);

    graphics(system, population, genSize, gen, id, simTime, maxTime);
    if (playerSpeed < 0.0001) {
            sleep_ms(500);
            return;
    }
    if (playBackSpeed <= 0) return;

    bool error = updateSystem(system, &population[id], &simTime);
    creatureIteration(error, &population[id], system, &simTime, &id, maxTime);

    static int record = -1;
    for (button * b = buttons; b != NULL; b = b->next) {
        if ((record == -1) && (b->countDown == 29) && !strcmp("Save Genome", b->label)) {
            printf("Trying to save ID: %d\n", id);
            record = id;
        }
    }
    if ((record != -1) && (record + 1 == id)) {
        puts("Saved");
        saveGenome(population, gen, record);
        record = -1;
    }

    applyGeneticOperators(system, population, id, genSize);
    generationIteration(genSize, &id, &gen);
    return;
}





// Menu mode
static void initMenu() {
    //playSong("../assets/AlpineRun.mp3", "menuSong");
    return;
}
#include "Math/myMath.h"
#include <math.h>
#include "Visual/basic.h"     // 2D,3D, Shapes & Text
void cinematicCamera(int counter) {
    double SPEED = 3.0;
    double speed = SPEED * 1 / 2000.0;
    static double arr[3] = {0.5, 1.0, 1.5};

    double static theta = 0.0; // 0 - 2pi
    double static phi = 0.0;
    double static r = 80;

    double static thetaDot = 1.0 / 1000.0;
    double static phiDot = 1.0 / 1000.0;
    double static rDot = 1.0 / 1000.0;

    /* Change Speeds Randomly */
    if (chance(0.1)) {
        int shift = rand() % 3;
        arr[0] = arr[(shift + 0) % 3];
        arr[1] = arr[(shift + 1) % 3];
        arr[2] = arr[(shift + 2) % 3];
    }

    theta += thetaDot * arr[0];
    phi += phiDot * arr[1];
    r += rDot * arr[2];

    if (phi > 3.14159/2.0) {
        phiDot = -fabs(phiDot);
        phi = 3.14159/2.0 - 0.001;
    } else if (phi < 0.1745) {
        phiDot = fabs(phiDot);
        phi = 0.1745 + 0.0001;
    }
    if (theta > 2*3.14159) {
        thetaDot = -fabs(thetaDot);
        theta = 2*3.14159 - 0.001;
    } else if (theta < 0) {
        thetaDot = fabs(thetaDot);
        theta = 0.001;
    }

    /* Move and Look at New Position */
    cameraPos.x = r*cos(theta)*sin(phi);
    cameraPos.y = r*sin(theta)*sin(phi);
    cameraPos.z = r*cos(phi);

    cameraDir.x = -cameraPos.x + 5 * sinf(counter * speed);
    cameraDir.y = -cameraPos.y + 5 * sinf(counter * speed);
    cameraDir.z = -cameraPos.z + 5 * sinf(counter * speed) + 3;
    return;
}

static void startText() {
    glColor3f(RED);
    drawHeader("EVOLUTION WILDFIRE!", wx * 0.47, wy * 0.12);

    glColor3f(ORANGE);
    drawText("What can you evolve, given the hand of God?", wx * 0.6, wy * 0.17, true, false);

    glColor3f(BLACK);
//    drawText("*Creatures are 2D only for display (faster evolution)", wx * 0.87, wy * 0.05, true);


    /* Draw Menu */
//    posi o = vec(wx * 0.15, wy * 0.25, 0.0);
//    drawMenu("Enter Simulation Mode\n"
//                    "Quit\n",
//
//                    "( B )\n"+
//                    "(ESC)\n", o.x, o.y);
//    glColor3f(BLACK);
    drawHeader("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Palfore Projects 2017 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", wx * 0.5, wy * 0.95);
    return;
}

#include "Functional/buttons.h"

static void assignCords(button * b) {
    switch (b->group) {
        case 0:
            {
                double width = 300;
                double height = 50;
                b->x = wx * 0.10;
                b->y = b->id * height +  wy * 0.25;
                b->w = wx * 0.10 + width;
                b->h = b->id * height + wy * 0.25 + height;
            }
            break;
        default: return;
    }
    return;
}
#include "GameModes/Simulate/StickBall/stickBallDrawing.h"
void switchToSimMode() {
    newGameMode(simMode);
    return;
}

void startUpMode() {
    static const unsigned int creatureSizes[] = {sizeof(stickball), sizeof(turbine), sizeof(cannon)};
    static const int system = stickballE;
    static int i = 0;
    static int id = 0;
    const int genSize = 1000;
    if (i == 0) initMenu();
    cinematicCamera(++i);

    static creature * population = NULL;
    population = initializePop(population, creatureSizes, 0, genSize);


    /* Simulate Creatures in Background */
    glutDrawing();
    drawGround(800, CAGESIZE, CAGESIZE);
    drawStickBall(population[id]);
    sleep_ms(5);
    int pseudoi = i;
    creatureIteration(updateSystem(system, &population[id], &pseudoi), &population[id], system, &pseudoi, &id, 1000);
    if (i % 1000 == 0) id = (id + 1) % genSize; // Cycle through creatures

    /* Display Information and Menus Text */
    enable2D();



    startText();
    if (buttons == NULL) {
        int j = 0;
        buttons = createButton("Enter Simulation Mode (ENTER)", switchToSimMode, assignCords, j++, 0, false , false);
        buttons = createButton("Exit (ESC)", normalExit, assignCords, j++, 0, false , false);
    }

    /* Highlighting */
    for (button * b = buttons; b != NULL; b = b->next) {
        b->highlighted = hoveringOver(b);
    }

    drawButtons();



    reenable3D();
    glutSwapBuffers();
    return;
}










