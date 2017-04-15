#include "Generation/generation.h"

#include <stdlib.h>            // NULL
#include "Math/myMath.h"       // Com, euc
#include "global.h"            // Globals
#include "Generation/genes.h"  // Various Genes
#include "Functional/list.h"   // Various Linked List Functions
#include "ErrorHandle/error.h" // Quit

#include "Generation/mutations.h"
gene* createGenome(gene * head, int a, int b, int c, int d) {
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
    head = addToBack(head, NULL); // Can likely remove

    removeStrandedNodes(head);

    return head;
}
#include <stdio.h>

void createCreature(creature * newBorn) {
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
                //printf("%d, %d\n", a, b);
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















