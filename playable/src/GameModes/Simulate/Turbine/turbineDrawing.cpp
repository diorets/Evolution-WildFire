#include "GameModes/Simulate/Turbine/turbineDrawing.h"
#include "Glut/myGlut.h"
#include "Visual/basic.h"          // 2D, 3D, Various Shapes & Text
#include "Visual/objects.h"        // Drawing Objects
#include "GameModes/Simulate/General/drawing.h"

void drawTurbine(creature individual);
void drawParticles(creature individual);

#include <stdio.h>
void turbineSystem(creature * pop, int genSize, int gen, int id, int simTime, int maxTime) {
    creature input = pop[id];
    /* Clear, Reset, Camera */
    glutDrawing();

    /* 3D Drawing */
    reenable3D();
    drawGround(50, 10, 20);
    drawTurbine(input);
    drawParticles(input);

    /* 2D drawing */
    enable2D();
    drawDetails(genSize, gen, id, simTime, maxTime);
    glutSwapBuffers(); // Make it all visible
}


void drawParticles(creature individual) {
    //glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(WHITE, 0.5);
    turbine * components = ((turbine*) individual.components);

    const int numParticles = individual.genome->iData[TE::numParticles];
    for (int i = 0; i < numParticles; i++) {
        posi loc = components->particles[i].loc;
        drawSphere(loc.x, loc.y, loc.z, 0.1);
    }
    //glEnable(GL_LIGHTING);

    return;
}

void drawTurbine(creature individual) {
    glColor3f(BLACK); // Z axis
	drawCylinder(0, 0, -10,
                 0, 0,  30,
                 0.3, 10);
    const int numBlocks = individual.genome->iData[TE::numBlocks];
    const double radius = individual.genome->fData[TE::radius];
    turbine * components = ((turbine*) individual.components);

    glColor3f(WHITE);
    double theta = components->blocks[0].angle;
    posi loc = components->blocks[0].loc;
    drawCube(loc.x, loc.y, loc.z, radius / 2, theta);

    glColor3f(BLUE);
    for (int i = 1; i < numBlocks; i++) {
        theta = components->blocks[i].angle;
        loc = components->blocks[i].loc;
        drawSphere(loc.x, loc.y, loc.z, radius);
    }
    return;
}
































