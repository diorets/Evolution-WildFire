#include "GameModes/Simulate/Cannon/cannonPhysics.h"
#include "ErrorHandle/error.h" // Quit

#include <stdio.h>
#include <math.h>

#define RADtoDEG 180.0 / 3.1415926
#define DEGtoRAD 3.1415926 / 180.0
double getCannonFitness(creature individual) {
    //printf("%f\n", individual.fitness);
	return -individual.fitness;
}

static bool error(creature * current) {

	return false;
}

static double getAngle(double v, double targetX, double * coeff, int numTerms) {
    double angle = coeff[0];
    double x = targetX;
    //puts("------\n");
    for (int i = 1; i < 4; i++) {
      //  printf("%f\n", angle);
        angle += x * coeff[i];
        x *= x;
    }
    //angle = fmod(angle, 3.14159 / 2.0);
    //printf("%f\n", angle);
   // return 0.02 * 180 / 3.14159;
   printf("%f, %f\n", 50.0, RADtoDEG *RADtoDEG*0.5 * asin(9.8 * 50 / (v*v*v)));
    return RADtoDEG * 0.5 * asin(9.8 * 50 / (v*v*v)); // outputs radians, input = ??
}

#include "Math/myMath.h"
bool updateCannon(creature * current, int * time) {
    if (error(current)) quit(CREATURE_ERROR);

    const double mass = 10;
    const double recipMass = 1.0 / mass;
    const double dt = 0.001;

    node * ball = &((cannon*) current->components)->ball;
    int * targetNum = &((cannon*) current->components)->targetNum;
    double targetX = ((cannon*) current->components)->target[*targetNum];
    double * coeff = ((cannon*) current->components)->coeff;
    double * angle = &((cannon*) current->components)->angle;


	posi * force = &(*ball).force;
    posi * vel   = &(*ball).vel;
    posi * loc   = &(*ball).loc;

    if ((*loc).z < -0.01) {
        current->fitness += fabs((*loc).x - targetX);

        double v_i = ((cannon*) current->components)->vInit;
        (*angle) = getAngle(v_i, targetX, coeff, current->genome->iData[CE::numPara]);
        (*vel) = vec(v_i * cos((*angle)), 0.0, v_i * sin((*angle)));
        (*loc) = vec(0, 0, CE::h);

        (*targetNum)++;
        (*time)++;
        return false;
    }

    /* Calculate Initial Velocity */
    if ((*time) == 0) {
        (*time) = 1;
        double v_i = ((cannon*) current->components)->vInit;
        (*angle) = getAngle(v_i, targetX, coeff, current->genome->iData[CE::numPara]);
        (*vel) = vec(v_i * cos((*angle)), 0.0, v_i * sin((*angle))); //inputs radians
    }

    (*force).z = -9.8 * mass;

    (*vel).x += (*force).x * dt * recipMass;
    (*vel).y += (*force).y * dt * recipMass;
    (*vel).z += (*force).z * dt * recipMass;

    (*loc).x += (*vel).x * dt;
    (*loc).y += (*vel).y * dt;
    (*loc).z += (*vel).z * dt;

    //(*time)++;
	return false;
}



































