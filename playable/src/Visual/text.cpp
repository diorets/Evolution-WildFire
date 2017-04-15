#include <stdarg.h>
#include <stdio.h> // sprintf
#include "Visual/basic.h"
#include "ErrorHandle/error.h"
#include <stdlib.h> // Free
#include "preproccessor.h"
#include "Glut/myGlut.h"
#include "Math/myMath.h"
#include "global.h"
#include "GameModes/mice.h" // bound

void write(const char * format, int x, int y, ...) {
    va_list args;
    va_start(args, y);
    char str[1000];
    vsnprintf(str, 1000, format, args);
    drawText(str, x, y, true);
    va_end(args);
}

void write(const char * format, int x, int y, int len) {
    char * str = (char*) malloc(sizeof(char) * (len + 1));
    if (str == NULL) quit(MALLOC_ERROR);
    sprintf(str, format);
    drawText(str, x, y, true);
    free(str);
    return;
}


posi writeParagraph1(const char * str1, int x, int y, double letterHeight) {
    char * str = strdup(str1);
    char * token = strtok(str, "\n");
    int i = 0;
    int maxX = -1;

    /* walk through other tokens */
    for (;token != NULL; i++) {
        int temp = drawText(token, x, y + letterHeight * i, true);
        if (temp > maxX) maxX = temp;
        token = strtok(NULL, "\n");
    }

    free(str);
    return vec((float) maxX,(float) y + i * letterHeight, 0);
}


void drawMenu(const char * str, const char * keys, int ox, int oy) {
    int letterHeight = 30;
    posi last = writeParagraph1(str, ox, oy, letterHeight);
    glColor3f(BROWN);
    drawSquare(ox - last.x / 2 - 20,     oy - letterHeight ,
               ox + last.x / 2 + 55    , last.y - letterHeight / 2,
               GREEN, RED, 0.3);
    glColor3f(BLACK);

    drawLine(ox + last.x / 2, oy - letterHeight ,
             ox + last.x / 2, last.y - letterHeight / 2);
    writeParagraph(keys, ox + last.x / 2 + 12 * 2.7, oy, letterHeight);

    glColor4f(BLUE, 0.5);
    int numOptions = 0;
    for (unsigned int i = 0; i < strlen(str); i++) {
        if (str[i] == '\n') {
            numOptions++;
        }
    }

    for (int j = 0; j < numOptions; j++) {
        int x1 = ox - last.x / 2  - 20;
        int y1 = (oy - letterHeight + (j + 1) * letterHeight - letterHeight) * 1.015;
        int x2 = ox + last.x / 2;
        int y2 = (oy - letterHeight + (j + 2) * letterHeight - letterHeight / 2) * 0.985;

        if (bound(mousePos.x, mousePos.y, x1, y1, x2, y2)) {
            drawSquare(x1, y1, x2, y2);
            break;
        }
    }


}

double writeParagraph(const char * str1, int x, int y, double letterHeight) {
    char * str = strdup(str1);
    char * token = strtok(str, "\n");
    int i = 0;

    /* walk through other tokens */
    for (;token != NULL; i++) {
       write(token, x, y + letterHeight * i, strlen(token));
       token = strtok(NULL, "\n");
    }
    free(str);
    return y + i * letterHeight;
}


int drawText(const char *str, float x, float y, bool center) { // centers
    int width = 0;
    if (center) {
        for (const char *c = str; *c != '\0'; c++) {
            width += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
        x -= width * 0.5;
    }
    glRasterPos2f(x, y);
    for (const char *c = str; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    return x;
}

void drawHeader(const char *str, float x, float y) {
    int width = 0;
    for (const char *c = str; *c != '\0'; c++) {
        width += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    x -= width * 0.5;

    glRasterPos2f(x, y);
    for (const char *c = str; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}

void drawText(const char *str, float x, float y, void* font) {
    glRasterPos2f(x, y);
    for (const char *c = str; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}







