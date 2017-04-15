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

#include <stdio.h>
void drawCreature(creature individual, bool skin) {
    int * sizes = individual.genome->iData;
    stickball * components = ((stickball*) individual.components);
    posi com = getCom(individual);

    /* Nodes */
    glColor3f(BLACK);
    for (int i = 0; i < sizes[nod]; i++) {
        posi loc = components->nodes[i].loc;
        drawSphere(loc.x, loc.y, loc.z, RADIUS);
    }

    /* Skin */
    if (skin) {
        glColor4f(BLACK, 1); // Used to be pink but it was too 'weird'
        for (int i = 0; i < sizes[nod]; i++) {
            posi locA = components->nodes[i].loc;
            for (int j = i + 1; j < sizes[nod]; j++) {
                posi locB = components->nodes[j].loc;
                for (int k = j + 1; k < sizes[nod];k++) {
                    posi locC = components->nodes[k].loc;
                    glBegin(GL_TRIANGLE_FAN);
                        glVertex3f(locA.x, locA.y, locA.z);
                        glVertex3f(locB.x, locB.y, locB.z);
                        glVertex3f(locC.x, locC.y, locC.z);
                    glEnd();
                }
            }
        }
    }

    double thickness = RADIUS * 0.5;
    glColor3f(WHITE);
    for (int i = 0; i < sizes[bon]; i++) {
        int a = components->bones[i].a;
        int b = components->bones[i].b;
        if (a == b) quit(GENOME_ERROR); // Do I need this check?
        //printf("%d, %d\n", a, b);
        posi locA = components->nodes[a].loc;
        posi locB = components->nodes[b].loc;
        drawCylinder(locA.x, locA.y, locA.z,
                      locB.x, locB.y - 0.1, locB.z,
                        thickness ,     5);
    }

    /* Muscle */
    glColor3f(RED);
    for (int i = 0; i < sizes[mus]; i++) {
        int a = components->muscles[i].a;
        int b = components->muscles[i].b;
        if (a == b) quit(GENOME_ERROR); // Do I need this check?

        posi locA = components->nodes[a].loc;
        posi locB = components->nodes[b].loc;
        float expansivity = components->muscles[i].origLength / euc(locA, locB);
        expansivity -= 1;
        expansivity *= 5;

        drawCylinder(locA.x, locA.y, locA.z,
                      locB.x, locB.y + 0.1, locB.z,
                       thickness * (1 + expansivity),     5);
    }

    /* COMs / Origins */
    glColor3f(WHITE);
    drawSphere(com.x, com.y, com.z, 0.1); // COM
    drawSphere(com.x, com.y,     0, 0.1); // Ground COM
    drawSphere(components->origin.x, components->origin.y, components->origin.z, 0.1); // Origin
    glColor3f(RED);
    drawLine(com.x, com.y, 0.1, // Distance
             components->origin.x, components->origin.y, 0.1);
    return;
}

void drawGround() {
    /* Draw Axis */
    glColor3f(BLACK); // Z axis
	drawCylinder(0, 0, -10,
                 0, 0,  10,
                 0.3, 10);
    glColor3f(BLUE); // X Axis
	drawCylinder(10, 0, 0,
                 -10, 0, 0,
                 0.3, 10);
    glColor3f(RED); // Y Axi
	drawCylinder(0,  10, 0,
                 0, -10, 0,
                 0.3, 10);


    /* Draw Cage */
    glColor3f(BLACK);
    drawCylinder(CAGESIZE, -CAGESIZE, 0.3,
                 CAGESIZE,  CAGESIZE, 0.3,
                 0.3, 10);
    drawCylinder(-CAGESIZE, -CAGESIZE, 0.3,
                 -CAGESIZE, CAGESIZE, 0.3,
                 0.3, 10);
    drawCylinder(-CAGESIZE, CAGESIZE, 0.3,
                 CAGESIZE,  CAGESIZE, 0.3,
                 0.3, 10);
    drawCylinder(-CAGESIZE, -CAGESIZE, 0.3,
                 CAGESIZE, -CAGESIZE, 0.3,
                 0.3, 10);

	/* Draw Ground */
	glColor3f(GRASS);
	double l = 500.0;
	double O = environment[0]; // About Y axis

    glColor3f(GRASS);
	glBegin(GL_QUADS);
		glVertex3f(-l * cos(O), -l,  l * sin(O));
		glVertex3f(-l * cos(O),  l,  l * sin(O));
		glVertex3f( l * cos(O),  l, -l * sin(O));
		glVertex3f( l * cos(O), -l, -l * sin(O));
	glEnd();


    // Grid
    int sep = 3;
    int len, corner;

    int fieldSize = 40;
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

    // Rocks
    double greyness = 0.5;
    double height1 = -1.5;
    double bigness = 3;
    glColor3f(greyness, greyness, greyness);
    drawSphere(-14, 14, height1, bigness, 6, 6);

    // Trees
    #define numTrees 9
    for (int i = 0; i < 123; i++) rand();

    static double random[numTrees * 4] = {0};
    if (fabs(random[0] - 0) < 1) {
        for (int i = 0; i < numTrees * 2; i++) {
            random[i] = randf(2 * len) - len; // size
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

