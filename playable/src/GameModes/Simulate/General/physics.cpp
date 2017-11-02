#include "GameModes/Simulate/General/physics.h"
#include "GameModes/Simulate/StickBall/stickBallPhysics.h"
#include "GameModes/Simulate/Turbine/turbinePhysics.h"
#include "GameModes/Simulate/Cannon/cannonPhysics.h"
#include "GameModes/Simulate/Cube/cubePhysics.h"
#include "ErrorHandle/error.h"
#include "global.h" // playBackSpeed
bool updateSystem(int system, creature * individual, int * time) {
    switch(system) {
        case stickballE: return updateStickBall(individual, time);
        case turbineE: return updateTurbine(individual, time);
        case cannonE: return updateCannon(individual, time);
        case cubeE: return updateCube(individual, time);
        default: quit(INPUT_ERROR); return false;
    }
}

double getFitness(int system, creature individual) {
    switch(system) {
        case stickballE: return getStickBallFitness(individual);
        case turbineE: return getTurbineFitness(individual);
        case cannonE: return getCannonFitness(individual);
        case cubeE: return getCubeFitness(individual);
        default: quit(INPUT_ERROR); return -0.0;
    }
}
