#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include "preproccessor.h"

bool updateSystem(int system, creature * individual, int time);
double getFitness(int system, creature individual);

#endif // PHYSICS_H_INCLUDED
