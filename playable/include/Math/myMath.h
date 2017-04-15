#ifndef MYMATH_H_INCLUDED
#define MYMATH_H_INCLUDED

#include "preproccessor.h"
#include <stdlib.h>

bool chance(double x);
bool chance(int x);
double randf(double number);
double pmRandf(double number);

double euc(posi a, posi b);
double euc2D(posi a, posi b);
posi sub(posi a, posi b);
posi add(posi a, posi b);
posi add(posi a, double k);
posi radForce(posi a, posi b, double force);
double mag(posi a);
posi scale(posi a, double scale);
posi zero();
posi vec(double, double, double);
posi cross(posi a, posi b);

double dot(posi a, posi b);

posi getCom(creature individual);
posi getAvgNodeVel(creature current);

int fact(int n);
int comb(int n);
int sgn(double x);


#endif // MYMATH_H_INCLUDED
