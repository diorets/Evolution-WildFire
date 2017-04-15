#include "Glut/initialize.h"

#include <time.h>                  // srand(time(NULL))
#include "preproccessor.h"         // Definitions
#include "Glut/basic.h"            // Various Glut Functions
#include "Glut/myGlut.h"           // Glut functions and definitions
#include "global.h"                // Globals
#include "Generation/generation.h" // createGenome, createCreature
#include "Generation/genes.h"      // PrintGenome
#include "GameModes/gameModes.h"   // renderScene
#include "ErrorHandle/error.h"     // Quit

void initiateSimulator(int* argc, char **argv, const char * title) {
    for (int i = 0; i < 256; i++) {
        keyStates[i] = false;
    }
    srand(time(NULL));
    glutInit(argc, argv);

    /* General Initializations */
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(WINDOW_POSITITION);
	glutInitWindowSize(WINDOW_SIZE);
	glutCreateWindow(title);
	if (fullscreen) {
        glutFullScreen();
	}


    /* Lighting */
    GLfloat mat_specular[]   = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[]  = {50.0};
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_SMOOTH);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    /* Blending */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Register Callbacks */
	glutReshapeFunc(changeSize);  // window reshape callback
	glutDisplayFunc(renderScene); // (re)display callback
	glutIdleFunc(update);         // incremental update
	glutIgnoreKeyRepeat(true);       // ignore key repeat when holding key down
	glutMouseFunc(callMouse);   // process mouse button push/release
	glutMotionFunc(mouseMove);    // process mouse dragging motion
    glutPassiveMotionFunc(passiveMouse);
	//glutKeyboardFunc(processNormalKeys); // process standard key clicks
	glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses
    glutKeyboardUpFunc(keyUp);    // Tell GLUT to use the method "keyUp" for key up events


	glutSpecialFunc(pressSpecialKey);     // process special key pressed
						// Warning: Nonstandard function! Delete if desired.
	glutSpecialUpFunc(releaseSpecialKey); // process special key release

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

    return;
}
