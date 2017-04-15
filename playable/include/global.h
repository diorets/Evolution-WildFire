#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include "preproccessor.h"

// Settings
extern int dataCollection;
extern int data[];
extern bool fullscreen;
extern bool person;
extern bool lights;
extern bool display;
extern int  gameMode;
extern int  wx;
extern int  wy;

// Camera position
extern posi cameraPos;
extern posi cameraMov;
extern posi cameraDir;
extern posi cameraAng;
extern posi cameraDel;

// Speeds
extern double playerSpeed;
extern double cameraSpeed;
extern int playBackSpeed;
extern int processSlowDown;

// Mouse
extern posi mousePos;
extern posi DragStart;
extern bool isDragging;

// Key Hold
extern bool* keyStates;

// Simulator Variables
extern double * environment;

extern generic * globalData;

#endif // GLOBAL_H_INCLUDED
