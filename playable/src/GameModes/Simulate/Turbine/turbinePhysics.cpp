#include "GameModes/Simulate/Turbine/turbinePhysics.h"
#include "ErrorHandle/error.h" // Quit
#include "Math/myMath.h"
#include <math.h>
#include "global.h"
#include <stdio.h>

double getTurbineFitness(creature individual) {
    printf("%+d %f\n", sgn(individual.fitness), fabs(individual.fitness));
	return fabs(individual.fitness);
}

static bool error(creature * current) {

	return false;
}


#include "Visual/basic.h"
bool updateTurbine(creature * current, int * time) {
    if (time == 0) {srand(0);}
	if (error(current)) quit(CREATURE_ERROR);
    turbine * components = ((turbine*) current->components);
    int numBlocks = current->genome->iData[TE::numBlocks];
    int numParticles = current->genome->iData[TE::numParticles];

    const double r = 0.1; // particle
    const double R = current->genome->fData[TE::radius]; // block
    const double e = 0.0; // 'Surface width'
    const double k = 1.0;

    const double Bdensity = 2800;
    const double Pdensity = 5.225;

	const double Bmass = Bdensity * 4 * 3.1415926 * R * R * 0.001;
	const double Pmass = Pdensity * 4 / 3 * 3.1415926 * r * r * r;
	const double recipPMass = 1.0 / Pmass;
	const double recipBMass = 1.0 / Bmass;
	const double dt = 0.01;


    for (int i = 0; i < numParticles; i++) {
        components->particles[i].force = zero();
    }

    double systemTorque = 0.0;
    for (int i = 0; i < numBlocks; i++) {
        block * b = &components->blocks[i];
        for (int j = 0; j < numParticles; j++) {
            node * p = &components->particles[j];
            if (euc(b->loc, p->loc) <= (R+r+e)) { // Collision
                posi radVec = sub(b->loc, p->loc);
                double force = -Pmass * (k + 1) * dot(p->vel, radVec) / mag(radVec) / dt; // v in dir of rad
                posi forceVec = scale(radVec, force / (R+r+e));

                systemTorque += dot(forceVec, b->loc) * mag(b->loc) * sgn(atan2(-p->loc.y, b->loc.x) +atan2(p->loc.y, p->loc.x));
                p->force   = add(p->force, forceVec);
            }
        }
    }

    double inertia = 0.0;
    for (int i = 0; i < numBlocks; i++) {
        inertia += mag(components->blocks[i].loc);
    }
    inertia *= Bmass;

    for (int i = 0; i < numBlocks; i++) {
        double * torque     = &components->blocks[i].torque;
        double * angSpeed   = &components->blocks[i].angSpeed;
        //double * angle      = &components->blocks[i].angle;
        posi   * loc        = &components->blocks[i].loc;
        *angSpeed += systemTorque * dt / inertia;

        double dAngle = *angSpeed * dt;
        current->fitness += dAngle;
        double x = (*loc).x;
        (*loc).x = x * cos(dAngle) - (*loc).y * sin(dAngle);
        (*loc).y = x * sin(dAngle) + (*loc).y * cos(dAngle);
        //*angle = atan2((*loc).y, (*loc).x);
    }

    for (int i = 0; i < numParticles; i++) {
        posi * force = &components->particles[i].force;
        posi * vel   = &components->particles[i].vel;
        posi * loc   = &components->particles[i].loc;

        (*vel).x += (*force).x * dt * recipPMass + pmRandf(0.1);
        (*vel).y += (*force).y * dt * recipPMass + pmRandf(0.1);
        (*vel).z += (*force).z * dt * recipPMass + pmRandf(0.1);

        (*loc).x += (*vel).x * dt;
        (*loc).y += (*vel).y * dt;
        (*loc).z += (*vel).z * dt;

        //if ((*loc).x > TE::xhigh)  (*loc).x = TE::xlow;
        if ((*loc).y < TE::ylow)  (*loc).y = TE::yhigh;
        if ((*loc).y > TE::yhigh) (*loc).y = TE::ylow;
        if ((*loc).z < TE::zlow)  (*loc).z = TE::zhigh;
        if ((*loc).z > TE::zhigh) (*loc).z = TE::zlow;

        if (((*loc).x > 20) || ((*loc).x < TE::xlow - 1)){
            *force = zero();
            //(*loc).x = TE::xlow;
            *loc = vec(TE::xlow - 1, randf2(TE::ylow, TE::yhigh), randf2(TE::zlow, TE::zhigh));
            *vel = vec(randf(40)+2, pmRandf(2), pmRandf(2));
            //*vel = vec(randf(100)+0.1, 0, 0);
        }

    }

	return false;
}






































