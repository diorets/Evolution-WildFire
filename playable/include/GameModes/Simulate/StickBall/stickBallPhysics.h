#ifndef STICKBALLPHYSICS_H_INCLUDED
#define STICKBALLPHYSICS_H_INCLUDED

#include "preproccessor.h"

bool updateStickBall(creature * specimen, int * simTime);
double getStickBallFitness(creature individual);
double ground(posi r);
#endif // STICKBALLPHYSICS_H_INCLUDED
