#include "Visual/basic.h"
#include "Glut/myGlut.h"   // Glut functions and definitions
#include "global.h"
#include <math.h>
#include "Math/myMath.h"
#include "ErrorHandle/error.h"

void drawTree(double x, double y, double height, double scale) {
    double ground = -tanf(environment[0]) * x;
    height *= scale;
    glColor3f(BROWN);
    drawCylinder(x, y, ground,
                 x, y, scale * height + ground,
                 scale * 0.5, 20);
    glColor3f(GREEN);
    drawSphere(x, y, scale * height - 0.01 + ground,
               scale * 2, 4,4);
}

void drawSight(double dist) {
    glColor3f(RED);
    drawLine(cameraPos.x            , cameraPos.y            , 0.2,
             cameraPos.x + dist * cameraDir.x, cameraPos.y + dist * cameraDir.y, 0.2);
    glColor3f(0.0, 1.0, 0.0);
    drawLine(cameraPos.x + dist*cameraDir.x, cameraPos.y + dist*cameraDir.y, 0.2,
             cameraPos.x + dist*cameraDir.x, cameraPos.y + dist*cameraDir.y, cameraPos.z + dist*cameraDir.z);
    glColor3f(0.5, 0.5, 0);
    drawSphere(cameraPos.x + dist*cameraDir.x, cameraPos.y + dist*cameraDir.y, cameraPos.z + dist*cameraDir.z, 0.3);
    return;
}

void drawAxes(double x, double y, double z) {
    const int quality = 4;
    const double r = 0.3;
    /* Draw Axis */

    glColor3f(BLUE); // X Axis
	drawCylinder(x, 0, 0,
                 -x, 0, 0,
                 r, quality);
    glColor3f(RED); // Y Axi
	drawCylinder(0,  y, 0,
                 0, -y, 0,
                 r, quality);
    glColor3f(BLACK); // Z axis
	drawCylinder(0, 0, -z,
                 0, 0,  z,
                 r, quality);

}

void drawCage(double length) {
    const int quality = 4;
    const double r = 0.3;

    glColor3f(BLACK);
    drawCylinder(length, -length, 0.3,
                 length,  length, 0.3,
                 r, quality);
    drawCylinder(-length, -length, 0.3,
                 -length, length, 0.3,
                 r, quality);
    drawCylinder(-length, length, 0.3,
                 length,  length, 0.3,
                 r, quality);
    drawCylinder(-length, -length, 0.3,
                 length, -length, 0.3,
                 r, quality);
}

void drawGrass(double l, double O) {
	glColor3f(GRASS);

	glBegin(GL_QUADS);
		glVertex3f(-l * cos(O), -l,  l * sin(O));
		glVertex3f(-l * cos(O),  l,  l * sin(O));
		glVertex3f( l * cos(O),  l, -l * sin(O));
		glVertex3f( l * cos(O), -l, -l * sin(O));
	glEnd();

}

void drawGrid(int fieldSize, double O) {
    int sep = 3;
    int len, corner;
    fieldSize /= sep;


    // Round to near multiple of sep (eg: 10 -> 12)
    len = corner = (fieldSize % sep) > (sep / 2) ? fieldSize - (fieldSize % sep) + sep : fieldSize - (fieldSize % sep);
    len *= sep;

    glColor3f(0.4, 0.4, 0.4);
    for (int i = -corner; i <= corner; i+= sep) {
        int temp = i * sep;
        /* Grid */
        drawCylinder(-temp * cosf(O), -len,  temp * sinf(O),
                     -temp * cosf(O),  len,  temp * sinf(O),
                     0.06, 10);
        drawCylinder(-len,  -temp * cosf(O),  len* tanf(O),
                      len,  -temp * cosf(O), -len* tanf(O),
                       0.06, 10);
    }

}
void drawGround(int groundSize, double horAxis, double zAxis) {
    double O = environment[0]; // About Y axis

    drawAxes(horAxis, horAxis, zAxis);
    drawCage(horAxis);
    drawGrass(groundSize, O);
    drawGrid(groundSize, O);

    // Rocks
    double greyness = 0.5;
    double height1 = -1.5;
    double bigness = 3;
    glColor3f(greyness, greyness, greyness);
    drawSphere(-14, 14, height1, bigness, 6, 6);

    // Trees
    #define numTrees 9
    //for (int i = 0; i < 123; i++) rand();

    static double random[numTrees * 4] = {0};
    if (fabs(random[0] - 0) < 1) {
        for (int i = 0; i < numTrees * 2; i++) {
            random[i] = randf(2 * groundSize) - groundSize; // size
        }
        for (int i = numTrees * 2; i < numTrees * 4; i++) {
            random[i] = randf(2) + 1;
        }
    }

    double x[numTrees];
    double y[numTrees];
    double h[numTrees];
    double s[numTrees];

    for (int i = 0; i < numTrees; i++) {
        x[i] = random[i + 0 * numTrees];
        y[i] = random[i + 1 * numTrees];
        h[i] = random[i + 2 * numTrees];
        s[i] = random[i + 3 * numTrees];
    }
    for (int i = 0; i < numTrees; i++) {
        drawTree(x[i], y[i], h[i], s[i]);
    }
    return;
}

void drawSun() {
    glDisable(GL_LIGHTING);
    glColor4f(0.8,0.8,0, 0.5);
    drawSphere(300, 300, 300, 30);
    glEnable(GL_LIGHTING);
    return;
}

