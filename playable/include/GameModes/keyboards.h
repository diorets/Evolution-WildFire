#ifndef KEYBOARDS_H_INCLUDED
#define KEYBOARDS_H_INCLUDED

#define ESC 27
#define SPACE 32

void (*getKeyboardFunct(bool hold))(unsigned char);

void simKeyPress(unsigned char key);
void simKeyHold(unsigned char key);

void menuKeyPress(unsigned char key);
void mutKeyPress(unsigned char key);



#endif // KEYBOARDS_H_INCLUDED
