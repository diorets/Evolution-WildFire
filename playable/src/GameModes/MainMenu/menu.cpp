#include "GameModes/MainMenu/menu.h"

#include <math.h>  // Cosf, Sinf
#include <stdio.h> // Sprintf

#include "Glut/myGlut.h"      // Glut
#include "global.h"           // Globals
#include "Visual/basic.h"     // 2D,3D, Shapes & Text
#include "Math/myMath.h"      // Chance
#include "GameModes/Simulate/StickBall/stickBallPhysics.h"          // UpdateCreature
#include "Visual/objects.h"   // Draw Creature
#include "Functional/music.h" // Music

static void startText();
void cinematicCamera(int counter);
static void initMenu();


void startUpMode() {
//    static int i = 0;
//    static int id = 0;
//    const int genSize = 1000;
//    if (i == 0) initMenu();
//    cinematicCamera(++i);
//
//    /* Simulate Creatures in Background */
//    glutDrawing();
//    drawGround();
//    drawCreature(specimen[id], true);
//    updateCreature(&specimen[id], i);
//    if (i % 250 == 0) id = (id + 1) % genSize; // Cycle through creatures
//
//    /* Display Information and Menus Text */
//    enable2D();
//    startText();
//    reenable3D();
//    glutSwapBuffers();
    return;
}

static void initMenu() {
    //playSong("../assets/AlpineRun.mp3", "menuSong");
    return;
}

void cinematicCamera(int counter) {
    double SPEED = 3.0;
    double speed = SPEED * 1 / 2000.0;
    double scale = SPEED * 0.06;
    static int arr[3] = {1, 2, 3};

    /* Change Speeds Randomly */
    if (chance(0.1)) {
        int shift = rand() % 3;
        arr[0] = arr[(shift + 0) % 3];
        arr[1] = arr[(shift + 1) % 3];
        arr[2] = arr[(shift + 2) % 3];
    }

    /* Move and Look at New Position */
    cameraPos.x += 1.0 * scale * cosf(counter * speed * arr[0]);
    cameraPos.y += 1.0 * scale * sinf(counter * speed * arr[1]);
    cameraPos.z += 0.8 * scale * sinf(counter * speed * arr[2]);

    cameraDir.x = -cameraPos.x + 5 * sinf(counter * speed);
    cameraDir.y = -cameraPos.y + 5 * sinf(counter * speed);
    cameraDir.z = -cameraPos.z + 5 * sinf(counter * speed) + 3;
    return;
}

static void startText() {
    glColor3f(RED);
    drawHeader("EVOLUTION WILDFIRE!", wx * 0.47, wy * 0.12);

    glColor3f(ORANGE);
    drawText("What can you evolve, given the hand of God?", wx * 0.6, wy * 0.17, true, false);

    glColor3f(BLACK);
//    drawText("*Creatures are 2D only for display (faster evolution)", wx * 0.87, wy * 0.05, true);


    /* Draw Menu */
    posi o = vec(wx * 0.15, wy * 0.25, 0.0);
    drawMenu("Enter Simulation Mode\n"
                    "Quit\n",

                    "( B )\n"
                    "(ESC)\n", o.x, o.y);
    glColor3f(BLACK);
    drawHeader("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ WildFire Co 2016 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", wx * 0.5, wy * 0.95);
    return;
}














