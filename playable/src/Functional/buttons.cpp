#include "Functional/buttons.h"
#include <stdlib.h>
#include "Glut/myGlut.h"
#include "global.h"
#include "Visual/basic.h"
#include <stdio.h>
#include "Errorhandle/error.h"

static button * getLastButton(button * head) {
    button * current = head;
    while (1) {
        if (current->next == NULL) {
            return current;
        }
        else {
            current = current->next;
        }
    }
}

int getNumButtons(button * head, int group) {
    int i = 0;
    for (button * b = head; b != NULL; b = b->next) if (b->group == group) i++;
    return i;
}

button * createButton(const char * label, callback cb, void (*f)(button*), int id, int group, bool togglable, bool defaultToggle) {
	button * p = (button*) malloc( sizeof(button) );
	memset(p,0,sizeof(button));
	p->x = 0;
	p->y = 0;
	p->w = 0;
	p->h = 0;
	p->id = id;
	p->group = group;

	p->togglable = togglable;
	p->toggled = defaultToggle;
	p->clicked = false;
	p->callbackFunction = cb;
	p->label = (char*)malloc( strlen(label)+1 );
	if(p->label) {
		sprintf(p->label,label);
	}
	p->drawing = f;

	if (buttons == NULL) {
        p->next = NULL;
        return p;
	}
	button * lastItem = getLastButton(buttons);
	lastItem->next = p;
	return buttons;
}

button * addToBack1(button * head, button * toBeAdded) {
    button * toReturn = toBeAdded;
    if (head == NULL) {
        toReturn->next = NULL;
        return toReturn;
    }

    button * lastItem = getLastButton(head);
    lastItem->next = toBeAdded;
    return head;
}

#include "Glut/myGlut.h"
#define in_ 0.2
#define out_ 0.9

#define INSIDE in_,in_,in_
#define OUTSIDE out_,out_,out_
void drawButtons() {
    for (button * b = buttons; b != NULL; b = b->next) {
        glBegin(GL_QUADS);
            glColor3f(INSIDE);
            glVertex2f(b->x,b->y);
            glVertex2f(b->x,b->h);

            if (!b->togglable && b->clicked) {
                b->countDown = HIGHLIGHT_DURATION;
            }

            if (b->toggled || (!b->togglable && b->countDown > 0)) {
                if (b->countDown > 0) {
                    b->countDown--;
                }
                glColor3f(0, 0.81, 0.98);
            } else {
                glColor3f(OUTSIDE);
            }

            glVertex2f(0.5 * (b->x + b->w),b->h);
            glVertex2f(0.5 * (b->x + b->w),b->y);

            glVertex2f(0.5 * (b->x + b->w),b->y);
            glVertex2f(0.5 * (b->x + b->w),b->h);

            glColor3f(INSIDE);
            glVertex2f(b->w,b->h);
            glVertex2f(b->w,b->y);
        glEnd();

        if (b->highlighted) {
            glColor3f(RED);
        } else {
            glColor3f(WHITE);
        }
        glBegin(GL_LINES);
            glVertex2f(b->x, b->y);
            glVertex2f(b->w, b->y);

            glVertex2f(b->w, b->h);
            glVertex2f(b->x, b->h);

            glVertex2f(b->x, b->y);
            glVertex2f(b->x, b->h);

            glVertex2f(b->w, b->y);
            glVertex2f(b->w, b->h);

        glEnd();

        glColor3f(BLACK);
        drawText(b->label, 0.5 * (b->x + b->w), 0.5 * (b->y + b->h), true, true);
    }
}

button * clearButtons(button * head) {
    for (button * curr = head; curr != NULL;) {
        button * temp = curr->next;
        free(curr);
        curr = temp;
    }
    return NULL;
}

bool hoveringOver(button * b) {
    if (mousePos.x > b->x &&
        mousePos.x < b->w &&
        mousePos.y > b->y &&
        mousePos.y < b->h ){
            return true;
    }
    return false;
}



