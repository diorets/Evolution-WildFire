#ifndef MYGLUT_H_INCLUDED
#define MYGLUT_H_INCLUDED

// include files are in a slightly different location for MacOS
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PURPLE 0.5, 0.0, 0.5
#define BLACK 0,0,0
#define GREY 0.5,0.5,0.5
#define WHITE 1,1,1
#define RED   1,0,0
#define GREEN  0,1,0
#define BLUE 0,0,1
#define YELLOW 1,1,0
#define ORANGE 1,0.3,0
#define SKY 0.00, 0.75, 1.0, 1.0
#define GRASS 0.0, 0.7, 0.0
#define BROWN 0.60,0.25,0.07
#define SKIN 1.0,0.71, 0.76

#endif // MYGLUT_H_INCLUDED
