#include "GameModes/Simulate/StickBall/stickBallDrawing.h"

#include "Glut/myGlut.h"           // Glut
#include "Visual/basic.h"          // 2D, 3D, Various Shapes & Text
#include "Visual/objects.h"        // Drawing Objects
#include "Math/myMath.h"
#include "ErrorHandle/error.h"
#include "global.h"
#include <math.h>
#include <stdio.h>
#include "GameModes/Simulate/General/drawing.h"

#include "Functional/buttons.h"
#include "Functional/graph.h"
void drawStickBall(creature individual);
static void createMenu();
void displaySystemInfo(creature * pop, int genSize, int id);




double g11(posi r) {
    return 3 * sin(0.25 * r.x) * cos(0.25 * r.y);
}

double f(posi r) { // max range is +/- 0.5*h*len. ex: l=500, h=0.1 => x:25
    #define len 500
    double h = 1;
    static bool builtField = false;
    static double field[len][len];
    if (!builtField) {
        double x = 0.0;
        double y = 0.0;
        for (int i = 0; i < len; i++) {
            x = h * (i-0.5*len);
            for (int j = 0; j < len; j++) {
                y = h * (j-0.5*len);
                field[i][j] = g11(vec(x, y, 0));
            }
        }
        builtField = true;
    }
    int i_ = (int) (r.x / h + 0.5 * len);
    int j_ = (int) (r.y / h + 0.5 * len);
    return field[i_ % len][j_ % len]; // bandaid for out of range
    #undef len
}


void fline(double x, double y, double X, double Y) {
    drawLine(x, y, f(vec(x, y, 0)), X, Y, f(vec(X, Y, 0)));
    return;
}



bool inSquare(posi p, double x, double y, double X, double Y) {
    if (p.x >= x)
    if (p.x <= X)
    if (p.y >= y)
    if (p.y <= Y)
    return true;
    return false;
}

//posi * addShadows(posi * shadows, int * numShadows, posi point, double x, double y, double X, double Y, double dx, double dy) {
//    if (inSquare(point, x, y, x+dx, y+dy)) {
//        shadows = (posi*) realloc(shadows, sizeof(posi) * (++(*numShadows)));
//        shadows[(*numShadows)-1] = vec(x+dx/2, y+dx/2, 0);
//    }
//    return shadows;
//}
//
//
//
//posi * buildShadows(posi * shadows, int * numShadows, posi point, double x, double y, double X, double Y) {
//    if (inSquare(point, x, y, X, Y)) {
//        // Try it in 4 squares each a quarter of the size:
//        double dx = 0.5 * (X - x);
//        double dy = 0.5 * (Y - y);
//
//        shadows = addShadows(shadows, numShadows, point, x, y, x+dx, y+dy, dx, dy);
//        shadows = addShadows(shadows, numShadows, point, x, y+dy, x+dx, Y, dx, dy);
//        shadows = addShadows(shadows, numShadows, point, x+dx, y, X, y+dy, dx, dy);
//        shadows = addShadows(shadows, numShadows, point, x+dx, y+dy, X, Y, dx, dy);
//
//    }
//    return shadows;
//}

void drawFunction(creature s) {
    int numNodes = s.genome->iData[nod];
    int numMuscle = s.genome->iData[mus];
    int numBone = s.genome->iData[bon];
    bool shading = false;
    double h = 1;
    double range = 30;
    double shadowScale = 1;
    int numShadows = 0;
    posi * shadows = NULL;

    posi c = getCom(s);
    c = vec(round(c.x / h) * h, floor(c.y / h) * h, floor(c.z / h) * h);
//    for (int i = 0; i < numNodes; i++) {
//        posi loc = ((stickball*) (&s)->components)->nodes[i].loc;
//        posi toVec = add(loc, scale(normal1(loc), 5));
//        drawCylinder(loc.x, loc.y, f(loc),
//                    toVec.x, toVec.y, -f(loc) - toVec.z + loc.z,
//                    0.1, 10);
//    }

    if (shading) {
        posi nodeShadows[numNodes];
        for (int i = 0; i < numNodes; i++) {
            posi m = vec(100, 100, 100); //sun (moon)
            posi l_0 = ((stickball*) (&s)->components)->nodes[i].loc;
            posi l = sub(m, l_0);
            posi n = vec(0, 0, -1);

            posi p = add(m, scale(l, -dot(m, n) / dot(l, n))) ;
            nodeShadows[i] = p;
        }



        for (double x = c.x - range; x <= c.x + range; x+=shadowScale*h) {
            for (double y = c.y- range; y <= c.y + range; y+=shadowScale*h) {
                /* Node Shadows */

    //            shadows = (int*) realloc(shadows, sizeof(int) * (numNodes + numShadows));
    //            for (int i = 0; i < numNodes; i++) {
    //                if (inSquare(nodeShadows[i], x, y, x+2*h, y+2*h)) {
    //                    shadows[numShadows++] = i_;
    //                }
    //            }
    /* Divide the tile into 4 peices. Check each one and see if they are shadows. Draw shadows. */

                /* Muscle Shadows */
                for (int i = 0; i < numMuscle; i++) {
                    muscle mus = ((stickball*) (&s)->components)->muscles[i];
                    int a = mus.a;
                    int b = mus.b;
                    posi shadowVec = sub(nodeShadows[b], nodeShadows[a]);
                    double step = euc2D(nodeShadows[b], nodeShadows[a]) / h;
                    for (double t = 0; t < 1; t += 1/step) {
                        posi point = add(scale(shadowVec, t), nodeShadows[a]);
                        if (inSquare(point, x-shadowScale*0.5*h, y-shadowScale*0.5*h, x+shadowScale*0.5*h, y+shadowScale*0.5*h)) {
                            shadows = (posi*) realloc(shadows, sizeof(posi) * (++numShadows));
                            shadows[numShadows-1] = vec(x, y, 0);
                        }
                        //shadows = buildShadows(shadows, &numShadows, point,
                        //             x-shadowScale*0.5*h, y-shadowScale*0.5*h, x+shadowScale*0.5*h, y+shadowScale*0.5*h);
                    }
                }
                /* Bone Shadows */
                for (int i = 0; i < numBone; i++) {
                    bone bon = ((stickball*) (&s)->components)->bones[i];
                    int a = bon.a;
                    int b = bon.b;
                    posi shadowVec = sub(nodeShadows[b], nodeShadows[a]);
                    double step = euc2D(nodeShadows[b], nodeShadows[a]) / h;
                    for (double t = 0; t < 1; t += 1/step) {
                        posi point = add(scale(shadowVec, t), nodeShadows[a]);
                        if (inSquare(point, x-shadowScale*0.5*h, y-shadowScale*0.5*h, x+shadowScale*0.5*h, y+shadowScale*0.5*h)) {
                            shadows = (posi*) realloc(shadows, sizeof(posi) * (++numShadows));
                            shadows[numShadows-1] = vec(x, y, 0);
                        }
                        //shadows = buildShadows(shadows, &numShadows, point,
                        //             x-shadowScale*0.5*h, y-shadowScale*0.5*h, x+shadowScale*0.5*h, y+shadowScale*0.5*h);
                    }
                }
            }
        }
    }
    glColor3f(BLACK);
    glBegin(GL_LINES);
    for (double x = c.x - range; x <= c.x + range; x+=h) {
        for (double y = c.y- range; y <= c.y + range; y+=h) {
            double z_x_y   = f(vec(x  , y, 0));
            double z_xh_y  = f(vec(x+h, y, 0));
            double z_xh_yh = f(vec(x+h, y+h, 0));
            double z_x_yh  = f(vec(x  , y+h, 0));

            glVertex3f(x  , y  , z_x_y);
            glVertex3f(x+h, y  , z_xh_y);

            glVertex3f(x+h, y  , z_xh_y);
            glVertex3f(x+h, y+h, z_xh_yh);

            glVertex3f(x+h, y+h, z_xh_yh);
            glVertex3f(x  , y+h, z_x_yh);

            glVertex3f(x  , y+h, z_x_yh);
            glVertex3f(x  , y  , z_x_y);
        }
    }
    glEnd();
    glColor3f(GREEN);
    glBegin(GL_QUADS);
    for (double x = c.x - range; x <= c.x + range; x+=h) {
        for (double y = c.y- range; y <= c.y + range; y+=h) {
                if (shading) {
                    for (int i = 0; i < numShadows; i++) {
                        if (equals(shadows[i].x, x, shadowScale*h) && equals(shadows[i].y, y, shadowScale*h)) {
                            glColor3f(DARKGREEN);
                        }
                    }
                }
                glVertex3f(x, y, f(vec(x, y, 0)));
                glVertex3f(x+h, y, f(vec(x+h, y, 0)));
                glVertex3f(x+h, y+h, f(vec(x+h, y+h, 0)));
                glVertex3f(x, y+h, f(vec(x, y+h, 0)));
        }
    }
    glEnd();
    return;
}


void stickBallSystem(creature * pop, int genSize, int gen, int id, int simTime, int maxTime) {
    creature input = pop[id];

    /* Clear, Reset, Camera */
    glutDrawing();

    /* 3D Drawing */
    reenable3D();
    //drawGround(500, 10, 10);
    drawAxes(10, 10, 10);
    drawFunction(input);
    drawSun();
    drawStickBall(input);

    /* 2D drawing */
    enable2D();
    bool twoG = globalData[creatureFitnessE].g.display && globalData[generationFitnessE].g.display;
    drawGraph(globalData[creatureFitnessE].g, 0, 0.5 * wx * (1.0 + !twoG));
    drawGraph(globalData[generationFitnessE].g, 0.5 * wx * twoG, wx);
    glColor3f(BLACK);
    drawLine(wx/2.0, (wy-45)/2.0+5, wx/2.0, (wy-45)/2.0-5);
    drawLine(wx/2.0+5, (wy-45)/2.0, wx/2.0-5, (wy-45)/2.0);
    createMenu();

    for (button * b = buttons; b != NULL; b = b->next) {
        if (b->toggled && !strcmp("System Info", b->label)) {
            displaySystemInfo(pop, genSize, id);
        }
    }
    drawDetails(genSize, gen, id, simTime, maxTime);


    glutSwapBuffers(); // Make it all visible
}

static void assignCords(button * b) {
    switch (b->group) {
        case 0:
            {
            int numButtons = getNumButtons(buttons, b->group);
            double width = (wx - 5) / (double) numButtons;
            b->x = b->id * width + 5;
            b->y = wy - 45;
            b->w = (b->id+1) * width;
            b->h = wy - 5;
            }
            break;
        case 1:
            b->x = wx * 0.90;
            b->y = 45 * (b->id);
            b->w = wx;
            b->h = 45 * (b->id + 1);
        break;
        default: return;
    }
    return;
}

void displaySystemInfo(creature * pop, int genSize, int id) {
    int yTop = 5 * 20;
    int yBottom = 5 * 20 + 20 * 14 + 20; // 50 % of screen
    drawSquare(0, yBottom, 220, yTop);
    glColor3f(WHITE);
    int height = yTop;
    char str[100];
    sprintf(str, "Creature Genes"); drawText(str, 0, height += 20, false, false);
    sprintf(str, "Total: %d",   pop[id].genome->iData[tot]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Nodes: %d",   pop[id].genome->iData[nod]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Bones: %d",   pop[id].genome->iData[bon]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Muscles: %d", pop[id].genome->iData[mus]); drawText(str, 20, height += 20, false, false);
    height+=20;
    double popStats[] = {0, 0, 0, 0, 0};
    for (int i = 0; i < genSize; i++) {
        popStats[0] += pop[i].genome->iData[tot];
        popStats[1] += pop[i].genome->iData[nod];
        popStats[2] += pop[i].genome->iData[bon];
        popStats[3] += pop[i].genome->iData[mus];
        popStats[4] += pop[i].fitness; // Only works because fitness for pop[i > id] = 0
    }
    double oneOverGenSize = 1.0 / (double) genSize;
    popStats[0] *= oneOverGenSize;
    popStats[1] *= oneOverGenSize;
    popStats[2] *= oneOverGenSize;
    popStats[3] *= oneOverGenSize;
    popStats[4] *= id == 0 ? 0 : 1.0 / (double) id;

    sprintf(str, "Population Genes"); drawText(str, 0, height += 20, false, false);
    sprintf(str, "Total: %.2f",   popStats[0]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Nodes: %.2f",   popStats[1]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Bones: %.2f",   popStats[2]); drawText(str, 20, height += 20, false, false);
    sprintf(str, "Muscles: %.2f", popStats[3]); drawText(str, 20, height += 20, false, false);
    height+=20;

    sprintf(str, "Population Fitness"); drawText(str, 0, height += 20, false, false);
    sprintf(str, "Current Average: %.2f",   popStats[4]); drawText(str, 20, height += 20, false, false);


}

#include "GameModes/inputFunctions.h"
void createMenu() {
    /* Create Buttons */
    if (buttons == NULL) {
        int i = 0;
        int j = 0;
        buttons = createButton("Fitness Graph"   , toggleCreatureGraph  , assignCords, j++    , 0, true , false);
        buttons = createButton("Generation Graph", toggleGenerationGraph, assignCords, j++    , 0, true , false);
        buttons = createButton("Exit"            , normalExit           , assignCords, i++    , 1, false, false);
        buttons = createButton("Back to Menu"    , nullFunc             , assignCords, i++    , 1, false, false);
        buttons = createButton("Full Screen"     , toggleFullScreen     , assignCords, i++    , 1, true , true );
        buttons = createButton("System Info"     , nullFunc             , assignCords, j++    , 0, true , false);
        buttons = createButton("Mutations"       , nullFunc             , assignCords, j++    , 0, true , false);
        buttons = createButton("Selection"       , nullFunc             , assignCords, j++    , 0, true , false);
        buttons = createButton("Save Genome"     , nullFunc             , assignCords, (++i)++, 1, false , false);
    }

    /* Draw Background */
    glColor3f(BLACK);
    drawSquare(0, wy, wx, wy-50);

    /* Highlighting */
    for (button * b = buttons; b != NULL; b = b->next) {
        b->highlighted = hoveringOver(b);
    }

    drawButtons();
    return;
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
        double minV = -40 / (i-10) + components->muscles[i].origLength;
        double maxV = +40 / (i-10) + components->muscles[i].origLength;
        double percent = (euc(locA, locB) - minV) / (maxV - minV);
        double expansivity = 8 * euc(locA, locB) / (maxV - minV) * euc(locA, locB) / (maxV - minV); // not sure why 8 works
        drawCylinder(locA.x, locA.y, locA.z,
                      locB.x, locB.y + 0.1, locB.z,
                       RADIUS * percent,     5);
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









