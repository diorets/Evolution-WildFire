/*****************************************************************
Evolution WildFire, A Unique 3D Evolution Simulator.

Nawar Ismail
July 2016
******************************************************************/
#include <stdlib.h> // standard definitions
#include <stdio.h>  // standard I/O
#include "Glut/initialize.h"       // Initialize
#include "Glut/myGlut.h"           // GlutMainLoop
#include "preproccessor.h"         // Defintions
#include "global.h"                // Globals
#include "Math/myMath.h"           // vec, zero

/** Global State Variables **/
// Settings
int dataCollection = 5; // none, parameterTest, impactTest, initial population, multiWorld
int data[] = {5,3,3,0, 10, 500}; // GENSIZE MYST Be 500
// initPopNodes, initPopMuscles, initPopBones, initPopNeurons, srand, genSize

bool person     = false;
bool lights     = true;
bool display    = true;
bool fullscreen = true;
int  gameMode   = startMode; //startMode
//int  genSize    = data[5];

// window size (obtained updated during runtime)
int  wx         = 0;
int  wy         = 0;

// Speeds
int    playBackSpeed = 1; // 1
double playerSpeed   = 1.2;// / playBackSpeed; // Should scale with overhead
double cameraSpeed   = 0.04;// / playBackSpeed;
int processSlowDown = 0;

// Camera position
posi cameraPos = vec(0.0, -40.0, 1.8);
posi cameraMov = vec(0.0,   0.0, 1.8);
posi cameraDir = vec(0.0,   1.0, 0.0);
posi cameraAng = zero();
posi cameraDel = zero();

// Mouse
posi mousePos   = zero();
posi xDragStart = zero();
bool isDragging = false;

// Key Hold
bool* keyStates = new bool[256];

// Variables
double * environment = NULL;
generic  * globalData = NULL;
button * buttons = NULL;


void graphTest();

int main(int argc, char **argv) {
    fclose(fopen("../assets/fitnesses.txt", "w")); // Clear File

    environment = (double*) malloc(sizeof(double) * 2);
    if (environment == NULL) exit(34523);
    environment[0] = 0.0;
    environment[1] = 0;

    initiateSimulator(&argc, argv, "Evolution WildFire v6.0");
    //initiatePopulation(6, 10, 5, 0, false);
	glutMainLoop();
	return 0;
}




#include "Math/distribution.h"
double testFunction(double x) {
    return x < 0.5 ? 1 : 0;
}
void graphTest() {
    for (int i = 0; i < 100; i++) {
        printf("%d\n", getDistribution(testFunction, 100, 0));
    }
    return;
}
























































