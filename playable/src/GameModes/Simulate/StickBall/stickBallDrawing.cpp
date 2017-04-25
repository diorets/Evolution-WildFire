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

void stickBallSystem(creature input, int genSize, int gen, int id, int simTime, int maxTime) {
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
    drawDetails(genSize, gen, id, simTime, maxTime);


    glutSwapBuffers(); // Make it all visible
}

static void assignCords(button * b) {
    switch (b->group) {
        case 0:
            {
            int numButtons = 2;//getNumButtons(buttons);
            double width = (wx - 5) / (double) numButtons;
            b->x = b->id * width + 5;
            b->y = wy - 45;
            b->w = (b->id+1) * width;
            b->h = wy - 5;
            }
            break;
        case 1:
            b->x = wx * 0.90;
            b->y = 45 * (b->id - 1) - 45;
            b->w = wx;
            b->h = 45 * (b->id - 1);
        break;
        default: return;
    }
    return;
}
#include "GameModes/inputFunctions.h"
void createMenu() {
    /* Create Buttons */
    if (buttons == NULL) {
        const char *names[]  = {"Fitness Graph",
                                "Generation Graph",
                                "Exit", "Fullscreen",
                                "Back",
                                "Creature Info",
                                "Generation Info",
                                "Mutation Info",
                                "Selection Info"};
        void (*callbacks[])() = {toggleCreatureGraph,
                                 toggleGenerationGraph,
                                 normalExit,
                                 toggleFullScreen,
                                 nullFunc,
                                 nullFunc,
                                 nullFunc,
                                 nullFunc,
                                 nullFunc};
        if (NUMELEMS(names) != NUMELEMS(callbacks)) quit(UNSPECIFIC_ERROR);

        buttons = clearButtons(buttons);
        for (int i = 0; i < 2; i++) {
            buttons = createButton(names[i], (*callbacks[i]), assignCords, i, 0);
        }
        for (int i = 2; i < (int) NUMELEMS(names); i++) {
            buttons = createButton(names[i], (*callbacks[i]), assignCords, i, 1);
        }

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









