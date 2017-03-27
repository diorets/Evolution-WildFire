#include "GameModes/Mutation/mut.h"

#include "global.h" // Specimen
#include "Glut/myGlut.h" // SwapBuffers
#include "Generation/generation.h" // Create Creatures
#include "Generation/mutations.h" // Various Mutations
#include "Generation/genes.h" // PrintGenome
#include "Visual/basic.h" // GlutDrawing
#include "Visual/objects.h" // Ground & Creature

static void constantMutations(double allChance);

void mutateMode() {
    static int i = 0;

    /* Mutations */
    constantMutations(10.0);

    /* Drawing */
    if (i % 10 == 0) {
        printGenome(0, false, true);
        glutDrawing();
//        drawGround();
        createCreature(&specimen[0]);
        drawCreature(specimen[0], true);
        glutSwapBuffers();
    }
    i++;
    return;
}

static void constantMutations(double allChance) {
    shiftNodes      (specimen[0].genome, allChance, 0.1);
    relocateNodes   (specimen[0].genome, allChance);
    addNode         (specimen[0].genome, allChance, 10, 10);
    removeNode      (specimen[0].genome, allChance);
    addConnection   (specimen[0].genome, allChance);
    removeConnection(specimen[0].genome, allChance);
    swapConnection  (specimen[0].genome, allChance);
    varifyGenome(specimen[0].genome);
    return;
}

