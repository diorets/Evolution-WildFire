#include "GameModes/Simulate/Cube/cubeDrawing.h"
#include "Glut/myGlut.h"
#include "Visual/basic.h"          // 2D, 3D, Various Shapes & Text
#include "Visual/objects.h"        // Drawing Objects
#include "GameModes/Simulate/General/drawing.h"

#include <stdio.h>
void drawCubeCreature(creature individual) {
    const int numBlocks = individual.genome->iData[TE::numBlocks];
    const int numSprings = individual.genome->iData[CUE::numSprings];
    const double radius = individual.genome->fData[TE::radius];
    cube * components = ((cube*) individual.components);

    glColor3f(BLUE);
    for (int i = 0; i < numBlocks; i++) {
        posi loc = components->blocks[i].loc;
        drawSphere(loc.x, loc.y, loc.z, radius);
    }

    glColor3f(RED);
    for (int i = 0; i < numSprings; i++) {
        int a = ((cube*) individual.components)->springs[i].a;
        int b = components->springs[i].b;

        posi locA = components->blocks[a].loc;
        posi locB = components->blocks[b].loc;

        drawCylinder(locA.x, locA.y, locA.z,
                     locB.x, locB.y, locB.z,
                     0.1, 10);
    }
}

void cubeSystem(creature * pop, int genSize, int gen, int id, int simTime, int maxTime) {
    creature input = pop[id];
    /* Clear, Reset, Camera */
    glutDrawing();

    /* 3D Drawing */
    reenable3D();
    drawGround(50, 10, 20);
    drawCubeCreature(input);

    /* 2D drawing */
    enable2D();
    drawDetails(genSize, gen, id, simTime, maxTime);
    glutSwapBuffers(); // Make it all visible
}
































