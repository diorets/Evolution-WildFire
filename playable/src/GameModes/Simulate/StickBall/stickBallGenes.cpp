#include "GameModes/Simulate/StickBall/stickBallGenes.h"

#include <math.h>   // Fabs
#include <stdio.h>  // printf, file Functions
#include <stdlib.h> // Malloc
#include "Math/myMath.h"       // Random, Vec
#include "global.h"            // Globals
#include "ErrorHandle/error.h" // Quit

#include "Functional/list.h"   // Various Linked List Functions
#include "GameModes/Simulate/StickBall/stickBallMutations.h"

gene* addMuscles(gene * head);

gene* createStickBallGenome(gene * head) {
    if (head != NULL) quit(GENOME_ERROR);


    if (0) {
        int n = 30;
        int m = 0;
        int b = 0;
        if (m + b > comb(n)) quit(GENOME_ERROR);

        /* Creating Genome */
        head = infoGene(n, b, m, 0);
        if (head == NULL) quit(GENOME_ERROR);

        int * sizes = head->iData;

        head = addToBack(head, nodeGene(vec(0, 0, 30)));
        for (int i = 1; i < sizes[nod]; i++) {
            head = addToBack(head, nodeGene(head));
        }
        head = addMuscles(head);
    } else {
        int n = 6;
        int m = 5;
        int b = 5;
        if (m + b > comb(n)) quit(GENOME_ERROR);

        /* Creating Genome */
        head = infoGene(n, b, m, 0);
        if (head == NULL) quit(GENOME_ERROR);

        int * sizes = head->iData;

        for (int i = 0; i < sizes[nod]; i++) {
            head = addToBack(head, nodeGene(head));
        }
        for (int i = 0; i < sizes[bon]; i++) {
            head = addToBack(head, boneGene(head));
        }
        for (int i = 0; i < sizes[mus]; i++) {
            head = addToBack(head, muscleGene(head));
        }
    }

    head = addToBack(head, NULL); // Can likely remove

    removeStrandedNodes(head);
    return head;
}

void createStickBallCreature(creature * newBorn) {
    /* Reading genome */
    int numNodes   = 0;
    int numBones   = 0;
    int numMuscles = 0;
//    int numAxons   = 0;
    gene * currentGenome = newBorn->genome;
    stickball * components = ((stickball*) newBorn->components);
//    puts("tes");
    FOR_ALL(currentGenome, 'n') {
        components->nodes[numNodes].loc.x = current->fData[xposi];
        components->nodes[numNodes].loc.y = current->fData[yposi];
        components->nodes[numNodes].loc.z = current->fData[zposi];
//        printf("%d, %f, %f, %f\n", numNodes, components->nodes[numNodes].loc.x, components->nodes[numNodes].loc.y, components->nodes[numNodes].loc.z);
        components->nodes[numNodes].vel.x = 0.0;
        components->nodes[numNodes].vel.y = 0.0;
        components->nodes[numNodes].vel.z = 0.0;

        components->nodes[numNodes].force.x = 0.0;
        components->nodes[numNodes].force.y = 0.0;
        components->nodes[numNodes].force.z = 0.0;

        components->nodes[numNodes].mass     = current->fData[mass];
        components->nodes[numNodes].friction = current->fData[fric];

        numNodes++;
    }

    FOR_ALL(currentGenome, 'm') {
        int a = components->muscles[numMuscles].a = current->iData[0];
        int b = components->muscles[numMuscles].b = current->iData[1];
        components->muscles[numMuscles].origLength = euc(components->nodes[a].loc, components->nodes[b].loc);
        numMuscles++;
    }

    FOR_ALL(currentGenome, 'b') {
        int a = components->bones[numBones].a = current->iData[0];
        int b = components->bones[numBones].b = current->iData[1];
        components->bones[numBones].length = euc(components->nodes[a].loc, components->nodes[b].loc);
        numBones++;
    }

    components->origin = getCom(*newBorn);
    newBorn->fitness= 0.0;
    // Add a check here for valid creature
    return;
}
















conn goodConnection(gene * head) {
    // This doesnt check that there IS a valid connection (?)
    int numNodes = head->iData[nod];
    while (true) {
        bool validConn = true;

         /* Try a new connection */
        int a = rand() % numNodes;
        int b = rand() % numNodes;

        if (a == b) {
            validConn = false;
            continue;
        }

        /* Check if this connection exist */
        for (gene * current = head; current != NULL; current = current->next) {
            if ((current->start == 'm') || (current->start == 'b'))  {
                if ((current->iData[0] == a && current->iData[1] == b) ||
                    (current->iData[1] == a && current->iData[0] == b) ){
                    validConn = false;
                    continue;
                }
            }
        }

        /* Valid Connection Found */
        if (validConn) {
            if (a == b) {
                quit(GENOME_ERROR);
            }
            conn validCon;
            validCon.a = a;
            validCon.b = b;
            return validCon;
        }
    }
}


bool nodeAt(gene * head, posi testLoc) {
    FOR_ALL(head, 'n') {
        posi nodeLoc = vec(current->fData[xposi], current->fData[yposi], current->fData[zposi]);
//        printf("(%f, %f, %f), (%f, %f, %f)\n", nodeLoc.x, nodeLoc.y, nodeLoc.z, testLoc.x, testLoc.y, testLoc.z);
        if (equals(testLoc, nodeLoc, 0.5)) {
            return true;
        }
    }
    return false;
}

posi dirPerturb(posi a, double h, int i) {
    switch(i) {
        case 0: return vec(a.x+h, a.y  , a.z);
        case 1: return vec(a.x+h, a.y  , a.z+h);
        case 2: return vec(a.x+h, a.y  , a.z-h);

        case 3: return vec(a.x+h, a.y+h, a.z);
//        case 4: return vec(a.x+h, a.y+h, a.z+h);
//        case 5: return vec(a.x+h, a.y+h, a.z-h);

        case 4: return vec(a.x+h, a.y-h, a.z);
//        case 7: return vec(a.x+h, a.y-h, a.z+h);
//        case 8: return vec(a.x+h, a.y-h, a.z-h);

        case 5: return vec(a.x-h, a.y  , a.z);
        case 6: return vec(a.x-h, a.y  , a.z+h);
        case 7: return vec(a.x-h, a.y  , a.z-h);

        case 8: return vec(a.x-h, a.y+h, a.z);
//        case 13: return vec(a.x-h, a.y+h, a.z+h);
//        case 14: return vec(a.x-h, a.y+h, a.z-h);

        case 9: return vec(a.x-h, a.y-h, a.z);
//        case 16: return vec(a.x-h, a.y-h, a.z+h);
//        case 17: return vec(a.x-h, a.y-h, a.z-h);

        case 10: return vec(a.x, a.y, a.z+h);
        case 11: return vec(a.x, a.y, a.z-h);

        case 12: return vec(a.x, a.y+h, a.z);
        case 13: return vec(a.x, a.y+h, a.z+h);
        case 14: return vec(a.x, a.y+h, a.z-h);

        case 15: return vec(a.x, a.y-h, a.z);
        case 16: return vec(a.x, a.y-h, a.z+h);
        case 17: return vec(a.x, a.y-h, a.z-h);
        default: exit(-1);
    }
}
#define numDirs 18
int * getNeighborhs(gene * head, posi attachNodeLoc, double nodeSeperation) {
    int * neighborIndexs = (int*) malloc(sizeof(int)); // The first index is reserved for the length of the array
    int numNeighbors = 1;
    /* Check each orthogonal direction surrounding attachNodeLoc */
    for (int i = 0; i < numDirs; i++) {
        posi neighborLoc = dirPerturb(attachNodeLoc, nodeSeperation, i);
        if (nodeAt(head, neighborLoc)) { // If there is a node in an adjacent direction
            /* Find the Index */
            int nodeNum = 0;
            FOR_ALL(head, 'n') {
                posi loc = vec(current->fData[xposi], current->fData[yposi], current->fData[zposi]);
                if (equals(loc, neighborLoc, 0.1)) {
                    neighborIndexs = (int*) realloc(neighborIndexs, sizeof(int) * (numNeighbors+1));
                    neighborIndexs[numNeighbors] = nodeNum;
                    numNeighbors++;
                }
                nodeNum++;
            }
        }
    }
    neighborIndexs[0] = numNeighbors;
    return neighborIndexs;
}

gene* addMuscles(gene * head) {
    int nodeNum = 0;
    double nodeSeperation = 40 * RADIUS;
    FOR_ALL(head, 'n') {
        posi loc = vec(current->fData[xposi], current->fData[yposi], current->fData[zposi]);
        int * neighbors = getNeighborhs(head, loc, nodeSeperation);
        int arrLen = neighbors[0];
        for (int i = 1; i < arrLen; i++) { // For each neighbor in neighbors
            if (nodeNum == neighbors[i]) { // cant be own neighbor
                printf("Something went wrong.\n");
                continue;
            }
            if (connectionExists(head, nodeNum, neighbors[i])) continue;
            if (chance(70)) {
                if (chance(50)) {
                    head = addToBack(head, muscleGene(nodeNum, neighbors[i]));
                    (head->iData[tot])++;
                    (head->iData[mus])++;
                } else {
                    head = addToBack(head, boneGene(nodeNum, neighbors[i]));
                    (head->iData[tot])++;
                    (head->iData[bon])++;
                }
            }
        }
        free(neighbors);
        nodeNum++;
    }
    return head;
}

posi getValidPosition12(gene * head) {
    int numNodes = 0;
    double nodeSeperation = 40 * RADIUS;
    /* Get Number of Nodes */
    FOR_ALL(head, 'n') numNodes++;

    for (int trial = 0; trial < 100; trial++) {
        /* Choose a Node to Attach to */
        int attachTo = numNodes == 1 ? 0 : rand() % numNodes;
        int countDown = attachTo;
        posi attachNodeLoc;
        FOR_ALL(head, 'n') {
            if (countDown-- == 0) {
                attachNodeLoc = vec(current->fData[xposi], current->fData[yposi], current->fData[zposi]);
            }
        }

        /* Choose One of the Orthogonal Directions to Attach to */
        // which directions are available?
        bool validDirs[numDirs];
        for (int i = 0; i < numDirs; i++) {
            if (nodeAt(head, dirPerturb(attachNodeLoc, nodeSeperation, i))) {
                validDirs[i] = false;
            } else {
                validDirs[i] = true;
            }
        }

        // How many directions
        int numValidDirs = 0;
        for (int i = 0; i < numDirs; i++) {
            if (validDirs[i]) {
                numValidDirs++;
            }
        }
        if (numValidDirs == 0) {
                printf("SSE12R\n");
            // try different node
        } else {
            // Now pick one
            int chosenDir = numValidDirs == 1 ? 0 : rand() % numValidDirs;
            int actualDir = -1;
            for (int i = 0; i < numDirs; i++) { // count down for every valid direction
                if (validDirs[i]) {
                    if (chosenDir-- == 0) { // after we passed choseDir valid values
                        actualDir = i;
                    }
                }
            }
            if (!validDirs[actualDir]) {
                exit(-1);
            }

            if (actualDir == -1) {
                printf("The node attempted to attach to has no valid locations.\n");
                continue;
            } else {

                posi newLoc = dirPerturb(attachNodeLoc, nodeSeperation, actualDir);
                if (nodeAt(head, newLoc)) {
                    printf("%d, %d\n", validDirs[actualDir], nodeAt(head, dirPerturb(attachNodeLoc, nodeSeperation, actualDir)));
                }
                if (equals(newLoc, attachNodeLoc, 0.5)) {
                    exit(-1);
                }
                /* Connect via bone or muscle */
                if (chance(50)) {
                    head = addToBack(head, muscleGene(numNodes, attachTo));
                    (head->iData[tot])++;
                    (head->iData[mus])++;
                } else {
                    head = addToBack(head, boneGene(numNodes, attachTo));
                    (head->iData[tot])++;
                    (head->iData[bon])++;
                }
                return newLoc;
            }
        }
    }
    puts("ERROR, cannot find valid node location.");
    return vec(0, 0, -10);
}
#undef numDirs
















// Returns values aleast min dist from all other nodes. Bound by CAGESIZE.
posi getValidPosition(gene * head) {
    while (true) {
        bool validPosition = true; // Not False

        /* Try a new location */
        double x = (rand() % (2 * CAGESIZE)) - CAGESIZE; //+-CageSize SWITCh TO DOUBLE
        double y = (rand() % (2 * CAGESIZE)) - CAGESIZE;
        double z = RAND_NODE_HEIGHT;                   //[0, cageSize]
//        y = 0.0;

        /* Check if it is in the same location as another */
        FOR_ALL(head, 'n') {
            if (fabs(x - current->fData[xposi]) < MIN_NODE_DISTANCE &&
                fabs(y - current->fData[yposi]) < MIN_NODE_DISTANCE &&
                fabs(z - current->fData[zposi]) < MIN_NODE_DISTANCE ){
                validPosition = false;
            }
        }

        /* If valid, return the position */
        if (validPosition) {
            posi validPos;
            validPos.x = x;
            validPos.y = y;
            validPos.z = z;
            return validPos;
        }
    }
}

// Returns similar values aleast min dist from all other nodes. Bound by CAGESIZE.
posi getValidShift(gene * head, const double ix, const double iy, const double iz, double amount) {
    int numAttempts = 0;
    while (true) {
        numAttempts++;
        if (numAttempts > 1000) { // No Valid Location
            return vec(ix, iy, iz);
        }

        bool validPosition = true; // Not False

        /* Try a new location */
        double x = ix + pmRandf(amount); //+-CageSize
        double y = iy + pmRandf(amount);
        double z = iz + pmRandf(amount); //[0, cageSize]
//        y = 0.0;

       //printf("{");
        /* Check if it is in the same location as another */
        int numOverlaps = 0;
        FOR_ALL(head, 'n') {
            if (fabs(x - current->fData[xposi]) < MIN_NODE_DISTANCE &&
                fabs(y - current->fData[yposi]) < MIN_NODE_DISTANCE &&
                fabs(z - current->fData[zposi]) < MIN_NODE_DISTANCE ){
                numOverlaps++;
            }
            if (fabs(x) > CAGESIZE ||
                fabs(y) > CAGESIZE ||
                     z  > CAGESIZE ||
                     z  < 0.1     ){
                validPosition = false;
                //printf("0");
            }
        }

        if (numOverlaps > 1) {
            validPosition = false; // The creature youre shifting is exception
            //printf("%d | %d", head->iData[nod], numOverlaps);
        }
        //printf("}");
        /* If valid, return the position */
        if (validPosition) {
            posi validPos;
            validPos.x = x;
            validPos.y = y;
            validPos.z = z;
            return validPos;
        }
    }
}

bool typeConnectionExists(gene * head, char type, int a, int b) {
    FOR_ALL(head, type) {
        if (current->iData[0] == a && current->iData[1] == b) return true;
        if (current->iData[1] == a && current->iData[0] == b) return true;
    }
    return false;
}

bool connectionExists(gene * head, int a, int b) {
    if (typeConnectionExists(head, 'b', a, b)) return true;
    if (typeConnectionExists(head, 'm', a, b)) return true;
    return false;
}

/* Genes */
gene* infoGene(int a, int b, int c, int d) {
    gene * newInfo = (gene*) malloc(sizeof(gene));
    if (newInfo == NULL) quit(MALLOC_ERROR);;

    newInfo->start = 'i';

    newInfo->iData[nod] = a;
    newInfo->iData[bon] = b;
    newInfo->iData[mus] = c;
    newInfo->iData[neu] = d;

    newInfo->fData[0] = 2; //  mutation chances
    newInfo->fData[1] = 2;
    newInfo->fData[2] = 2;
    newInfo->fData[3] = 2;
    newInfo->fData[4] = 2;
    newInfo->fData[5] = 2;
    newInfo->fData[6] = 2;
    newInfo->fData[7] = 2;

    newInfo->iData[tot] = a + b + c + d + 1; // +1 for info gene
    newInfo->endof = '\0';
    newInfo->next = NULL;
    return newInfo;
}


// Given two nodes to connect to
gene* muscleGene(int a, int b) {
    gene * nodee = (gene*) malloc(sizeof(gene));
    if (nodee == NULL) quit(MALLOC_ERROR);;

    nodee->start = 'm';

    nodee->iData[0] = a;
    nodee->iData[1] = b;

    nodee->endof = '\0';
    nodee->next = NULL;
    return nodee;
}

// To 2 Valid nodes
gene* muscleGene(gene * genome) {
    gene * nodee = (gene*) malloc(sizeof(gene));
    if (nodee == NULL) quit(MALLOC_ERROR);;

    nodee->start = 'm';

    conn valid = goodConnection(genome);
    nodee->iData[0] = valid.a;
    nodee->iData[1] = valid.b;

    nodee->next = NULL;
    nodee->endof = '\0';
    return nodee;
}


gene* boneGene(int a, int b) {
    gene * nodee = (gene*) malloc(sizeof(gene));
    if (nodee == NULL) quit(MALLOC_ERROR);;

    nodee->start = 'b';

    nodee->iData[0] = a;
    nodee->iData[1] = b;

    nodee->endof = '\0';
    nodee->next = NULL;
    return nodee;
}

gene* boneGene(gene * genome) {
    gene * nodee = (gene*) malloc(sizeof(gene));
    if (nodee == NULL) quit(MALLOC_ERROR);;

    nodee->start = 'b';

    conn valid = goodConnection(genome);
    nodee->iData[0] = valid.a;
    nodee->iData[1] = valid.b;

    nodee->next = NULL;
    nodee->endof = '\0';
    return nodee;
}


gene * nodeGene(posi loc) {
    gene * nod = (gene*) malloc(sizeof(gene));
    if (nod == NULL) quit(MALLOC_ERROR);;
    nod->start = 'n';

    nod->fData[xposi] = loc.x;
    nod->fData[yposi] = loc.y;
    nod->fData[zposi] = loc.z;
    nod->fData[mass] =  randf(4) + 1;
    nod->fData[fric] =  randf(0.8) + 0.1;

    nod->endof = '\0';
    nod->next = NULL;
    return nod;
}

gene * nodeGene(gene * head) { // Does check valid location
   gene * nod = (gene*) malloc(sizeof(gene));
    if (nod == 0) quit(MALLOC_ERROR);;
    nod->start = 'n';

    posi valid = getValidPosition(head);
    nod->fData[xposi] = valid.x;
    nod->fData[yposi] = valid.y;
    nod->fData[zposi] = valid.z;

    nod->fData[mass] =  randf(4) + 1;
    nod->fData[fric] =  randf(0.8) + 0.1;


    nod->endof = '\0';
    nod->next = NULL;
    return nod;
}


// Connected to a given node and the last node
gene * addMuscle(int numNodes, int a) {
    gene * newMuscle = (gene*) malloc(sizeof(gene));
    if (newMuscle == NULL) quit(MALLOC_ERROR);;

    newMuscle->start = 'm';

    newMuscle->iData[0] = numNodes - 1;
    newMuscle->iData[1] = a;
    newMuscle->endof = '\0';
    newMuscle->next = NULL;
    return newMuscle;
}

gene * addBone(int numNodes, int a) {
    gene * newBone = (gene*) malloc(sizeof(gene));
    if (newBone == NULL) quit(MALLOC_ERROR);;

    newBone->start = 'b';

    newBone->iData[0] = numNodes - 1;
    newBone->iData[1] = a;
    newBone->endof = '\0';
    newBone->next = NULL;
    return newBone;
}


/* Printing */
void printGenome(int index, bool connection, bool location) {
    if (index || connection || location) return; //silence warning
//    col(red);
//    if (specimen[index].genome == NULL) {
//        printf("NULL\n");
//        return;
//    }
//    col(white);
//    printf("%d|", gen);
//    printf("id %2d :: ", index);
//    printf("T: %3d| ", specimen[index].genome->iData[tot]);
//    printf("N: %3d| ", specimen[index].genome->iData[nod]);
//    printf("M: %3d| ", specimen[index].genome->iData[mus]);
//    printf("B: %3d| ", specimen[index].genome->iData[bon]);
//
//
//    for (gene * current = specimen[index].genome; current != NULL; current = current->next) {
//        switch(current->start) {
//            case 'i':
//                col(yellow);
//                printf("%c", current->start);
//                break;
//            case 'n':
//                col(aqua);
//                printf("%c", current->start);
//                if (location) {
//                    printf("[");
//                    for (int i = 0; i < 3; i++) {
//                        col(i + 8);
//                        printf("%.1f", current->fData[i]);
//                        if (i != 2) printf(",");
//                    }
//                    printf("]");
//                }
//                break;
//            case 'm':
//                col(red);
//                printf("%c", current->start);
//                if (connection) printf("[%d,%d]", current->iData[0], current->iData[1]);
//                break;
//            case 'b':
//                col(white);
//                printf("%c", current->start);
//                if (connection) printf("[%d,%d]", current->iData[0], current->iData[1]);
//                break;
//            default:
//                col(green);
//                break;
//        }
//        printf(",");
//    }
//    printf("\n");
//    return;
}

void saveGenome() {
//    FILE * fptr = fopen("genomes.txt", "a");
//    fprintf(fptr, "Generation %d\n", gen);
//    for (int j = 0; j < genSize; j++) {
//        fprintf(fptr, "\t%d:", j);
//        for (gene * current = specimen[j].genome; current != NULL; current = current->next) {
//            fprintf(fptr, "|%c|", current->start);
//            switch (current->start) {
//                case 'i':
//                    for (int i = 1; i < 6; i++) {
//                        fprintf(fptr, "%d", current->iData[i]);
//                        if (i != 5) {
//                            fprintf(fptr, ",");
//                        }
//                    }
//                case 'n':
//                    for (int i = 0; i < 5; i++) {
//                        fprintf(fptr, "%f", current->fData[i]);
//                        if (i != 4) {
//                            fprintf(fptr, ",");
//                        }
//                    }
//                    break;
//                case 'm':
//                    fprintf(fptr, "(%d, %d)", current->iData[0], current->iData[1]);
//                    break;
//                case 'b':
//                    fprintf(fptr, "(%d, %d)", current->iData[0], current->iData[1]);
//                    break;
//                default: fprintf(fptr, "ERROR");
//            }
//        }
//        fprintf(fptr, "\n");
//    }
//    fclose(fptr);
    return;
}
























