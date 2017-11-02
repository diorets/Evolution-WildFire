#include "Functional/graph.h"
#include "global.h"
#include "Glut/myGlut.h"
#include "Visual/basic.h"
#include <stdio.h>

void drawGraph(graph g, double xMin, double xMax) {
    if (!g.display) return;

    double yMin = wy-50;
    double yMax = yMin - 0.2*wy;
    double indent = 100;

    /* Draw Background */
    glColor3f(BLACK);
    drawSquare(xMin, yMin, xMax, yMax);
    glColor3f(WHITE);
    drawLine(xMin + indent, yMin, xMin + indent, yMax);

    /* Determine Graphability of Data */
    int numPoints = g.numEntries;
    if ((g.points == NULL) || (numPoints < 2)) {
        drawText("Not Enough Points", xMin + (xMax-xMin) / 2.0, yMin+0.5*(yMax-yMin), true, false);
        return;
    }

    /* Get Peak and Trough for Normalizing Graph */
    double peak   = -100000;
    double trough =  100000;
    for (int i = 0; i < numPoints; i++) {
        if (g.points[i] > peak) {
            peak = g.points[i];
        }
        if (g.points[i] < trough) {
            trough = g.points[i];
        }
    }

    /* Write Extreme Points on Axis */
    char str[10];
    sprintf(str, "%.2f", peak);
    drawText(str, xMin + indent / 2.0, yMax+15, true, false);

    sprintf(str, "%.2f", trough);
    drawText(str, xMin + indent / 2.0, yMin-5, true, false);

    /* Graphing Line Plot */
    for (int i = 0; i < numPoints - 1; i++) {
        double * ys = g.points;
        double xScale = (-indent+xMax-xMin) / ((double) numPoints - 1.0);
        double yScale = (yMax-yMin) / (peak-trough);

        double x1 = (i+0) * xScale + indent + xMin;
        double x2 = (i+1) * xScale + indent + xMin;
        double y1 = (ys[i+0]-trough) * yScale + yMin;
        double y2 = (ys[i+1]-trough) * yScale + yMin;
        drawLine(x1, y1, x2, y2);
    }
}
