#include "GameModes/mice.h"    // Various keyboards

#include "global.h"                 // Globals
#include "Glut/MyGlut.h"                 // Globals
#include "GameModes/inputFunctions.h" // Various keyboard functions
#include "Generation/mutations.h"   // Mutation
#include "ErrorHandle/error.h"      // quit
#include "GameModes/MainMenu/menuInputFuncts.h"
#include "GameModes/Simulate/simInputFuncts.h"



bool bound(int x, int y, int x1, int y1, int x2, int y2) {
    if ((x >= x1) && (x <= x2)) {
        if ((y >= y1) && (y <= y2)) {
            return true;
        }
    }
    return false;
}

static void none(int press, int state, int x, int y) {if (press||state||x||y||true) return;}
static void menuMouseClick(int button, int state, int mx, int my);
static void simMouseClick(int button, int state, int mx, int my);

/** To Add a Mouse: Create press and hold functions, add to getKeyboardFunct **/
/** To Add a Button: Display its icon, add check for in bound(mx, my,...) and call function in GameMode Folder **/
void (*getMouseFunct(bool hold))(int, int, int, int) {
    switch (gameMode) {
        case startMode: return (hold)? menuMouseClick:none;
        case simMode  : return (hold)? simMouseClick:none;
        default       : return (hold)? none:none;
    }
}

#include "Visual/basic.h"
static void simMouseClick(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
            if (globalData[simPositionE].i == 0) { // Simulation Screen
                nextIntroMessage();
                toggleSkin();
                toggleGraph();
                rotateGround();
                addObject();
            }
            if (globalData[simPositionE].i == 1) { // Finished Gen Screen
                mutateChange();
                normalContinue();
                timeGenScreen();
                skipNGens();
                selectionType();
            }

		}
    }
    if (mx||my||true) return;
}

static void menuMouseClick(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
            menuOptions();
		}
	}
	if (mx||my||true) return;
}










//			isDragging = 1; // start dragging
//			xDragStart = mx; // save x where button first pressed
//			angleX += deltaAngleX; // update camera turning angle
//			isDragging = 0; // no longer dragging




