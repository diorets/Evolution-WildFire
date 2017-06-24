#ifndef MYMATH_H_INCLUDED
#define MYMATH_H_INCLUDED

#include "preproccessor.h"
#include <stdlib.h>

double randf2(double a, double b);
double randf2(int a, int b);
bool chance(double x);
bool chance(int x);
double randf(double number);
double pmRandf(double number);
posi normalize(posi a);
posi normal(double (*surface)(posi), posi r);

bool equals(double a, double b, double eps);
double euc(posi a, posi b);
double euc2D(posi a, posi b);
posi sub(posi a, posi b);
posi add(posi a, posi b);
posi add(posi a, double k);
posi compMul(posi a, posi b);
double der(double (*f)(posi), posi a, int i);
bool equals(posi a, posi b, double eps);
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
