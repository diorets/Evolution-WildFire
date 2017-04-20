#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

#include "Visual/text.h"
#include "preproccessor.h"

#define ULTRA_QUAL 20, 20
#define HIGH_QUAL 100, 100
#define MED_QUAL   20, 20
#define LOW_QUAL    5, 5


void reenable3D();
void enable2D();

void drawCube(double x, double y, double z, double radius, double theta);
void drawSphere(double x, double y, double z, double radius);
void drawSphere(double x, double y, double z, double radius, int qualityA, int qualityB);
void drawLine(double x, double y, double z, double X, double Y, double Z);
void drawCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions);
void glutDrawing();

void drawHollowSquare(double x, double y, double X, double Y);
void drawSquare(double x, double y, double X, double Y, double r, double g, double b,
                 double r1, double g1, double b1, double alpha);

void drawSquare(double x, double y, double X, double Y);
void drawLine(double x, double y, double X, double Y);
void drawPoint(double x, double y);
void drawCircle(double xC, double yC, double r, int num_segments);

#endif // DRAW_H_INCLUDED

/* Possible Text Sizes
GLUT_BITMAP_8_BY_13
GLUT_BITMAP_9_BY_15
GLUT_BITMAP_TIMES_ROMAN_10
GLUT_BITMAP_TIMES_ROMAN_24
GLUT_BITMAP_HELVETICA_10
GLUT_BITMAP_HELVETICA_12
GLUT_BITMAP_HELVETICA_18
*/
