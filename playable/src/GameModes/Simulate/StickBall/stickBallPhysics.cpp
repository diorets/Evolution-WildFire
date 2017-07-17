#include "GameModes/Simulate/StickBall/stickBallPhysics.h"

#include <math.h> // isinf, sinf
#include "Math/myMath.h"       // Various Vector Functions
#include "ErrorHandle/error.h" // Quit
#include "global.h" // Environment

#define PI 3.141592653589793
#define G  9.8066500

#define dt 1

//#define MUSCLEMACRO(i) 1.5 * sinf((float) ((*time) * dt * (2 * (i % 5 + 1)) * 0.005))
#define MUSCLEMACRO(i) 1 * sinf((float) ((*time) * dt * (2 * (i % 5 + 1)) * 0.005))

#define bounceBack 0.00
#define drag       0.98
#define constant   0.40
#define fricConst   0.25



double getStickBallFitness(creature individual) {
    enum {getFromPhysics, plusX, planeDistance, upHill, downHill};

    int measure = planeDistance;//environment[1]; // Change this!
    posi com = getCom(individual);
    posi origin = ((stickball*) individual.components)->origin;
    switch (measure) {
        case getFromPhysics : return individual.fitness;
        case plusX          : return com.x - origin.x;
        case planeDistance  : return euc2D(com, origin);
        case upHill         : return -sgn(environment[0]) * (com.x - origin.x);
        case downHill       : return sgn(environment[0]) * (com.x - origin.x);
        default : return euc2D(com, origin);
    }
}

static bool errorCheck(creature * current) {
    int len = current->genome->iData[nod];
    stickball * components = ((stickball*) current->components);

    for (int i = 0; i < len; i++) {
        posi loc = components->nodes[i].loc;
        if (mag(loc) > 100000) return true;
        if (isinf(loc.x) || isinf(loc.y) || isinf(loc.z)){
            return true;
        }
    }
    return false;
}
#include <stdio.h>

double ground(posi r) {
//    return 1*r.y * exp(0.002*(-r.x*r.x-r.y*r.y));
//    return -1.3*r.x;
    return 0;
//    return 3 * cos(0.25 * r.x);
    return 3 * sin(0.25 * r.x) * cos(0.25 * r.y);
}

bool updateStickBall(creature * current, int * time) {
//  (*time)++;  return false;
    if (errorCheck(current)) quit(CREATURE_ERROR);
    int nodeLen   = current->genome->iData[nod];
    int muscleLen = current->genome->iData[mus];
    int boneLen   = current->genome->iData[bon];

//    double groundAngle = environment[0];
    stickball * components = ((stickball*) current->components);
    double x_0 = getCom(*current).x;
    if (*time == 0) { // lower to ground
        double minZ = 30.0;
        for (int i = 0; i < nodeLen; i++) {
            posi * loc = &components->nodes[i].loc;
            minZ = (*loc).z < minZ ? (*loc).z : minZ;
        }
        for (int i = 0; i < nodeLen; i++) {
            posi * loc = &components->nodes[i].loc;
            (*loc).z -= minZ - 2 * RADIUS;
        }
    }
    if (*time == 100) { // give time to fall over
        components->origin = getCom(*current);
    }


    /* Node Initializtion */
    for (int i = 0; i < nodeLen; i++) {
        posi * force = &components->nodes[i].force;
        (*force)     = zero();
        (*force).z   = -G * components->nodes[i].mass / 1500.0;
    }


    /* Get Muscle Forces : Find force needed to get to this frames length */
    for (int i = 0; i <  muscleLen; i++) {
        int a = components->muscles[i].a;
        int b = components->muscles[i].b;
        double * currLength = &components->muscles[i].currLength;
//        double orgiLength = components->muscles[i].origLength;
        *currLength = euc(components->nodes[a].loc, components->nodes[b].loc);
        *currLength += MUSCLEMACRO(i);

        double deviation = components->muscles[i].origLength - *currLength;
        posi radVector = sub(components->nodes[a].loc, components->nodes[b].loc);
        //if (mag(radVector) < 0.01) continue; // idk if I need this

        posi deltaFor = scale(radVector, (deviation * constant) / (mag(radVector) * dt * dt));

        components->nodes[a].force = add(components->nodes[a].force, scale(deltaFor, components->nodes[a].mass));
        components->nodes[b].force = sub(components->nodes[b].force, scale(deltaFor, components->nodes[b].mass));
    }

    /* Get Bone Forces */
    for (int i = 0; i < boneLen; i++) {
        int a = components->bones[i].a;
        int b = components->bones[i].b;
        double deviation = components->bones[i].length - euc(components->nodes[a].loc, components->nodes[b].loc);

        posi radVector = sub(components->nodes[a].loc, components->nodes[b].loc);
       // if (mag(radVector) < 0.01) continue;

        posi deltaAcc = scale(radVector, (deviation * constant) / (mag(radVector) * dt * dt));

        components->nodes[a].force = add(components->nodes[a].force, scale(deltaAcc, components->nodes[a].mass));
        components->nodes[b].force = sub(components->nodes[b].force, scale(deltaAcc, components->nodes[b].mass));
    }

    /* Friction, Collision and Apply Forces */
    for (int i = 0; i < nodeLen; i ++) {
        posi * force = &components->nodes[i].force;
        posi * vel   = &components->nodes[i].vel;
        posi * loc   = &components->nodes[i].loc;
        double mass = components->nodes[i].mass;
        double recipMass = 1.0 / mass;

        double groundHeight = ground(*loc);
        if ((*loc).z <=  RADIUS + groundHeight) {
            posi N = normal(ground, *loc);
            double bounce = 0.4;


//            posi normalForce = scale(N, -bounce * 2 * dot(N, *vel) * mass / dt);
//            (*force) = add(*force, normalForce);
            (*vel) = sub(*vel, scale(N, bounce * 2 * dot(N, *vel))); // normal force
            (*loc).z = RADIUS + groundHeight;


            posi norm = normal(ground, *loc);
            double x = norm.z / norm.x;
            double y = norm.z / norm.y;
            (*vel).x *= (1 - (0.1 * dt))  * (isinf(x) || (x>100)? 1 : x / sqrt(1 + x*x));
            (*vel).y *= (1 - (0.1 * dt))  * (isinf(y) || (y>100)? 1 : y / sqrt(1 + y*y));
        }
        (*vel).x += (*force).x * dt * recipMass;
        (*vel).y += (*force).y * dt * recipMass;
        (*vel).z += (*force).z * dt * recipMass;

        (*vel).x *= 1 - 0.001 * dt;
        (*vel).y *= 1 - 0.001 * dt;
        (*vel).z *= 1 - 0.001 * dt;

        (*loc).x += (*vel).x * dt;
        (*loc).y += (*vel).y * dt;
        (*loc).z += (*vel).z * dt;

        if ((*loc).z >= 200) { // If they go over this, it is very likely an error.
            return true;
        }
    }

    /* Graphing Creature Fitness */
    if ((*time) == 0) { // Clear Graph
        globalData[creatureFitnessE].g.numEntries = 0;
        if (globalData[creatureFitnessE].g.points != NULL) {
            free(globalData[creatureFitnessE].g.points);
            globalData[creatureFitnessE].g.points = NULL;
        }
    }
    if ((*time) % 10 == 0 && (*time) > 100) { // Add Data Point
        int numEntries = globalData[creatureFitnessE].g.numEntries;
        globalData[creatureFitnessE].g.points = (double*) realloc(globalData[creatureFitnessE].g.points, sizeof(double) * (1+numEntries));
        globalData[creatureFitnessE].g.points[numEntries] = euc2D(zero(), getCom(*current));
        globalData[creatureFitnessE].g.numEntries++;
    }
    posi com = getCom(*current);
    double x_1 = com.x;
    // Bonus for moving distance, scaled by your heigh, but deviations from x axis is heavy penalty
    current->fitness += (x_1 - x_0) * com.x * com.z * exp(-com.y * com.y);

    (*time)++;
    return false;
}































