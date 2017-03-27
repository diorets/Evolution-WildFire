#ifndef GENES_H_INCLUDED
#define GENES_H_INCLUDED

#include "preproccessor.h"

#define RAND_NODE_HEIGHT randf(CAGESIZE / 1.3) + 5

/* Checking Functions */
bool invalidCon(int * connections, gene * genome);
posi goodPosition(gene * head);
conn goodConnection(gene * head, int a, int b, int numNodes);

/* New Genes */
gene* infoGene(int a, int b, int c, int d);

gene* nodeGene(posi loc);
gene* nodeGene(gene * head);

gene* muscleGene(int a, int b);
gene* muscleGene(gene * genome);

gene* boneGene(int numNodes);
gene* boneGene(int a, int b);
gene* boneGene(gene * genome);

gene* addMuscle(int numNodes, int a);
gene* addBone(int numNodes, int a);

gene * addAxon(int a, int b, int layer);

/* Printing */
void saveGenome();
void printGenome(int id, bool conn, bool loc);

#endif // GENES_H_INCLUDED
