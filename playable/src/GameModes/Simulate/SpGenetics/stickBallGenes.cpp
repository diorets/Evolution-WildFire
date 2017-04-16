#include "GameModes/Simulate/SpGenetics/stickBallGenes.h"

#include <math.h>   // Fabs
#include <stdio.h>  // printf, file Functions
#include <stdlib.h> // Malloc
#include "Math/myMath.h"       // Random, Vec
#include "global.h"            // Globals
#include "ErrorHandle/error.h" // Quit

#include "Functional/list.h"   // Various Linked List Functions
#include "GameModes/Simulate/SpGenetics/stickBallMutations.h"

gene* createStickBallGenome(gene * head) {
    if (head != NULL) quit(GENOME_ERROR);

    int n = 8;
    int m = 4;
    int b = 4;
    if (m + b > comb(n)) quit(GENOME_ERROR);

    /* Creating Genome */
    head = infoGene(n, m, b, 0);
    if (head  == NULL) quit(GENOME_ERROR);

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
    head = addToBack(head, NULL); // Can likely remove

    removeStrandedNodes(head);
    return head;
}

void createStickBallCreature(creature * newBorn) {
    /* Reading genome */
    int numNodes   = 0;
    int numBones   = 0;
    int numMuscles = 0;
    int numAxons   = 0;

    gene * currentGenome = newBorn->genome;
    stickball * components = ((stickball*) newBorn->components);
    while (currentGenome != NULL) {
        int a;
        int b;
        switch (currentGenome->start) {
            case 'n':
                components->nodes[numNodes].loc.x = currentGenome->fData[xposi];
                components->nodes[numNodes].loc.y = currentGenome->fData[yposi];
                components->nodes[numNodes].loc.z = currentGenome->fData[zposi];

                components->nodes[numNodes].vel.x = 0.0;
                components->nodes[numNodes].vel.y = 0.0;
                components->nodes[numNodes].vel.z = 0.0;

                components->nodes[numNodes].force.x = 0.0;
                components->nodes[numNodes].force.y = 0.0;
                components->nodes[numNodes].force.z = 0.0;

                components->nodes[numNodes].mass     = currentGenome->fData[mass];
                components->nodes[numNodes].friction = currentGenome->fData[fric];

                numNodes++;
                break;
            case 'b':
                a = components->bones[numBones].a = currentGenome->iData[0];
                b = components->bones[numBones].b = currentGenome->iData[1];
                components->bones[numBones].length = euc(components->nodes[a].loc, components->nodes[b].loc);
                numBones++;
                break;
            case 'm':
                a = components->muscles[numMuscles].a = currentGenome->iData[0];
                b = components->muscles[numMuscles].b = currentGenome->iData[1];
                components->muscles[numMuscles].origLength = euc(components->nodes[a].loc, components->nodes[b].loc);
                numMuscles++;
                break;
            case 'a':
                break; // No axons
                components->axons[numAxons].a      = currentGenome->iData[connectionA];
                components->axons[numAxons].b      = currentGenome->iData[connectionB];
                components->axons[numAxons].layer  = currentGenome->iData[layerE];
                components->axons[numAxons].weight = currentGenome->fData[weightE];
                numAxons++;
                break;
            default:
                break;
        }
        currentGenome = currentGenome->next;
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
        if (current->iData[0] == a && current->iData[1] == b) return false;
        if (current->iData[1] == a && current->iData[0] == b) return false;
    }
    return true;
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


gene * addAxon(int a, int b, int l) {
    gene * newAxon = (gene*) malloc(sizeof(gene));
    if (newAxon == NULL) quit(MALLOC_ERROR);

    newAxon->start = 'a';
    newAxon->iData[connectionA] = a;
    newAxon->iData[connectionB] = b;
    newAxon->iData[layerE]      = l;
    newAxon->fData[weightE]     = pmRandf(1.0);
    newAxon->endof = '\0';
    newAxon->next = NULL;
    return newAxon;
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
























