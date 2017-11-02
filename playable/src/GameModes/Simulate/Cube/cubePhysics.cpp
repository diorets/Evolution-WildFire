#include "GameModes/Simulate/Cube/cubePhysics.h"
#include "ErrorHandle/error.h" // Quit
#include "Math/myMath.h"
#include <math.h>
#include "global.h"
#include <stdio.h>

double getCubeFitness(creature individual) {
    printf("%+d %f\n", sgn(individual.fitness), fabs(individual.fitness));
	return fabs(individual.fitness);
}

static bool error(creature * current) {
    if (current == NULL)  return false; // WS
	return false;
}

#define PI 3.141592653589793
#define G  9.8066500

#define dt 0.1

#define MUSCLEMACRO(i) 1.3 * sinf((*time) * 0.0025)

#define bounceBack 0.5
#define drag       0.999
#define constant   0.4 // Should be 0.5 I think lowering it makes muscles strechy
#define friction   0.25

#include "Visual/basic.h"
bool updateCube(creature * current, int * time) {
    if ((*time) == 0) {srand(0);}
	if (error(current)) quit(CREATURE_ERROR);

    int numNodes  = current->genome->iData[TE::numBlocks];
    int numMuscles = current->genome->iData[CUE::numSprings];

    double groundAngle = environment[0];
    cube * components = ((cube*) current->components);

    /* Node Initializtion */
    for (int i = 0; i < numNodes; i++) {
        posi * force = &components->blocks[i].force;
        (*force)     = zero();
        (*force).z   = -G  * components->blocks[i].mass / 1500.0;
    }


    /* Get Muscle Forces : Find force needed to get to this frames length */
    for (int i = 0; i < numMuscles; i++) {
        int a = components->springs[i].a;
        int b = components->springs[i].b;
        if (a == b) continue;
//        printf("%d, %d\n", a, b);
        double * currLength = &components->springs[i].currLength;

        *currLength = euc(components->blocks[a].loc, components->blocks[b].loc);

        *currLength += MUSCLEMACRO(i);
        double deviation = components->springs[i].origLength - *currLength;
//        printf("%d, %d, %f, %f\n", a, b, components->springs[i].origLength, *currLength, deviation);
        posi radVector = sub(components->blocks[a].loc, components->blocks[b].loc);
        // if (mag(radVector) < 0.01) continue; // idk if I need this

        posi deltaFor = scale(radVector, (deviation * constant) / (mag(radVector) * dt * dt));

        components->blocks[a].force = add(components->blocks[a].force, scale(deltaFor, components->blocks[a].mass));
        components->blocks[b].force = sub(components->blocks[b].force, scale(deltaFor, components->blocks[b].mass));
    }

    /* Friction, Collision and Apply Forces */
    for (int i = 0; i < numNodes; i ++) {
        posi * force = &components->blocks[i].force;
        posi * vel   = &components->blocks[i].vel;
        posi * loc   = &components->blocks[i].loc;
        double recipMass = 1.0 / components->blocks[i].mass;

        double groundHeight = - (tanf(groundAngle)) * (*loc).x;
        if  ((*loc).z < RADIUS + groundHeight) {
            (*vel).z *= -bounceBack;
            (*loc).z = RADIUS + groundHeight;

            (*vel).x *= friction;
            (*vel).y *= friction;
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

        if ((*loc).z >= 200) { // If they go over this, it is very likely an error.
            return true;
        }
    }

    (*time)++;
	return false;
}






































