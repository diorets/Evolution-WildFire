#include <stdio.h>             // sprintf

#include "ErrorHandle/error.h"
#include "Visual/basic.h"      // drawText, enable2D
#include "Glut/myGlut.h"       // glutDrawing, swapbuffers
#include "Functional/sleep.h"  // sleep_ms
#include "global.h"

static void drawMessage(int code) {
    glColor3f(BLACK);
    switch(code) {
        case NORMAL_EXIT:
            drawText("The Program Has Exited Normally.", wx * 0.2, wy * 0.5, false);
            glutSwapBuffers();
            sleep_ms(1000);
//            while(true);
            exit(EXIT_SUCCESS);
        case MALLOC_ERROR:
            drawText("MEMORY ALLOCATION FAILURE.", wx * 0.2, wy * 0.5, false);
            break;
        case INPUT_ERROR:
            drawText("UNHANDLED / EXPECTED INPUT FAILURE.", wx * 0.2, wy * 0.5, false);
            break;
        case GENOME_ERROR:
            drawText("GENOME FAILURE.", wx * 0.2, wy * 0.5, false);
            break;
        case CREATURE_ERROR:
            drawText("CREATURE FAILURE.", wx * 0.2, wy * 0.5, false);
            break;
        case UNSPECIFC_ERROR:
            drawText("UNSPECIFIC FAILURE.", wx * 0.2, wy * 0.5, false);
            break;
        default:
            drawText("UNKNOWN FAILURE", wx * 0.2, wy * 0.5, false);
            break;

    }
    return;
}


void quit_(int code, int line, const char * file) {
    char str[20];
    if ((code < 100) && (code % 100 == 0)) code *= 100;

    glutDrawing();
    glutSwapBuffers();
    enable2D();
    drawMessage(code);

    if (code == 0) sprintf(str, "Error Code 000");
    else           sprintf(str, "Error Code %d", code);
    drawText(str, wx * 0.2, wy * 0.54, false);
    sprintf(str, "Occured at line %d", line);

    drawText(str, wx * 0.2, wy * 0.57, false);
    sprintf(str, "In File: %s", file);

    drawText(str, wx * 0.2, wy * 0.60, false);
    drawText("Program will terminate shortly.", wx * 0.2, wy * 0.63, false);

    glutSwapBuffers();
    while(true);
    sleep_ms(5000);
    exit(EXIT_FAILURE);
}
















