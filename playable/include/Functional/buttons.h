#ifndef BUTTONS_H_INCLUDED
#define BUTTONS_H_INCLUDED

#include "preproccessor.h"

#define HIGHLIGHT_DURATION 30

button * clearButtons(button * head);
button * createButton(const char * label, callback cb, void (*f)(button*), int id, int group, bool togglable, bool defaultToggle);
bool hoveringOver(button * b);
void drawButtons();
int getNumButtons(button * head, int group);

#endif // BUTTONS_H_INCLUDED

