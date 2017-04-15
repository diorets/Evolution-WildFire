#include "Generation/generation.h"

#include <stdlib.h>            // NULL
#include "Math/myMath.h"       // Com, euc
#include "global.h"            // Globals
#include "Generation/genes.h"  // Various Genes
#include "Functional/list.h"   // Various Linked List Functions
#include "ErrorHandle/error.h" // Quit

gene* createGenome(gene * head, int a, int b, int c, int d) {
//    switch (dataCollection) {
//        case 1:
//        case 2:
//        case 3:
//            if (data[4] != -1) {
//                srand(data[4]);
//            }
//            break;
//        default:
//            break;
//    }
    if (b + c > comb(a)) quit(GENOME_ERROR);
    /* Creating Genome */
    head = infoGene(a, b, c, d);
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

//    int hiddenLayerSize = HIDDEN_LAYER_SIZE;
//    for (int i = 0; i < sizes[mus]; i++) {
//        for (int j = 0; j < hiddenLayerSize; j++) {
//            head = addToBack(head, addAxon(i, j, 0));
//        }
//    }
//    for (int i = 0; i < hiddenLayerSize; i++) {
//        for (int j = 0; j < sizes[mus]; j++) {
//            head = addToBack(head, addAxon(i, j, 1));
//        }
//    }

    head = addToBack(head, NULL); // Can likely remove

    return head;
}

void createCreature(creature * newBorn) {
    /* Reading genome */
    int numNodes   = 0;
    int numBones   = 0;
    int numMuscles = 0;
    int numAxons   = 0;

    gene * currentGenome = newBorn->genome;
    while (currentGenome != NULL) {
        int a;
        int b;
        switch (currentGenome->start) {
            case 'n':
                newBorn->nodes[numNodes].loc.x = currentGenome->fData[xposi];
                newBorn->nodes[numNodes].loc.y = currentGenome->fData[yposi];
                newBorn->nodes[numNodes].loc.z = currentGenome->fData[zposi];

                newBorn->nodes[numNodes].vel.x = 0.0;
                newBorn->nodes[numNodes].vel.y = 0.0;
                newBorn->nodes[numNodes].vel.z = 0.0;

                newBorn->nodes[numNodes].force.x = 0.0;
                newBorn->nodes[numNodes].force.y = 0.0;
                newBorn->nodes[numNodes].force.z = 0.0;

                newBorn->nodes[numNodes].mass     = currentGenome->fData[mass];
                newBorn->nodes[numNodes].friction = currentGenome->fData[fric];

                numNodes++;
                break;
            case 'b':
                a = newBorn->bones[numBones].a = currentGenome->iData[0];
                b = newBorn->bones[numBones].b = currentGenome->iData[1];
                newBorn->bones[numBones].length = euc(newBorn->nodes[a].loc, newBorn->nodes[b].loc);
                numBones++;
                break;
            case 'm':
                a = newBorn->muscles[numMuscles].a = currentGenome->iData[0];
                b = newBorn->muscles[numMuscles].b = currentGenome->iData[1];
                newBorn->muscles[numMuscles].origLength = euc(newBorn->nodes[a].loc, newBorn->nodes[b].loc);
                numMuscles++;
                break;
            case 'a':
                break; // No axons
                newBorn->axons[numAxons].a      = currentGenome->iData[connectionA];
                newBorn->axons[numAxons].b      = currentGenome->iData[connectionB];
                newBorn->axons[numAxons].layer  = currentGenome->iData[layerE];
                newBorn->axons[numAxons].weight = currentGenome->fData[weightE];
                numAxons++;
                break;
            default:
                break;
        }
        currentGenome = currentGenome->next;
    }
    newBorn->origin = getCom(*newBorn);
    newBorn->fitness= 0.0;
    // Add a check here for valid creature
    return;
}















