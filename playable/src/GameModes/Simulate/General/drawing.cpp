#include "GameModes/Simulate/General/drawing.h"

#include <stdio.h>
#include "global.h" // playBackSpeed
#include "Glut/myGlut.h" // Color
#include "Visual/text.h" // drawText

#include "GameModes/Simulate/General/drawing.h"
#include "GameModes/Simulate/StickBall/stickBallDrawing.h"
#include "GameModes/Simulate/Turbine/turbineDrawing.h"
#include "GameModes/Simulate/Cannon/cannonDrawing.h"

#include "Visual/basic.h"          // 2D, 3D, Various Shapes & Text
#include "Visual/objects.h"        // Drawing Objects
//#include "GameModes/Simulate/drawing.h"        // Drawing Objects
#include "GameModes/Simulate/General/drawing.h"
#include "ErrorHandle/error.h"
void drawSystem(int system, creature input, int genSize, int gen, int id, int simTime, int maxTime) {
    switch(system) {
        case stickballE: stickBallSystem(input, genSize, gen, id, simTime, maxTime); break;
        case turbineE: turbineSystem(input, genSize, gen, id, simTime, maxTime); break;
        case cannonE: cannonSystem(input, genSize, gen, id, simTime, maxTime); break;
        default: quit(INPUT_ERROR);
    }
    return;
}


void drawDetails(int genSize, int gen, int id, int simTime, int maxTime) {
    char str[50];
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    sprintf(str, "Creature ID: %d / %d (%.2f%%)", id, genSize, 100 * id / (double) genSize);
    int deltaX = drawText(str, 6, -100, false);
    glColor4f(WHITE, 0.6);
    drawSquare(0, 0, deltaX + 5, 20 * 5);

    /* General Stats */
    glColor3f(BLACK);

    sprintf(str, "Generation:  %d", gen);
    int height = 0;
    drawText(str, 2, height += 20, false);
    sprintf(str, "Creature ID: %d / %d (%.2f%%)", id, genSize, 100 * id / (double) genSize);
    drawText(str, 5, height += 20, false);
    if (simTime < 1000000) {
        sprintf(str, "Time: %d / %d (%.2f%%)", simTime, maxTime, 100 * simTime / (double) maxTime);
    } else {
        sprintf(str, "Time: Error, value too large.");
    }
    drawText(str, 5, height += 20, false);
    sprintf(str, "Playback Speed: %dx", playBackSpeed);
    drawText(str, 5, height += 20, false);




    return;
}

























