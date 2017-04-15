#ifndef PREPROCCESSOR_H_INCLUDED
#define PREPROCCESSOR_H_INCLUDED

#include <stdlib.h> // NULL

/* Constants */
#define WINDOW_X 800
#define WINDOW_Y 400
#define WINDOW_SIZE WINDOW_X,WINDOW_Y
#define WINDOW_POSITITION 600,100

#define RADIUS 0.6

/* Bounds */
#define CAGESIZE 20
#define MIN_NODE_DISTANCE 1.5
#define MAX_ELEMENTS 100
#define HIDDEN_LAYER_SIZE 6

/* Macros */
#define FOR_ALL(head, type) for (gene * current = head; current != NULL; current = current->next) if (current->start == type)
#define FOR_ALL_GENES(head) for (gene * current = head; current != NULL; current = current->next)

/* Enums */
enum simMode {skipE, debugE, instructionsE, graphE, howToE, goThroughGenE,
 simPositionE, timeGenScreenE, displaySkinE, quickGenE, selectionPowerE};

enum {inf, nod, bon, mus, neu, tot};
enum {xposi, yposi, zposi, mass, fric};
enum {connectionA, connectionB, layerE, weightE = 0};
enum {startMode, simMode, editMode, mutMode};
enum {black, darkBlue, darkGreen, darkAqua, darkRed, darkPurple, darkYellow, grey, darkGrey, blue, green, aqua, red, purple, yellow, white};

typedef union generic_unions {
    int i;
    bool b;
    char c;
    double f;
} generic;

/* Structs */
typedef struct organized_struct {
    double order;
    double unord;
} ord;

typedef struct conn_struct {
    int a;
    int b;
} conn;

typedef struct posi_struct {
    double x;
    double y;
    double z;
} posi;

typedef struct node_struct {
    posi force;
    posi vel;
    posi loc;
    double mass;
    double friction;
} node;

typedef struct bone_struct {
    int a; // connections
    int b;
    double length;
} bone;

typedef struct muscle_struct {
    int a; // connections
    int b;
    double currLength;
    double prevLength;
    double origLength;
    double potential;
} muscle;

typedef struct axon_struct {
    int a;
    int b;
    int layer;
    double weight;
} axon;

typedef struct gene_struct {
    char start;
    int iData[10]; // Should probably be replaced by union
    double fData[10];
    char endof;
    struct gene_struct * next;
} gene;

typedef struct creature_struct {
    double fitness;
    gene *genome;
    void * components;
} creature;

typedef struct stickball_struct {
    posi   origin;
    node   nodes[MAX_ELEMENTS];
    bone   bones[MAX_ELEMENTS];
    muscle muscles[MAX_ELEMENTS];
    axon   axons[MAX_ELEMENTS];
} stickball;

typedef struct turbine_struct {
    posi blocks[MAX_ELEMENTS];
} turbine;

#endif // PREPROCCESSOR_H_INCLUDED

























