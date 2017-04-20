#include "GameModes/Simulate/Cannon/cannonDrawing.h"
#include "Glut/myGlut.h"
#include "Visual/basic.h"          // 2D, 3D, Various Shapes & Text
#include "Visual/objects.h"
#include "GameModes/Simulate/General/drawing.h"

void drawCannon(creature individual);
void writeCoeff(creature ind);

#include "Functional/sleep.h"
void cannonSystem(creature input, int genSize, int gen, int id, int simTime, int maxTime) {
    /* Clear, Reset, Camera */
    glutDrawing();

    /* 3D Drawing */
    reenable3D();
    drawGround(100, 10, 10);
    drawCannon(input);


    /* 2D drawing */
    enable2D();
    drawDetails(genSize, gen, id, simTime, maxTime);
    writeCoeff(input);
    sleep_ms(1);
    glutSwapBuffers(); // Make it all visible
}

#include <stdio.h>
#include "global.h"
void writeCoeff(creature input) {
    glColor3f(BLACK);
    char str[100];
    sprintf(str, "Coeff: ");

    int numPara = input.genome->iData[CE::numPara];
    for (int i = 0; i < numPara; i++) {
        char str2[100];
        sprintf(str2, "%+6.2f, ", input.genome->next->fData[i]);
        strcat(str, str2);
    }
    int height = wy - 100;

    drawText(str, 2, height, false);

}

#include <math.h>
void drawCannon(creature individual) {
    double angle = ((cannon*) individual.components)->angle;

    int * targetNum = &((cannon*) individual.components)->targetNum;
    double targetX = ((cannon*) individual.components)->target[*targetNum];

    double r = 3.0;
    double h = CE::h;
    glColor3f(GREEN);
    drawSphere(targetX, 0, 0, 0.3);
    drawSphere(50, 0, 0, 1);

    glColor3f(BLACK);
    drawCylinder(0, 0, h,
                 r*cos(angle), 0, r*sin(angle) + h + 0.01,
                 0.3, 10);
    glColor3f(PURPLE);
    drawCylinder(0, 0, h,
                 r*cos(angle), 0, r*sin(angle) + h,
                 0.3, 50);
    drawSphere(0, 0, h, 0.5, HIGH_QUAL);

    posi l = ((cannon*) individual.components)->ball.loc;

    drawSphere(l.x, l.y, l.z, 0.25);




}










































