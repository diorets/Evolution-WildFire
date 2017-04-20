#include "Visual/basic.h"


#include <math.h>
#include "Glut/myGlut.h"                // Glut functions and definitions
#include "global.h" // Camera
#include "preproccessor.h" // person
#include "Math/myMath.h"

/* Control Functions */
void enable2D() {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, wx, wy, 0.0, -1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_CULL_FACE);
    glClear(GL_DEPTH_BUFFER_BIT);
    return;
}

void reenable3D() {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    return;
}

#include <stdio.h>
/* 3D Drawing Functions */
void drawCube(double x, double y, double z, double radius, double theta) {
    double rad = radius;
    double r = rad + 0.00001;
    theta *= 57.2957795130823208767;

    glColor3f(BLUE);
    glPushMatrix();
        glTranslated(x, y, z);
        glRotated(theta, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);        // Draw The Cube Using quads
            glVertex3f( rad, rad,-rad);    // Top Right Of The Quad (Top)
            glVertex3f(-rad, rad,-rad);    // Top Left Of The Quad (Top)
            glVertex3f(-rad, rad, rad);    // Bottom Left Of The Quad (Top)
            glVertex3f( rad, rad, rad);    // Bottom Right Of The Quad (Top)
            glVertex3f( rad,-rad, rad);    // Top Right Of The Quad (Bottom)
            glVertex3f(-rad,-rad, rad);    // Top Left Of The Quad (Bottom)
            glVertex3f(-rad,-rad,-rad);    // Bottom Left Of The Quad (Bottom)
            glVertex3f( rad,-rad,-rad);    // Bottom Right Of The Quad (Bottom)
            glVertex3f( rad, rad, rad);    // Top Right Of The Quad (Front)
            glVertex3f(-rad, rad, rad);    // Top Left Of The Quad (Front)
            glVertex3f(-rad,-rad, rad);    // Bottom Left Of The Quad (Front)
            glVertex3f( rad,-rad, rad);    // Bottom Right Of The Quad (Front)
            glVertex3f( rad,-rad,-rad);    // Top Right Of The Quad (Back)
            glVertex3f(-rad,-rad,-rad);    // Top Left Of The Quad (Back)
            glVertex3f(-rad, rad,-rad);    // Bottom Left Of The Quad (Back)
            glVertex3f( rad, rad,-rad);    // Bottom Right Of The Quad (Back)
            glVertex3f(-rad, rad, rad);    // Top Right Of The Quad (Left)
            glVertex3f(-rad, rad,-rad);    // Top Left Of The Quad (Left)
            glVertex3f(-rad,-rad,-rad);    // Bottom Left Of The Quad (Left)
            glVertex3f(-rad,-rad, rad);    // Bottom Right Of The Quad (Left)
            glVertex3f( rad, rad,-rad);    // Top Right Of The Quad (Right)
            glVertex3f( rad, rad, rad);    // Top Left Of The Quad (Right)
            glVertex3f( rad,-rad, rad);    // Bottom Left Of The Quad (Right)
            glVertex3f( rad,-rad,-rad);    // Bottom Right Of The Quad (Right)
        glEnd();            // End Drawing The Cube - See more at: http://www.codemiles.com/c-opengl-examples/draw-3d-cube-using-opengl-t9018.html#sthash.R8kuU2ey.dpuf

        glColor3f(BLACK);
        glBegin(GL_LINE_STRIP);
            glVertex3f(+r, +r, +r);
            glVertex3f(+r, +r, -r);
            glVertex3f(+r, -r, -r);
            glVertex3f(+r, -r, +r);
            glVertex3f(+r, +r, +r);

            glVertex3f(-r, +r, +r);
            glVertex3f(-r, +r, -r);
            glVertex3f(-r, -r, -r);
            glVertex3f(-r, -r, +r);
            glVertex3f(-r, +r, +r);
        glEnd();
        glBegin(GL_LINES);
            glVertex3f(+r, -r, +r);
            glVertex3f(-r, -r, +r);

            glVertex3f(+r, +r, -r);
            glVertex3f(-r, +r, -r);

            glVertex3f(+r, -r, -r);
            glVertex3f(-r, -r, -r);
        glEnd();
    glPopMatrix();
    return;
}

void drawSphere(double x, double y, double z, double radius, int qualityA, int qualityB) {
    glPushMatrix();
        glTranslated(x, y, z);
        glutSolidSphere(radius, qualityA, qualityB);
    glPopMatrix();
    return;
}

void drawSphere(double x, double y, double z, double radius) {
    glPushMatrix();
        glTranslated(x, y, z);
        glutSolidSphere(radius, MED_QUAL);
    glPopMatrix();
    return;
}

void drawCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions) {
  // https://github.com/curran/renderCyliner/blob/master/renderCylinder.c
  GLUquadricObj *quadric = gluNewQuadric();
  gluQuadricNormals(quadric, GLU_SMOOTH);

  float dx = x2-x1;
  float dy = y2-y1;
  float dz = z2-z1;
  float d = sqrt( dx*dx + dy*dy + dz*dz );
  float ax;

  if (fabs(dz) < 1.0e-1) { // z = 0
    ax = 57.2957795*acos( dx/d ); // rotation angle in x-y plane
    if ( dy <= 0.0 )
      ax = -ax;
  }
  else {
    ax = 57.2957795*acos( dz/d ); // rotation angle
    if ( dz <= 0.0 )
      ax = -ax;
  }

  float rx = -dy*dz;
  float ry =  dx*dz;

  glPushMatrix();
      //draw the cylinder body
      glTranslatef( x1,y1,z1 );
      if (fabs(dz) < 1.0e-1) {
        glRotated(90.0, 0, 1, 0.0); // Rotate & align with x axis
        glRotated(ax, -1.0, 0.0, 0.0); // Rotate to point 2 in x-y plane
      }
      else {
        glRotated(ax, rx, ry, 0.0); // Rotate about rotation vector
      }
      gluQuadricOrientation(quadric,GLU_OUTSIDE);
      gluCylinder(quadric, radius, radius, d, subdivisions, 1);

      //draw the first cap
      gluQuadricOrientation(quadric,GLU_INSIDE);
      gluDisk( quadric, 0.0, radius, subdivisions, 1);
      glTranslatef( 0,0,d );

      //draw the second cap
      gluQuadricOrientation(quadric,GLU_OUTSIDE);
      gluDisk( quadric, 0.0, radius, subdivisions, 1);
  glPopMatrix();

  gluDeleteQuadric(quadric);
  return;
}

void drawLine(double x, double y, double z, double X, double Y, double Z) {
    glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(X, Y, Z);
    glEnd();
    return;
}

void glutDrawing() {
    // Clear, reset, camera
    glClearColor(SKY);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    if (person) cameraPos.z = 1.8; // Standing on Ground
    gluLookAt(
            cameraPos.x,      cameraPos.y,      cameraPos.z,
            cameraPos.x + cameraDir.x, cameraPos.y + cameraDir.y, cameraPos.z + cameraDir.z,
            0.0,    0.0,    1.0);
    glLineWidth(3);
    return;
}

/* 2D Drawing Functions */
void drawCircle(double xC, double yC, double r, int num_segments) {
    glBegin(GL_POLYGON);
    for(int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        glVertex2f(x + xC, y + yC);//output vertex
    }
    glEnd();
}

void drawSquare(double x, double y, double X, double Y, double r, double g, double b,
                 double r1, double g1, double b1, double alpha) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
        glColor4f(r, g, b, alpha);
        glVertex2f(x, y);
        glVertex2f(x, Y);
        glColor4f(r1, g1, b1, alpha);
        glVertex2f(X, Y);
        glVertex2f(X, y);
    glEnd();
    return;
}

void drawHollowSquare(double x, double y, double X, double Y) {
    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x, Y);
        glVertex2f(X, Y);
        glVertex2f(X, y);
    glEnd();
    return;
}

void drawSquare(double x, double y, double X, double Y) {
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x, Y);
        glVertex2f(X, Y);
        glVertex2f(X, y);
    glEnd();
    return;
}

void drawLine(double x, double y, double X, double Y) {
    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(X, Y);
    glEnd();
    return;

}

void drawPoint(double x, double y) {
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
    return;

}




































