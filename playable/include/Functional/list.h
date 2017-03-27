#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "preproccessor.h"     // Struct

gene * getLastItem(gene * head);
gene * addToBack(gene * head, gene * toBeAdded);
gene * clearGenome(gene * head);
void removeItem(gene * head, gene * toRemove);

#endif // LIST_H_INCLUDED
