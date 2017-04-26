#include "Glut/basic.h"

#include <math.h>                  // Trig for camera
#include "global.h"                // Globals
#include "preproccessor.h"         // Definition
#include "Glut/myGlut.h"           // Glut functions and definitions
#include "GameModes/keyboards.h"   // Glut functions and definitions
#include "GameModes/mice.h"

#define ESC 27
#define SPACE 32
#define PI 3.141592653589793

// Grab the keyboard, and call it
void callKeyboard(unsigned char key, bool hold) {
    void (*keyboardFunc)(unsigned char) = getKeyboardFunct(hold);
    (keyboardFunc)(key);
    return;
}

void passiveMouse(int mx, int my) {
    mousePos.x = mx;
    mousePos.y = my;
    return;
}


#include <stdio.h>
#include "Functional/buttons.h"
void callMouse(int buttonPressed, int state, int mx, int my) {
    if (buttonPressed != GLUT_LEFT_BUTTON) return;
//    void (*mouseFunc)(int, int, int, int) = getMouseFunct(true);
//    (mouseFunc)(buttonPressed, state, mx, my);

    if (state == GLUT_DOWN) {
        for (button * b = buttons; b != NULL; b = b->next) {
            b->highlighted = hoveringOver(b);
            if (b->highlighted) {
                b->callbackFunction();
                b->clicked = true;
                if (b->togglable) {
                    b->toggled ^= true;
                }
                b->countDown = HIGHLIGHT_DURATION;
            }
        }
    } else {
        for (button * b = buttons; b != NULL; b = b->next) {
            b->clicked = false;
            b->highlighted = hoveringOver(b);
        }
    }
    return;
}

#include <stdio.h>
void changeSize(int w, int h) {
    int drawDistance = 1000.0;
	double ratio =  ((double) w) / ((double) h); // window aspect ratio
	glMatrixMode(GL_PROJECTION); // projection matrix is active
	glLoadIdentity(); // reset the projection
	gluPerspective(45.0, ratio, 0.1, drawDistance); // perspective transformation
	glMatrixMode(GL_MODELVIEW); // return to modelview mode
	glViewport(0, 0, w, h); // set viewport (drawing area) to entire window
	return;
}

#include "GameModes/Simulate/StickBall/stickBallDrawing.h"

void update(void) {
    glutPostRedisplay(); // redisplay everything
    wx = glutGet(GLUT_WINDOW_WIDTH);
    wy = glutGet(GLUT_WINDOW_HEIGHT);

    for (button * b = buttons; b != NULL; b = b->next) {
        b->drawing(b);
    }

    double zero = 1e-10;
    if (!display) return;

    /* Update Position */// Note: Not in orthogonal directions
    // Move 'Forward / Backwards'
	if (cameraMov.x > zero || cameraMov.x < -zero) {
		cameraPos.x += cameraMov.x * cameraDir.x;
		cameraPos.y += cameraMov.x * cameraDir.y;
		//if (cameraPos.z + cameraMov.x * cameraDir.z > 0) { // prevent going under z=0
            cameraPos.z += cameraMov.x * cameraDir.z;
		//}
	}
	// Move 'Left / Right'
	if (cameraMov.y > zero || cameraMov.y < -zero) { // Cross product with Up vector
        cameraPos.x += cameraMov.y * cameraDir.y;
		cameraPos.y -= cameraMov.y * cameraDir.x;
	}
	// Move 'Up / Down'
	if (cameraMov.z > zero || cameraMov.z < -zero) { // Simply move Up
        cameraPos.z += cameraMov.z;
	}

    /* Update Viewing Angle */
	if (cameraDel.x > zero || cameraDel.x < -zero) // Should rename to phi and theta
    {
        cameraAng.x += cameraDel.x;
        cameraDir.x = -sin(cameraAng.x);
        cameraDir.y = cos(cameraAng.x);

    }
    if (cameraDel.z > zero || cameraDel.z < -zero)
    {
        // Restrict angle to PI / 2
        if (cameraAng.z + cameraDel.z < PI / 2 && cameraDel.z > 0)
            cameraAng.z += cameraDel.z;
        if (cameraAng.z - cameraDel.z > -PI / 2 && (cameraDel.z < 0))
            cameraAng.z += cameraDel.z;

        cameraDir.z = sin(cameraAng.z);
    }

	// Reset speed
	cameraMov.x = 0.0;
	cameraMov.y = 0.0;
	cameraMov.z = 0.0;
}

/* Keyboard Presses */
#include <ctype.h>
void keyPressed (unsigned char key, int x, int y) {
    if (key & x * y){}
    keyStates[tolower(key)] = true; // Set the state of the current key to pressed for holding
    callKeyboard(key, false);
}

void keyUp (unsigned char key, int x, int y) {
    if (key & x * y){}
    keyStates[tolower(key)] = false; // Set the state of the current key to not pressed for holding
}

void processNormalKeys(unsigned char key, int xx, int yy) {
    if (key & xx * yy){}
}

void pressSpecialKey(int key, int kxx, int kyy) {
    if (key & kxx * kyy){}

    switch (key) {
		case GLUT_KEY_LEFT  : cameraDel.x =  cameraSpeed; break;
		case GLUT_KEY_RIGHT : cameraDel.x = -cameraSpeed; break;
		case GLUT_KEY_UP    : cameraDel.z =  cameraSpeed; break;
		case GLUT_KEY_DOWN  : cameraDel.z = -cameraSpeed; break;
//		case GLUT_KEY_PAGE_UP    : editorLength += 0.01; break;
//		case GLUT_KEY_PAGE_DOWN  : editorLength -= 0.01; break;
		default : return;
	}
}

void releaseSpecialKey(int key, int kx, int ky) {
    if (key & kx * ky){}
    switch (key) {
		case GLUT_KEY_LEFT  : cameraDel.x = 0.0; break;
		case GLUT_KEY_RIGHT : cameraDel.x = 0.0; break;
		case GLUT_KEY_UP    : cameraDel.z = 0.0; break;
		case GLUT_KEY_DOWN  : cameraDel.z = 0.0; break;
//        case GLUT_KEY_PAGE_UP    : editorLength = 0.0; break;
//		case GLUT_KEY_PAGE_DOWN  : editorLength = 0.0; break;
		default : return;

	}

}


void mouseMove(int mx, int my) {
    if (my||mx||true) return; // remove warning
//	if (isDragging) { // only when dragging
//		// update the change in angle
//		cameraDelX = (mx - xDragStart) * 0.005;
//
//		// camera's direction is set to angle + cameraDel
//		lx = -sin(angleX + cameraDelX);
//		ly = cos(angleX + cameraDelX);
//	}
}

