#ifndef BASIC_H_INCLUDED
#define BASIC_H_INCLUDED

void callKeyboard(unsigned char key, bool hold); // ('\0', false)->hold, (key, true)->press
void changeSize(int w, int h);
void update(void);

void keyPressed (unsigned char key, int x, int y);
void keyUp (unsigned char key, int x, int y);
void pressSpecialKey(int key, int kxx, int kyy);
void releaseSpecialKey(int key, int kx, int ky);

void mouseMove(int mx, int my);
void callMouse(int button, int state, int mx, int my);
void passiveMouse(int mx, int my);

#endif // GLOBAL_H_INCLUDED
