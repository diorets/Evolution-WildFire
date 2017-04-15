#include "Generation/mutations.h"

#include <math.h> // Fabs
#include "Math/myMath.h"       // Chance, vec, euc
#include "Generation/Genes.h"  // Various Genes
#include "Functional/list.h"   // Various linked list functions
#include "ErrorHandle/error.h" // Quit
void modifyWeights(gene * head, double modifyChance) {
    FOR_ALL(head, 'a') {
        if (chance(modifyChance)) {
            current->fData[weightE] += pmRandf(0.05);
        }
    }
    return;
}

#include "global.h"
#include <stdio.h>
void mutateGenome(creature * toMutate) {
//    switch (dataCollection) {
//        case 1:
//            static int i = 0;
//            if (gen == (int) ceil(log(genSize)/log(2))) { // When we establish the best of 10,000. gen == ceil(ln(genSize)/ln(2))
//                FOR_ALL(toMutate->genome, 'n') {
//                    current->fData[xposi] =  -CAGESIZE + (2 * CAGESIZE * i /(double) genSize);
//                    break;
//                }
//                i++;
//            }
//            return;
//        case 2:
//            addNode      (toMutate->genome, 100, 15,  15);
//            varifyGenome    (toMutate->genome);
//            return;
//        default:
//            break;
//    }



    /* Nodes Mods */
    relocateNodes(toMutate->genome, toMutate->genome->fData[0]); // These assume a valid location exists
    shiftNodes   (toMutate->genome, toMutate->genome->fData[1], 0.1);
    addNode      (toMutate->genome, toMutate->genome->fData[2], 15,  15);
    removeNode   (toMutate->genome, toMutate->genome->fData[3]);
//    changeStats     (toMutate->genome, chance); // Least important

    /* Connections */
    //changeConnection(toMutate->genome, chance); // goto diferent muscles
    addConnection   (toMutate->genome, toMutate->genome->fData[4]);
    removeConnection(toMutate->genome, toMutate->genome->fData[5]);
    swapConnection  (toMutate->genome, toMutate->genome->fData[6]);

    /* Neural Network */
    // modifyWeights(toMutate->genome, toMutate->genome->fData[6]);


    /* Varification */
    varifyGenome    (toMutate->genome);
    return;
}

void addAxons(gene * head, int attachTo) {
    int hiddenLayerSize = HIDDEN_LAYER_SIZE;
    for (int j = 0; j < hiddenLayerSize; j++) {
        head = addToBack(head, addAxon(attachTo, j, 0));
        head = addToBack(head, addAxon(j, attachTo, 1));
        head->iData[tot] += 2;
        head->iData[neu] += 2;
    }
    return;
}

void reduceAxons(gene * head, int muscleRemoved) {
    /* Decrement input and output layers to account for removed muscle */
    FOR_ALL(head, 'a') {
        int layer = current->iData[layerE];
        int * connA = &current->iData[connectionA];
        int * connB = &current->iData[connectionB];

        switch(layer) {
            case 0:
                if ((*connA) == muscleRemoved) {       // Remove
                    removeItem(head, current);
                    head->iData[neu]--;
                    head->iData[tot]--;
                } else if ((*connA) > muscleRemoved) { // Adjust
                    (*connA)--;
                }
                break;
            case 1:
                if ((*connB) == muscleRemoved) {       // Remove
                    removeItem(head, current);
                    head->iData[neu]--;
                    head->iData[tot]--;
                } else if ((*connB) > muscleRemoved) { // Adjust
                    (*connB)--;
                }
                break;
            default:
                break;
        }
    }
    return;
}


void shiftNodes(gene * head, double shiftChance, double amount) {
    FOR_ALL(head, 'n') if (chance(shiftChance)) {
        double x = current->fData[xposi];
        double y = current->fData[yposi];
        double z = current->fData[zposi];
        posi newPos = getValidShift(head, x, y, z, amount);
        current->fData[xposi] = newPos.x;
        current->fData[yposi] = newPos.y;
        current->fData[zposi] = newPos.z;
    }
    return;
}

void relocateNodes(gene * head, double shiftChance) {
    FOR_ALL(head, 'n') if (chance(shiftChance)) {
        posi newPos = getValidPosition(head);
        current->fData[xposi] = newPos.x;
        current->fData[yposi] = newPos.y;
        current->fData[zposi] = newPos.z;
    }
    return;
}

void addNode(gene * head, double nodeChance, double muscleChance, double boneChance) {
    if (head->iData[nod] >= MAX_ELEMENTS) return;
    if (chance(nodeChance)) {
        /* Add Node */
        addToBack(head, nodeGene(head));
        head->iData[tot]++;
        head->iData[nod]++;

        /* Add (Potentially) Multiple Connections */
        int counter = 0;
        FOR_ALL(head, 'n') {
            if (counter == head->iData[nod] - 1) continue; // Counter is at new node
            // Function can be improved to make it more evenly random
            if (chance(muscleChance)) {
                if (head->iData[mus] >= MAX_ELEMENTS) continue;
                addAxons(head, head->iData[mus]);
                addToBack(head, muscleGene(counter, head->iData[nod] - 1));
                head->iData[tot]++;
                head->iData[mus]++;
            }
            else if (chance(boneChance)) {
                if (head->iData[bon] >= MAX_ELEMENTS) continue;
                addToBack(head, boneGene(counter, head->iData[nod] - 1));
                head->iData[tot]++;
                head->iData[bon]++;
            }
            counter++;
        }
    }
    return;
}

void removeNode(gene * head, double removeChance) {
    if (head->iData[nod] <= 1) return;

    if (chance(removeChance)) {
        int toRemove = rand() % head->iData[nod];

        /* Remove Node */
        int nodeNum = 0;
        FOR_ALL(head, 'n') {
            if (nodeNum == toRemove) {
                removeItem(head, current);
                head->iData[tot]--;
                head->iData[nod]--;
                break;
            }
            nodeNum++;
        }

        /* Remove Connected Muscles */
        int muscleNum = 0;
        FOR_ALL(head, 'm') {
            if (current->iData[0] == toRemove || current->iData[1] == toRemove) {
                reduceAxons(head, muscleNum--);
                removeItem(head, current);
                head->iData[tot]--;
                head->iData[mus]--;
            }
            muscleNum++;
        }

        /* Remove Connected Bones */
        FOR_ALL(head, 'b') {
            if (current->iData[0] == toRemove || current->iData[1] == toRemove) {
                removeItem(head, current);
                head->iData[tot]--;
                head->iData[bon]--;
            }
        }

        /* Adjust Connections */ // Removing node 3, means conn > 3 need to be decremented
        for (gene * current = head; current != NULL; current = current->next) {
            if (current->start == 'm' || current->start == 'b') {
                if (current->iData[0] >= toRemove) current->iData[0]--; // should never be ==
                if (current->iData[1] >= toRemove) current->iData[1]--;
            }
        }
    }
    return;
}

void addConnection(gene * head, double addChance) {
    int numNodes   = head->iData[nod];
    int numMuscles = head->iData[mus];
    int numBones   = head->iData[bon];

    if (numMuscles + numBones + 1 > comb(numNodes)) return; // +1 for added

    if (chance(addChance)) {
        conn connection = goodConnection(head);
        if (chance(50)) {
            if (head->iData[mus] >= MAX_ELEMENTS) return;
            //addAxons(head, head->iData[mus]);
            addToBack(head, muscleGene(connection.a, connection.b));
            head->iData[tot]++;
            head->iData[mus]++;
        } else {
            if (head->iData[bon] >= MAX_ELEMENTS) return;
            addToBack(head, boneGene(connection.a, connection.b));
            head->iData[tot]++;
            head->iData[bon]++;
        }
    }
    return;
}

void removeConnection(gene * head, double addChance) {
    int numMuscles = head->iData[mus];
    int numBones   = head->iData[bon];

    if (numMuscles + numBones <= 0) return;

    if (chance(addChance)) {
        /* No Muscles -> Remove Bone */
        if (numMuscles <= 0) { // Cant Both be 0
            int toRemove = rand() % numBones;
            if (numBones == 1) toRemove = 0;
            int i = 0;
            FOR_ALL(head, 'b') {
                if (i == toRemove) {
                   removeItem(head, current);
                   head->iData[tot]--;
                   head->iData[bon]--;
                }
                i++;
            }
            return;
        }

        /* No Bones-> Remove Muscle */
        if (numBones <= 0) {
            int toRemove = rand() % numMuscles;
            if (numMuscles == 1) toRemove = 0;
            int muscleNum = 0;
            FOR_ALL(head, 'm') {
                if (muscleNum == toRemove) {
                    //reduceAxons(head, muscleNum--);
                    removeItem(head, current);
                    head->iData[tot]--;
                    head->iData[mus]--;
                }
                muscleNum++;
            }
            return;
        }

        if (chance(50)) {
            /* Remove Muscle */
            int toRemove = rand() % numMuscles;
            if (numMuscles == 1) toRemove = 0;
            int muscleNum = 0;
            FOR_ALL(head, 'm') {
                if (muscleNum == toRemove) {
                    reduceAxons(head, muscleNum--);
                    removeItem(head, current);
                    head->iData[tot]--;
                    head->iData[mus]--;
                    return;
                }
                muscleNum++;
            }
        } else {
            /* Remove Bone */
            int toRemove = rand() % numBones;
            if (numBones == 1) toRemove = 0;
            int i = 0;
            FOR_ALL(head, 'b') {
                if (i == toRemove) {
                    removeItem(head, current);
                    head->iData[tot]--;
                    head->iData[bon]--;
                    return;
                }
                i++;
            }
        }
    }

    int numNodes = head->iData[nod];
    int needed[numNodes];
    for (int i = 0; i < numNodes; i++) {
        needed[i] = i;
    }
    FOR_ALL_GENES(head) if (current->start == 'm' || current->start == 'b') {
        needed[current->iData[0]] = -1;
        needed[current->iData[1]] = -1;
    }


    /* Remove Node */
    int nodeNum = 0;
    FOR_ALL(head, 'n') {
        if (needed[i] == -1) continue;
        if (nodeNum == needed[i]) {
            removeItem(head, current);
            head->iData[tot]--;
            head->iData[nod]--;
            break;
        }
        nodeNum++;
    }
    return;
}

// This function will break if muscles and bones have different attributes
void swapConnection(gene * head, double swapChance) {
    /* Muscle -> Bone */ // Can make this fast by combining

//    addAxons(head, head->iData[mus]);
//    reduceAxons(head, muscleNum--);
    int muscleNum = 0;
    FOR_ALL(head, 'm') if (chance(swapChance)) {
        if (head->iData[bon] >= MAX_ELEMENTS) break;
        int a = current->iData[0];
        int b = current->iData[1];
        if (connectionExists(head, a, b)) {
//            reduceAxons(head, muscleNum--);

            current->start = 'b';
            head->iData[mus]--;
            head->iData[bon]++;
        }
        muscleNum++;
    }

    FOR_ALL(head, 'b') {
        if (chance(swapChance)) {
            if (head->iData[mus] >= MAX_ELEMENTS) break;
            int a = current->iData[0];
            int b = current->iData[1];
            if (connectionExists(head, a, b)) {
//                addAxons(head, /* How do I know what muscle this is */);

                current->start = 'm';
                head->iData[mus]++;
                head->iData[bon]--;
            }
        }
    }
    return;
}

void varifyGenome(gene * head) {
    int numGenes   = head->iData[tot];
    int numNodes   = head->iData[nod];
    int numMuscles = head->iData[mus];
    int numBones   = head->iData[bon];
    int numAxons   = head->iData[neu];

    /* Number Errors */
    if (numNodes    > MAX_ELEMENTS || numNodes   <= 0) quit(GENOME_ERROR);
    if (numMuscles  > MAX_ELEMENTS || numMuscles <  0) quit(GENOME_ERROR);
    if (numBones    > MAX_ELEMENTS || numBones   <  0) quit(GENOME_ERROR);

    if (numNodes + numMuscles + numBones + numAxons + 1 != numGenes) quit(GENOME_ERROR);

    if (numMuscles + numBones > comb(numNodes)) quit(GENOME_ERROR);

    /* Connection Errors */
    for (gene * i = head; i != NULL; i = i->next) {
        for (gene * j = i->next; j != NULL; j = j->next) {
            bool error = false;
            /* Only look at muscles and bones */
            if (i->start != 'b' && i->start != 'm') continue;
            if (j->start != 'b' && j->start != 'm') continue;

            /* Any two sets of connections */
            int a1 = i->iData[0];
            int a2 = i->iData[1];
            int b1 = j->iData[0];
            int b2 = j->iData[1];

            /* Self-Attachment */
            if (a1 == a2) error = true;
            if (b1 == b2) error = true;
            if (error) quit(GENOME_ERROR);

            /* Duplicate Attachments */
            if (a1 == b1 && a2 == b2) error = true;
            if (a1 == b2 && a2 == b1) error = true;
            if (error) quit(GENOME_ERROR);
        }
    }

    /* Node Location Errors */
    for (gene * i = head; i != NULL; i = i->next) {
        if (i->start != 'n') continue;
        bool error = false;
        double x1 = i->fData[xposi];
        double y1 = i->fData[yposi];
        double z1 = i->fData[zposi];

        for (gene * j = i->next; j != NULL; j = j->next) {
            if (j->start != 'n') continue;
            double x2 = j->fData[xposi];
            double y2 = j->fData[yposi];
            double z2 = j->fData[zposi];

            /* Bounds */
            if (fabs(x1 - x2) < MIN_NODE_DISTANCE &&
                fabs(y1 - y2) < MIN_NODE_DISTANCE &&
                fabs(z1 - z2) < MIN_NODE_DISTANCE ){
                error = true;
            }
            if (error) quit(GENOME_ERROR);

            /* Overlap  & Proximit*/
            if (euc(vec(x1, y1, z1), vec(x2, y2, z2)) < MIN_NODE_DISTANCE) quit(GENOME_ERROR);
            if ((fabs(x2 - x1) < MIN_NODE_DISTANCE) &&
                (fabs(y2 - y1) < MIN_NODE_DISTANCE) &&
                (fabs(z2 - z1) < MIN_NODE_DISTANCE)) {
                    error = true;;
            }
            if (error) quit(GENOME_ERROR);
        }
    }
    return;
}

/** TYPES OF MUTATIONS
INFO: None -> Must be updated
NODES:
    Change Mass (Big / Small)
    Change Friction
BONES:
    Change Attachment (?)
MUSCLES:
    Change Attachment (?)
NEURONS:

MUTABILITY:
    Large Change
    Small Change

**/

























