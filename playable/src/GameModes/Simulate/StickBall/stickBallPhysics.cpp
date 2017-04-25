#include "GameModes/Simulate/StickBall/stickBallPhysics.h"

#include <math.h> // isinf, sinf
#include "Math/myMath.h"       // Various Vector Functions
#include "ErrorHandle/error.h" // Quit
#include "global.h" // Environment

#define PI 3.141592653589793
#define G  9.8066500

#define dt 1

#define MUSCLEMACRO(i) 1.3 * sinf((float) ((*time) * (10 - i)) * 0.005)

#define bounceBack 0.00
#define drag       0.99
#define constant   0.4 // Should be 0.5 I think lowering it makes muscles strechy
#define friction   0.25
#define friction2  0.01


double getStickBallFitness(creature individual) {
    enum {planeDistance, upHill, downHill};
    int measure = environment[1];
    posi com = getCom(individual);
    posi origin = ((stickball*) individual.components)->origin;
    switch (measure) {
        case planeDistance : return euc2D(com, origin);
        case upHill        : return -sgn(environment[0]) * (com.x - origin.x);
        case downHill      : return sgn(environment[0]) * (com.x - origin.x);
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

bool updateStickBall(creature * current, int * time) {
    if (errorCheck(current)) quit(CREATURE_ERROR);
    int nodeLen   = current->genome->iData[nod];
    int muscleLen = current->genome->iData[mus];
    int boneLen   = current->genome->iData[bon];
//    int numAxons  = current->genome->iData[neu];

    double groundAngle = environment[0];
    stickball * components = ((stickball*) current->components);
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
        double recipMass = 1.0 / components->nodes[i].mass;

        double groundHeight = - (tanf(groundAngle)) * (*loc).x;
        if     ((*loc).z <= RADIUS + groundHeight) {
            if ((*loc).z <  RADIUS + groundHeight) {
                (*vel).z *= -bounceBack * sinf(groundAngle);
                (*vel).x *= -bounceBack * cosf(groundAngle);
                (*loc).z = RADIUS + groundHeight;

                (*vel).x *= friction;
                (*vel).y *= friction;
            }
        }

        (*vel).x += (*force).x * dt * recipMass;
        (*vel).y += (*force).y * dt * recipMass;
        (*vel).z += (*force).z * dt * recipMass;

        (*vel).x *= drag;
        (*vel).y *= drag;
        (*vel).z *= drag;


        (*loc).x += (*vel).x * dt;
        (*loc).y += (*vel).y * dt;
        (*loc).z += (*vel).z * dt;

        if ((*loc).z >= 100) { // If they go over this, it is very likely an error.
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

    (*time)++;
    return false;
}































