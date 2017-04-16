#include "GameModes/Simulate/General/physics.h"
#include "GameModes/Simulate/SpPhysics/stickBallPhysics.h"

bool updateSystem(int system, creature * individual, int time) {
    switch(system) {
        case stickballE: return updateStickBall(individual, time);
        default: return false;
    }
}

double getFitness(int system, creature individual) {
    switch(system) {
        case stickballE: return getStickBallFitness(individual);
        default: return -1;
    }
}
