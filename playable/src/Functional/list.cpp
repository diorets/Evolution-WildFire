#include "Functional/list.h"

#include <stdlib.h> // NULL, free

#include "ErrorHandle/error.h" // quit

gene * getLastItem(gene * head) {
    gene * current = head;
    while (1) {
        if (current->next == NULL) {
            return current;
        }
        else {
            current = current->next;
        }
    }
}

gene * addToBack(gene * head, gene * toBeAdded) {
    gene * toReturn = toBeAdded;
    if (head == NULL) {
        toReturn->next = NULL;
        return toReturn;
    }

    gene * lastItem = getLastItem(head);
    lastItem->next = toBeAdded;
    return head;
}

gene * clearGenome(gene * head) {
    for (gene * curr = head; curr != NULL;) {
        gene * temp = curr->next;
        free(curr);
        curr = temp;
    }
    return NULL;
}

void removeItem(gene * head, gene * toRemove) {
    // toRemove being head is not handled because the i gene should never be removed
    gene * prev = head;
    while (prev->next != NULL && prev->next != toRemove) {
        prev = prev->next;
    }

    if(prev->next == NULL) { // Couldnt find item
        quit(UNSPECIFIC_ERROR); // Should never request for this
    }
    prev->next = prev->next->next; // Remove Item
    if (toRemove != NULL) free(toRemove);
    return;
}





























