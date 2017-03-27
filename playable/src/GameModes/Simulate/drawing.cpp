#include "GameModes/mice.h"    // Various keyboards
#include "global.h"
#include "Glut/myGlut.h"
#include "GameModes/Simulate/drawing.h"
#include "GameModes/Simulate/sim.h"
#include "Visual/basic.h"
#include "Visual/objects.h"
#include "Functional/sleep.h"

#include <stdio.h>
#include <math.h>
#include <Math/myMath.h>
#include "ErrorHandle/error.h"



void draw() {
    if ((simTime % 5 == 0) && !globalData[skipE].b && globalData[graphE].b) drawDisGraph(simTime == 0, true);
    if (!(simTime % playBackSpeed)) {
        /* Clear, Reset, Camera */
        glutDrawing();

        /* 3D Drawing */
        reenable3D();
        drawGround();
        drawSun();
        if (globalData == NULL) exit(6);
        drawCreature(specimen[id + 0], globalData[displaySkinE].b);

        /* 2D drawing */
        enable2D();
        drawStats();
        drawDisGraph(false, false);
        glutSwapBuffers(); // Make it all visible
        sleep_ms(FRAME_SLEEP); // Why do this?
    }
    return;
}

void drawStats() {
    glColor3f(BLACK);
    if (globalData[howToE].i != 0) {
        int boxx = wx * 0.05;
        int boxy = wy * 0.70;

        drawSquare(boxx, boxy, wx - boxx, wy * 0.95,
        GREEN, WHITE, 0.6);

        glColor4f(GREY, 0.8);
        if (bound(mousePos.x, mousePos.y, wx * 0.85, boxy, wx * 0.95, wy * 0.95)) {
            glColor4f(WHITE, 0.8);
        } else {
            glColor4f(BLACK, 0.8);
        }
        drawSquare(wx * 0.85, boxy, wx * 0.95, wy * 0.95);
        glColor4f(BLUE, 0.8);
        drawCircle(wx * 0.90, wy * 0.5 * (0.95 + 0.70), 70, 20);
        glColor4f(WHITE, 0.8);
        drawHeader("NEXT", 0.9 * wx, 0.5 * (0.95 + 0.70)* wy);


        int y = boxy * 1.07;
        glColor3f(BLACK);
        switch(globalData[howToE].i) {
            case 1: writeParagraph(
                    "Welcome! This is your first creature!\n"
                    "Let's call him Dennis. Dennis is made of nodes (spheres), muscles (red lines), bones (white lines) and brain cells (not yet implemented)\n"
                    "His nodes can colide with the ground (not scenery yet) and produce friction letting him push off and (hopefully!) move. His muscles contract in irregular\n"
                    "intervals giving him his power. His bones are a bit squishy but they try not to change length, giving him structure.\n"
                    "And his skin (pink) which just packages him up nicely, but doesnt actually do anything\n"
                    "These are all randomly generated when you start this game mode.\n"
                    "", wx / 2, y, 24);
                    break;
            case 2: writeParagraph(
                    "Dennis is kind of useless, but to be fair his entire species has only\n"
                    "been around for about 30 seconds! So you can't really blame him for not being able to do anything.\n"
                    " \n"
                    "Let's see if you can get Dennis to move... But how could you do that? Given that he's just a random collection of sticks and circles...\n"
                    "Well first I should mention, Dennis isnt the only one here, he has siblings!\n"
                    "Press SPACE BAR to look at the next creature. (Use WASD,EQ and ARROW KEYS to move around).\n"
                    "", wx / 2, y, 24);
                    break;
            case 3: writeParagraph(
                    "In fact, he has about 200 randomly generated siblings!\n"
                    "This set of 200 is called a 'Generation'. \n"
                    " \n"
                    "To progress the ability to walk, we simulate this whole generation, and at the end we have to make some decisions...\n"
                    "Unfortunalety, there isnt enough room for all of Denis' siblings, and their children.\n"
                    "We can only take 200 with us to the next generation, so we will take the fastest children!\n"
                    "", wx / 2, y, 24);
                    break;
            case 4: writeParagraph(
                    "Now that we got aquinted with Dennis, this probably isnt a good time to mention that he won't make it...\n"
                    "But not to fear! If he is fast enough, his genes and his children will survive into the next generation!\n"
                    " \n"
                    "You may have noticed, that Denice and his siblings dont have any hair or sunscreen, this means they have no protection against the harsh sun.\n"
                    "And this causes them to get mutations. These can affect their structure and how they function.\n"
                    "They are mostly bad, but if we try enough times, we will stumble onto some good ones!\n"
                    "", wx / 2, y, 24);
                    break;
            case 5: writeParagraph(
                    "And that's it... That is all it takes to get - Not Dennis, but his children's children's ... children to walk!\n"
                    "Generate random creatures, pick out the fastest ones, and mutate them a little bit, and repeat!\n"
                    "The only other ingredient is time...\n"
                    "", wx / 2, y, 24);
                    break;
            case 6: writeParagraph(
                    "If we press ENTER, we can watch this whole generation go by! In this game, we can can \"play God\" by changing the environment,\n"
                    "the mutation rates, selecting the survival criteria (albeit limited at this point), along with a lot of future \"Powers\"!\n"
                    "Your main controls are SPACE (next creature), ENTER (Fastforward a generation), +/- to change speed and the buttons on screen.\n"
                    "Pressing 1 will open controls if you need help. Press B or click NEXT to begin normal game play.\n"
                    " \n"
                    "Nerd Fact: All the nodes, connections, and changing forces, velocities, and positions, makes an unknown function describing their distance over time\n"
                    "that depends all these variables. The method of finding the \"fastest\" creature is finding a maximum in this function! (~Gradient Ascent)\n"
                    "", wx / 2, y, 24);
                    break;
            default: break;
        }
    } else {
        char str[20];
        glColor3f(RED);
        /* General Stats */
        drawText("Press: 1 -> Toggle Help, 2 -> Toggle Creature Data, 3-> Toggle Graph",0,20,false);
        sprintf(str, "Generation:  %d", gen);
        int height = 20;
        drawText(str, 2, height += 20, false);
        sprintf(str, "Specimen ID: %d / %d (%.2f%%)", id, genSize, 100 * id / (double) genSize);
        drawText(str, 5, height += 20, false);
        sprintf(str, "Time: %d / %d (%.2f%%)", simTime, MAX_TIME, 100 * simTime / (double) MAX_TIME);
        drawText(str, 5, height += 20, false);
        sprintf(str, "Playback Speed: %dx", playBackSpeed);
        drawText(str, 5, height += 20, false);

        glColor3f(RED);
        double y = wy * 0.75;
        if (!globalData[graphE].b) y = wy * 0.9;
        double x1 = wx * 0.1;
        double x2 = wx * 0.2;
        double r = 50;
        if (bound(mousePos.x, mousePos.y, x1 - r, y - r, x1 + r, y + r)) {
            glColor3f(WHITE);
        } else glColor3f(RED);
        drawCircle(x1, y, r, 10);
        glColor3f(BLACK);
        writeParagraph("Rotate World\nDown (about red)", x1, y, 24);
        write("Angle %0.2f", 0.5 * (x1 + x2), y * 0.95, 180 / 3.14159* environment[0]);

        if (bound(mousePos.x, mousePos.y, x2 - r, y - r, x2 + r, y + r)) {
            glColor3f(WHITE);
        } else glColor3f(RED);
        drawCircle(x2, y, r, 10);
        glColor3f(BLACK);
        writeParagraph("Rotate World\n Up (about red)", x2, y, 24);



        double x = wx * 0.9;
        y = wy * 0.1;
        r = 50;
        if (bound(mousePos.x, mousePos.y, x - r, y - r, x + r, y + r)) {
            glColor3f(WHITE);
        } else glColor3f(SKIN);
        drawCircle(x, y, r, 10);
        glColor3f(BLACK);
        writeParagraph("  Toggle\nSkin", x, y, 24);


        y = wy * 0.75;
        if (!globalData[graphE].b) y = wy * 0.9;
        x = wx * 0.3;
        r = 50;
        if (bound(mousePos.x, mousePos.y, x - r, y - r, x + r, y + r)) {
            glColor3f(WHITE);
        } else glColor3f(GREEN);
        drawCircle(x, y, r, 10);
        glColor3f(BLACK);
        writeParagraph("   Dis / Time Graph", x, y, 24);


        glColor3f(BLACK);
        if (globalData[instructionsE].b) {
            writeParagraph("WASD,EQ(z)\n"
                           "Arrows\n"
                           " \n"
                           "SPACE\n"
                           "ENTER\n"
                           "+\n"
                           "-\n"
                           " \n"
                           "F\n"
                           "L\n"
                           "P\n"
                           " \n"
                           "O\n"
                           "ESC", wx* 0.7, wy * 0.33, 24);
            writeParagraph("Camera Position\n"
                           "Camera Direction\n"
                           " \n"
                           "Skip Creature (Use This!)\n"
                           "Go Through Generation\n"
                           "Speed Up Time (Use This!)\n"
                           "Slow Down Time\n"
                           " \n"
                           "Toggle FullScreen\n"
                           "Toggle Lights (Broken)\n"
                           "Toggle Person (No Flying)\n"
                           " \n"
                           "Reset Camera (If screen goes blue)\n"
                           "Exit Simulator", wx* 0.9, wy * 0.33, 24);


            /* Explanation */
            writeParagraph("If you want to watch them move:\n"
            "Press ENTER so the generations go by very quickly. At the end of a generation, you will be able to change mutation rates.\n"
            "You can change the game speed with +/- to see them in more detail\n"
            "Remember! These are just randomly assembled, and randomly mutated, and selected so they might take time to learn to walk!\n"
            "They should start moving after a few generations. Note: Rotating the world produces a graphical glitch (I let you ROTATE the PLANET! Give me a break!)\n"
            "Close interfaces (Notebly this one) if you dont need them as they slow down the game.\n"
            "", wx / 2, wy * 0.1, 24);
        }
        if (globalData[debugE].b) {
            /* Creature Details */


//            FOR_ALL(specimen[id].genome, 'a') {
//                int X1 = 0.55 * wx;
//                int X2 = 0.65 * wx;
//                if (current->iData[layerE] == 1) {
//
//                }
//                int A = current->iData[connectionA];
//                int B = current->iData[connectionB];
//                drawLine(X1, wy * (0.4 + 0.05 * A),
//                         X2, wy * (0.4 + 0.05 * B));
//            }


            int * sizes = specimen[id].genome->iData;
            write("Genes:%d, Nodes:%d Muscles:%d, Bones:%d, Neurons:%d", wx * 0.2, wy * 0.28, sizes[tot] - 1, sizes[nod], sizes[mus], sizes[bon], sizes[neu]);

//            drawSquare(wx * 0.5, wy * 0.1, wx * 0.8, wy * 0.9);
//            glColor3f(WHITE);
//            drawText("In the process of adding neural network", wx * 0.55, wy * 0.15, false);
//
//
//            glColor3f(YELLOW);
//            for (int i = 0; i < sizes[neu]; i++) {
//                int layer = specimen[id].axons[i].layer;
//                x1 = (0.55 + 0.1 * layer) * wx;
//                x2 = (0.65 + 0.1 * layer) * wx;
//
//                int A = specimen[id].axons[i].a;
//                int B = specimen[id].axons[i].b;
//                drawLine(x1, wy * (0.2 + 0.1 * A),
//                         x2, wy * (0.2 + 0.1 * B));
//            }


//            int radius = 20;
//            for (int i = 0; i < sizes[mus]; i++) {
//                glColor3f(RED);
//                drawCircle(0.55 * wx, wy * (0.2 + 0.1 * i), radius, 20);
//
//                glColor3f(WHITE);
//                double input = specimen[id].muscles[i].currLength / specimen[id].muscles[i].origLength - 1;
//                input /= 1.5 / 100.0;
//                write("%+06.2f", 0.55 * wx, wy * (0.2 + 0.1 * i), input);
//            }
//            glColor3f(BLUE);
//            for (int i = 0; i < HIDDEN_LAYER_SIZE; i++) {
//                drawCircle(0.65 * wx, wy * (0.2 + 0.1 * i), radius, 20);
//            }
//            glColor3f(RED);
//            for (int i = 0; i < sizes[mus]; i++) {
//                drawCircle(0.75 * wx, wy * (0.2 + 0.1 * i), radius, 20);
//            }




            glColor3f(GREEN);
            for (int i = 0; i < sizes[nod]; i++) {
                posi loc = specimen[id].nodes[i].loc;
                sprintf(str, "(%+5.2f, %+5.2f, %+5.2f)", loc.x, loc.y, loc.z);
                drawText(str, wx * 0.01, wy * 0.3 + 24 * i, false);
            }
            glColor3f(RED);
            for (int i = 0; i < sizes[mus]; i++) {
                int a = specimen[id].muscles[i].a;
                int b = specimen[id].muscles[i].b;
                sprintf(str, "(%d, %d)", a, b);
                drawText(str, wx * 0.13, wy * 0.3  + 24 * i, false);
            }
            glColor3f(WHITE);
            for (int i = 0; i < sizes[bon]; i++) {
                int a = specimen[id].bones[i].a;
                int b = specimen[id].bones[i].b;
                sprintf(str, "(%d, %d)", a, b);
                drawText(str, wx * 0.18, wy * 0.3  + 24 * i, false);
            }
        }
    }
    return;
}

void traceLines(double * entries, double maxEntry, int numEntriesI, double r, double g, double b){
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
        dis[num] = euc2D(       getCom(specimen[id]), specimen[id].origin);
        vel[num] = euc2D(getAvgNodeVel(specimen[id]), specimen[id].origin);
        acc[num] = euc2D(getAvgNodeVel(specimen[id]), specimen[id].origin); // Fix ME

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
                sprintf(str, "%d(%d%%)", simTime * i / numDiv, 100 *  i / numDiv);
                drawText(str, 0.95 * wx * i / numDiv, wy * 0.92, false);
            }
            sprintf(str, "Max Dis: %0.2f-", maxDis);
            drawText(str, 0.5, wy * 0.815, false);
        }
    }
    return;
}
