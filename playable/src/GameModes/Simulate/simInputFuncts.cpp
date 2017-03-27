#include "preproccessor.h"
#include "global.h"
#include "GameModes/mice.h" // Bound
#include "Visual/basic.h"      // drawText, enable2D
#include "Glut/myGlut.h"

void nextIntroMessage() {
    int boxy = wy * 0.70;
    if (bound(mousePos.x, mousePos.y, wx * 0.85, boxy, wx * 0.95, wy * 0.95)) {
        if (globalData[howToE].i > 0) {
            globalData[howToE].i++;
        }
        if (globalData[howToE].i > 6) globalData[howToE].i = 0;
    }
    return;
}

void mutateChange() {
    /* Modify Mutation Rates */
    for (int i = 0; i < 9; i++) {
        /* Handle Spaces Between Text */
        if ((i == 2) || (i == 5)) continue;
        int shift = i > 2;
        shift += i > 5;

        /* Get Starting Position of Options */
        double last1 = writeParagraph("Now, you must decide how to pick the next generation\n"
                   "How would you like to mutate them? Can you evolve a fast creature with 6 nodes?"
                   "", wx / 2, wy * 0.13, 24);
        last1 *= 1.2;
        int x1 = wx * 0.17;
        int x2 = wx * 0.23;
        int y =  last1 + 24 * (i + 4) - 5;
        int r = 11;


        /* Check and Apply Clicks */
        if (bound(mousePos.x, mousePos.y, x1-r, y-r, x1+r, y+r)) { // Inc value
            for (int j = 0; j < genSize; j++) {
                if (specimen[j].genome->fData[i - shift] > 0.01) {
                    specimen[j].genome->fData[i - shift] *= 0.7;
                }
            }
        }
        if (bound(mousePos.x, mousePos.y, x2-r, y-r, x2+r, y+r)) { // Dec value
            for (int j = 0; j < genSize; j++) {
                if (specimen[j].genome->fData[i - shift] < 100) {
                    specimen[j].genome->fData[i - shift] *= 1.3;
                }
            }
        }
    }
    return;
}

void rotateGround() {
    double y  = wy * 0.75;
    double x1 = wx * 0.1;
    double x2 = wx * 0.2;
    double r  = 50;
    if (!globalData[graphE].b) y = wy * 0.9;
    if (bound(mousePos.x, mousePos.y, x1 - r, y - r, x1 + r, y + r)) {
        environment[0] -= 0.02;
    }

    if (bound(mousePos.x, mousePos.y, x2 - r, y - r, x2 + r, y + r)) {
        environment[0] += 0.02;
    }

}

void addObject() {
    return;
}

void normalContinue() {
    int x = wx * 0.35;
    int y = wy * 0.7;
    int r = 20;
    if (bound(mousePos.x, mousePos.y, x-r, y-r, x+r, y+r)) {
        globalData[simPositionE].i = 0;
    }
    return;
}

void timeGenScreen() {
    int x = wx * 0.65;
    int y = wy * 0.7;
    int r = 20;
    if (bound(mousePos.x, mousePos.y, x-r, y-r, x+r, y+r)) {
        globalData[timeGenScreenE].b ^= true;
    }
    return;
}

void toggleSkin() {
    double x = wx * 0.9;
    double y = wy * 0.1;
    double r = 50;
    if (bound(mousePos.x, mousePos.y, x - r, y - r, x + r, y + r)) {
        globalData[displaySkinE].b ^= true;
    }
    return;
}

void toggleGraph() {
    double x = wx * 0.3;
    double y = wy * 0.75;
    double r = 50;
    if (!globalData[graphE].b) y = wy * 0.9;
    if (bound(mousePos.x, mousePos.y, x - r, y - r, x + r, y + r)) {
        globalData[graphE].b ^= true;
    }
    return;
}

void skipNGens() {
    double y = wy * 0.7;
    double x = wx * 0.5;
    double r = 20;
    if (bound(mousePos.x, mousePos.y, x-r, y-r, x+r, y+r)) {
        globalData[quickGenE].i = 3;
        globalData[simPositionE].i = 0;
        glutSetCursor(GLUT_CURSOR_WAIT);
    }
    return;
}

void selectionType() {
    double last1 = writeParagraph("Now, you must decide how to pick the next generation\n"
           "How would you like to mutate them? Can you evolve a fast creature with 6 nodes?"
           "", wx / 2, wy * 0.13, 24);
    last1 *= 1.2;
    double x = wx * 0.9;
    double y = last1 + 50 - 5 + 24;
    double r = 10;
    if (bound(mousePos.x, mousePos.y, x-r, y-r, x+r, y+r)) {
        environment[1] = 0;
    }

    y += 24;
    if (bound(mousePos.x, mousePos.y, x-r, y-r, x+r, y+r)) {
        environment[1] = 1;
    }

    y+=24;
    if (bound(mousePos.x, mousePos.y, x-r, y-r, x+r, y+r)) {
        environment[1] = 2;
    }
    return;
}


