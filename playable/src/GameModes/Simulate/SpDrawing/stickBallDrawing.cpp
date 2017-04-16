#include "GameModes/Simulate/SpDrawing/stickBallDrawing.h"

#include "Glut/myGlut.h"           // Glut
#include "Visual/basic.h"          // 2D, 3D, Various Shapes & Text
#include "Visual/objects.h"        // Drawing Objects
#include "GameModes/Simulate/drawing.h"        // Drawing Objects
#include "Math/myMath.h"
#include "ErrorHandle/error.h"
#include "global.h"
#include <math.h>
#include <stdio.h>
#include "GameModes/Simulate/General/drawing.h"

void drawStickBall(creature individual);

void stickBallSystem(creature input, int genSize, int gen, int id, int simTime, int maxTime) {
    /* Clear, Reset, Camera */
    glutDrawing();

    /* 3D Drawing */
    reenable3D();
    drawGround();
    drawSun();
    drawStickBall(input);

    /* 2D drawing */
    enable2D();
    drawDetails(genSize, gen, id, simTime, maxTime);
    //drawDisGraph(false, false);
    glutSwapBuffers(); // Make it all visible
}

void drawStickBall(creature individual) {
    int * sizes = individual.genome->iData;
    stickball * components = ((stickball*) individual.components);
    posi com = getCom(individual);

    /* Nodes */
    glColor3f(BLACK);
    for (int i = 0; i < sizes[nod]; i++) {
        posi loc = components->nodes[i].loc;
        drawSphere(loc.x, loc.y, loc.z, RADIUS);
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


static void traceLines(double * entries, double maxEntry, int numEntriesI, double r, double g, double b){
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    double numEntries = (double) numEntriesI;
    for (int i = 1; i < numEntries; i++) {
        double x = (i + -1) / (double) (numEntries);
        double X = (i + 0) / (double) (numEntries);
        double y = 5 * (entries[i + 0]);
        double Y = 5 * (entries[i + 1]);
        glVertex2f(wx * x, 0.1 * wy * (-0.2*y/maxEntry + 9));
        glVertex2f(wx * X, 0.1 * wy * (-0.2*Y/maxEntry + 9));
    }
    glEnd();
    return;
}

void drawDisGraph(bool reset, bool adding) {
    /* Initialize */
    static int num = 0;
    static double *dis = NULL;
    static double *vel = NULL;
    static double *acc = NULL;
    static double maxDis = 0.0;
    static double maxVel = 0.0;
    static double maxAcc = 0.0;

    /* Initial Memory Allocation */
    if (!dis) dis = (double*) malloc(sizeof(double));
    if (!vel) vel = (double*) malloc(sizeof(double));
    if (!acc) acc = (double*) malloc(sizeof(double));
    if (!dis||!vel||!acc) quit(MALLOC_ERROR);

    if (reset) {
        if (dis != NULL) free(dis);
        if (vel != NULL) free(vel);
        if (acc != NULL) free(acc);
        dis = (double*) malloc(sizeof(double));
        vel = (double*) malloc(sizeof(double));
        acc = (double*) malloc(sizeof(double));
        if (!dis||!vel||!acc) quit(MALLOC_ERROR);
        maxDis = 0.0;
        maxVel = 0.0;
        maxAcc = 0.0;
        num = 0;
        return;
    }

    if (adding) {
        num++;
        /* Reallocating for New Data */
        dis = (double*) realloc(dis, sizeof(double) * (num + 1));
        vel = (double*) realloc(vel, sizeof(double) * (num + 1));
        acc = (double*) realloc(acc, sizeof(double) * (num + 1));

        if (!dis||!vel||!acc) quit(MALLOC_ERROR);


        /* Get Averages per Node */
//        dis[num] = euc2D(       getCom(specimen[id]), specimen[id].origin);
//        vel[num] = euc2D(getAvgNodeVel(specimen[id]), specimen[id].origin);
//        acc[num] = euc2D(getAvgNodeVel(specimen[id]), specimen[id].origin); // Fix ME

        /* Get Maximums (for scaling) */
        if (fabs(dis[num]) > maxDis) maxDis = fabs(dis[num]);
        if (fabs(vel[num]) > maxVel) maxVel = fabs(vel[num]);
        if (fabs(acc[num]) > maxAcc) maxAcc = fabs(acc[num]);
    } else {
        /** Drawing **/
        if (globalData[graphE].b) {
            /* Draw Background */
            glColor3f(BLACK);
            drawSquare(wx, wy, 0, wy * 0.8);
            glColor3f(WHITE);
            drawText("The sudden jump in the graph is the Center Of Mass (COM) being reset after they have time to fall and get stable",
                      0, wy * 0.95, false);

            /* Zero Line */
            glBegin(GL_LINES);
                glVertex2f(0,        0.9 * wy);
                glVertex2f(wx, 0.9 * wy);
            glEnd();

            /* Draw Data */
            traceLines(dis, maxDis, num, WHITE);
    //        traceLines(vel, maxVel, num, GREEN);
        //    traceLines(acc, maxAcc, num, BLUE);

            /* Measures */
            glColor3f(WHITE);
            char str[20];
            int numDiv = 8;
            for (int i = 0; i <= numDiv; i++) {
//                sprintf(str, "%d(%d%%)", simTime * i / numDiv, 100 *  i / numDiv);
                drawText(str, 0.95 * wx * i / numDiv, wy * 0.92, false);
            }
            sprintf(str, "Max Dis: %0.2f-", maxDis);
            drawText(str, 0.5, wy * 0.815, false);
        }
    }
    return;
}



void genDisGraph(double avg, bool adding) {
    int gen = 500; // TEMPORARY
    /* Initialize */
    static double *dis = NULL;
    static double maxDis = 0.0;
    if (dis == NULL) {
        dis  = (double*) malloc(sizeof(double));
    } else if (adding) {
        dis  = (double*) realloc(dis, sizeof(double) * (gen + 1));
    }
    if (dis == NULL) quit(MALLOC_ERROR);

    /* Get Max */
    if (adding) {
        dis[gen] = avg;
        if (fabs(dis[gen]) > maxDis) {
            maxDis = fabs(dis[gen]);
        }
    } else {

        /* Draw Background */
        glColor3f(BLACK);
        drawSquare(0, wy * 0.79, wx, wy);

        /* Draw Lines */
        char str[20];
        glColor3f(WHITE);
        glBegin(GL_LINES);
        glVertex2f(0, 0.9 * wy);
        glVertex2f(wx, 0.9 * wy);

        for (int i = 0; i < gen; i++) {
            if (gen == 1) {
                double x = 0;
                double X = 1;
                double y = 5 * dis[0];
                double Y = 5 * dis[0];
                glVertex2f(wx * x, 0.1 * wy * (-0.2*y/maxDis + 9));
                glVertex2f(wx * X, 0.1 * wy * (-0.2*Y/maxDis + 9));
            } else {
                double x = (i + 0) / (double) (gen - 1);
                double X = (i + 1) / (double) (gen - 1);
                double y = 5 * dis[i + 0];
                double Y = 5 * dis[i + 1];
                glVertex2f(wx * x, 0.1 * wy * (-0.2*y/maxDis + 9));
                glVertex2f(wx * X, 0.1 * wy * (-0.2*Y/maxDis + 9));
            }
        }
        glEnd();

        /* Measures */
        int numDiv = 8;
        for (int i = 0; i <= numDiv; i++) {
            sprintf(str, "%d(%d%%)", gen * i / numDiv, 100 *  i / numDiv);
            drawText(str, 0.95 * wx * i / numDiv, wy * 0.92, false);
        }
        glColor3f(BLACK);
        drawText("Distance vs Generation Graph", 0.5, wy * 0.78, false);
        glColor3f(WHITE);
        sprintf(str, "Max Dis: %0.2f", maxDis);
        drawText(str, 0.5, wy * 0.818, false);
    }

    return;
}
