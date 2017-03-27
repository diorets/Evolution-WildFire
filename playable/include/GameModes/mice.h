#ifndef MICE_H_INCLUDED
#define MICE_H_INCLUDED

bool bound(int x, int y, int x1, int y1, int x2, int y2);
void (*getMouseFunct(bool hold))(int, int, int, int);

#endif // MICE_H_INCLUDED
