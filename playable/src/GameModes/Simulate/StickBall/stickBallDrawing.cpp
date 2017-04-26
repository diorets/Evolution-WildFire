#include "GameModes/Simulate/StickBall/stickBallDrawing.h"

#include "Glut/myGlut.h"           // Glut
#include "Visual/basic.h"          // 2D, 3D, Various Shapes & Text
#include "Visual/objects.h"        // Drawing Objects
#include "Math/myMath.h"
#include "ErrorHandle/error.h"
#include "global.h"
#include <math.h>
#include <stdio.h>
#include "GameModes/Simulate/General/drawing.h"

#include "Functional/buttons.h"
#include "Functional/graph.h"
void drawStickBall(creature individual);
static void createMenu();
void displaySystemInfo(creature * pop, int genSize, int id);

void stickBallSystem(creature * pop, int genSize, int gen, int id, int simTime, int maxTime) {
    creature input = pop[id];

    /* Clear, Reset, Camera */
    glutDrawing();

    /* 3D Drawing */
    reenable3D();
    drawGround(500, 10, 10);
    drawSun();
    drawStickBall(input);

    /* 2D drawing */
    enable2D();
    bool twoG = globalData[creatureFitnessE].g.display && globalData[generationFitnessE].g.display;
    drawGraph(globalData[creatureFitnessE].g, 0, 0.5 * wx * (1.0 + !twoG));
    drawGraph(globalData[generationFitnessE].g, 0.5 * wx * twoG, wx);

    createMenu();

    for (button * b = buttons; b != NULL; b = b->next) {
        if (b->toggled && !strcmp("System Info", b->label)) {
            displaySystemInfo(pop, genSize, id);
        }
    }
    drawDetails(genSize, gen, id, simTime, maxTime);


    glutSwapBuffers(); // Make it all visible
}

static void assignCords(button * b) {
    switch (b->group) {
        case 0:
            {
            int numButtons = getNumButtons(buttons, b->group);
            double width = (wx - 5) / (double) numButtons;
            b->x = b->id * width + 5;
            b->y = wy - 45;
            b->w = (b->id+1) * width;
            b->h = wy - 5;
            }
            break;
        case 1:
            b->x = wx * 0.90;
            b->y = 45 * (b->id);
            b->w = wx;
            b->h = 45 * (b->id + 1);
        break;
        default: return;
    }
    return;
}

void displaySystemInfo(creature * pop, int genSize, int id) {
    int yTop = 5 * 20;
    int yBottom = 5 * 20 + 20 * 14 + 20; // 50 % of screen
    drawSquare(0, yBottom, 220, yTop);
    glColor3f(WHITE);
    int height = yTop;
    char str[100];
    sprintf(str, "Creature Genes"); drawText(str, 0, height += 20, false, false);
    sprintf(str, "Total: %d",   pop[id].genome->iData[tot]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Nodes: %d",   pop[id].genome->iData[nod]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Bones: %d",   pop[id].genome->iData[bon]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Muscles: %d", pop[id].genome->iData[mus]); drawText(str, 20, height += 20, false, false);
    height+=20;
    double popStats[] = {0, 0, 0, 0, 0};
    for (int i = 0; i < genSize; i++) {
        popStats[0] += pop[i].genome->iData[tot];
        popStats[1] += pop[i].genome->iData[nod];
        popStats[2] += pop[i].genome->iData[bon];
        popStats[3] += pop[i].genome->iData[mus];
        popStats[4] += pop[i].fitness; // Only works because fitness for pop[i > id] = 0
    }
    double oneOverGenSize = 1.0 / (double) genSize;
    popStats[0] *= oneOverGenSize;
    popStats[1] *= oneOverGenSize;
    popStats[2] *= oneOverGenSize;
    popStats[3] *= oneOverGenSize;
    popStats[4] *= id == 0 ? 0 : 1.0 / (double) id;

    sprintf(str, "Population Genes"); drawText(str, 0, height += 20, false, false);
    sprintf(str, "Total: %.2f",   popStats[0]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Nodes: %.2f",   popStats[1]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Bones: %.2f",   popStats[2]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Muscles: %.2f", popStats[3]); drawText(str, 20, height += 20, false, false);
    height+=20;

    sprintf(str, "Population Fitness"); drawText(str, 0, height += 20, false, false);
    sprintf(str, "Current Average: %.2f",   popStats[4]); drawText(str, 20, height += 20, false, false);


}

#include "GameModes/inputFunctions.h"
void createMenu() {
    /* Create Buttons */
    if (buttons == NULL) {
        int i = 0;
        int j = 0;
        buttons = createButton("Fitness Graph"   , toggleCreatureGraph  , assignCords, j++    , 0, true , false);
        buttons = createButton("Generation Graph", toggleGenerationGraph, assignCords, j++    , 0, true , false);
        buttons = createButton("Exit"            , normalExit           , assignCords, i++    , 1, false, false);
        buttons = createButton("Back to Menu"    , nullFunc             , assignCords, i++    , 1, false, false);
        buttons = createButton("Full Screen"     , toggleFullScreen     , assignCords, i++    , 1, true , true );
        buttons = createButton("System Info"     , nullFunc             , assignCords, j++    , 0, true , false);
        buttons = createButton("Mutations"       , nullFunc             , assignCords, j++    , 0, true , false);
        buttons = createButton("Selection"       , nullFunc             , assignCords, j++    , 0, true , false);
        buttons = createButton("Save Genome"     , nullFunc             , assignCords, (++i)++, 1, false , false);
    }

    /* Draw Background */
    glColor3f(BLACK);
    drawSquare(0, wy, wx, wy-50);

    /* Highlighting */
    for (button * b = buttons; b != NULL; b = b->next) {
        b->highlighted = hoveringOver(b);
    }

    drawButtons();
    return;
}

void drawStickBall(creature individual) {
    int * sizes = individual.genome->iData;
    stickball * components = ((stickball*) individual.components);
    posi com = getCom(individual);

    /* Nodes */
    glColor3f(BLACK);
    for (int i = 0; i < sizes[nod]; i++) {
        posi loc = components->nodes[i].loc;
        drawSphere(loc.x, loc.y, loc.z, RADIUS);
    }

    double thickness = RADIUS * 0.5;
    glColor3f(WHITE);
    for (int i = 0; i < sizes[bon]; i++) {
        int a = components->bones[i].a;
        int b = components->bones[i].b;
        if (a == b) quit(GENOME_ERROR); // Do I need this check?
        //printf("%d, %d\n", a, b);
        posi locA = components->nodes[a].loc;
        posi locB = components->nodes[b].loc;
        drawCylinder(locA.x, locA.y, locA.z,
                      locB.x, locB.y - 0.1, locB.z,
                        thickness ,     5);
    }

    /* Muscle */
    glColor3f(RED);
    for (int i = 0; i < sizes[mus]; i++) {
        int a = components->muscles[i].a;
        int b = components->muscles[i].b;
        if (a == b) quit(GENOME_ERROR); // Do I need this check?

        posi locA = components->nodes[a].loc;
        posi locB = components->nodes[b].loc;
        double minV = -1.3 * components->muscles[i].origLength;
        double maxV =  1.3 * components->muscles[i].origLength;
        double expansivity = 8 * euc(locA, locB) / (maxV - minV) * euc(locA, locB) / (maxV - minV); // not sure why 8 works
        drawCylinder(locA.x, locA.y, locA.z,
                      locB.x, locB.y + 0.1, locB.z,
                       thickness * expansivity,     5);
    }

    /* COMs / Origins */
    glColor3f(WHITE);
    drawSphere(com.x, com.y, com.z, 0.1); // COM
    drawSphere(com.x, com.y,     0, 0.1); // Ground COM
    drawSphere(components->origin.x, components->origin.y, components->origin.z, 0.1); // Origin
    glColor3f(RED);
    drawLine(com.x, com.y, 0.1, // Distance
             components->origin.x, components->origin.y, 0.1);
    return;
}









