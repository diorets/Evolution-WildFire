#include "GameModes/Simulate/General/drawing.h"

#include <stdio.h>
#include "global.h" // playBackSpeed
#include "Glut/myGlut.h" // Color
#include "Visual/text.h" // drawText

#include "GameModes/Simulate/General/drawing.h"
#include "GameModes/Simulate/SpDrawing/stickBallDrawing.h"

#include "Visual/basic.h"          // 2D, 3D, Various Shapes & Text
#include "Visual/objects.h"        // Drawing Objects
#include "GameModes/Simulate/drawing.h"        // Drawing Objects
#include "GameModes/Simulate/General/drawing.h"

void drawSystem(int system, creature input, int genSize, int gen, int id, int simTime, int maxTime) {
    switch(system) {
        case stickballE:
            stickBallSystem(input, genSize, gen, id, simTime, maxTime);
            break;
        default:
            break;
    }
    return;
}


void drawDetails(int genSize, int gen, int id, int simTime, int maxTime) {
    /* General Stats */
    glColor3f(BLACK);
    char str[50];
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
