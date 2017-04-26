#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED
#include <stdarg.h>
double writeParagraph(const char * request, int x, int y, double letterHeight);
void drawMenu(const char * str, const char * keys, int x, int y);
void write(const char * format, int x, int y, ...);
void drawHeader(const char *str, float x, float y);
int drawText(const char *str, float x, float y, bool centerX, bool centerY);
void drawText(const char *str, float x, float y, void* font);


#endif // TEXT_H_INCLUDED
