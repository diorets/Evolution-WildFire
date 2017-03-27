#include "physics.h"

#include <math.h> // isinf, sinf
#include "Math/myMath.h"       // Various Vector Functions
#include "ErrorHandle/error.h" // Quit
#include "global.h" // Environment

#define PI 3.141592653589793
#define G  9.8066500

#define dt 1

#define MUSCLEMACRO(i) 1.3 * sinf((float) (time * (10 - i)) * 0.005)

#define bounceBack 0.00
#define drag       0.99
#define constant   0.4 // Should be 0.5 I think lowering it makes muscles strechy
#define friction   0.25
#define friction2  0.01



static bool errorCheck(creature * current) {
    int len = current->genome[inf].iData[nod];
    for (int i = 0; i < len; i++) {
        posi loc = current->nodes[i].loc;
        if (mag(loc) > 100000) return true;
        if (isinf(loc.x) || isinf(loc.y) || isinf(loc.z)){
            return true;
        }
    }
    return false;
}


#include "Functional/neuralNet.h"
bool updateCreature(creature * current, int time) {
//    return false;
    if (errorCheck(current)) quit(CREATURE_ERROR);
    int nodeLen   = current->genome[inf].iData[nod];
    int muscleLen = current->genome[inf].iData[mus];
    int boneLen   = current->genome[inf].iData[bon];
//    int numAxons  = current->genome[inf].iData[neu];

    double groundAngle = environment[0];

    /* Node Initializtion */
    for (int i = 0; i < nodeLen; i++) {
        posi * force = &current->nodes[i].force;
        (*force)     = zero();
        (*force).z   = -G * current->nodes[i].mass / 1500.0;
    }

//    static double * inputs = NULL;
//    static double *** weights = NULL;
//    if (simTime == 0) {
//        /* Get length change */
//        inputs = (double*) malloc(sizeof(double) * muscleLen);
//        weights = (double***) malloc(sizeof(double**) * 2);
//        for (int i = 0; i < 2; i++) {
//            weights[i] = (double**) malloc(sizeof(double*) * 100);
//            for (int j = 0; j < 100; j++) {
//                weights[i][j] = (double*) malloc(sizeof(double) * 100);
//            }
//        }
//
//        for (int i = 0; i < numAxons; i++) {
//             numAxons is probably wrong
//            int a      = current->axons[i].a;
//            int b      = current->axons[i].b;
//            int layer  = current->axons[i].layer;
//            int weight = current->axons[i].weight;
//            if (layer > 1) exit(__LINE__);
//            if (a > 100) exit(__LINE__);
//            if (b > 100) exit(__LINE__);
//            if (fabs(weight > 1000)) exit(__LINE__);
//
//            weights[layer][a][b] = weight;
//        }
//    }
//
//
//
//    for (int i = 0; i < muscleLen; i++) {
//        inputs[i] = specimen[id].muscles[i].currLength / specimen[id].muscles[i].origLength - 1;
//        inputs[i] /= 1.5 / 100.0;
//    }
//    double * outputs = runNetwork(inputs, weights, muscleLen); // crashes here
//
//    if (simTime == 9000) {
//        free(inputs);
//        for (int i = 0; i < 2; i++) {
//            for (int j = 0; j < 100; j++) {
//               free(weights[i][j]);
//            }
//            free(weights[i]);
//        }
//        free(weights);
//    }


    /* Get Muscle Forces : Find force needed to get to this frames length */
    for (int i = 0; i <  muscleLen; i++) {
        int a = current->muscles[i].a;
        int b = current->muscles[i].b;
        double * currLength = &current->muscles[i].currLength;
        *currLength = euc(current->nodes[a].loc, current->nodes[b].loc);
        *currLength += MUSCLEMACRO(i);
//        *currLength += 3 * outputs[i];
        double deviation = current->muscles[i].origLength - *currLength;
        posi radVector = sub(current->nodes[a].loc, current->nodes[b].loc);
        //if (mag(radVector) < 0.01) continue; // idk if I need this

        posi deltaFor = scale(radVector, (deviation * constant) / (mag(radVector) * dt * dt));

        current->nodes[a].force = add(current->nodes[a].force, scale(deltaFor, current->nodes[a].mass));
        current->nodes[b].force = sub(current->nodes[b].force, scale(deltaFor, current->nodes[b].mass));
    }
//    free(outputs);



    /* Get Bone Forces */
    for (int i = 0; i < boneLen; i++) {
        int a = current->bones[i].a;
        int b = current->bones[i].b;
        double deviation = current->bones[i].length - euc(current->nodes[a].loc, current->nodes[b].loc);

        posi radVector = sub(current->nodes[a].loc, current->nodes[b].loc);
       // if (mag(radVector) < 0.01) continue;

        posi deltaAcc = scale(radVector, (deviation * constant) / (mag(radVector) * dt * dt));

        current->nodes[a].force = add(current->nodes[a].force, scale(deltaAcc, current->nodes[a].mass));
        current->nodes[b].force = sub(current->nodes[b].force, scale(deltaAcc, current->nodes[b].mass));
    }

    /* Friction, Collision and Apply Forces */
    for (int i = 0; i < nodeLen; i ++) {
        posi * force = &current->nodes[i].force;
        posi * vel   = &current->nodes[i].vel;
        posi * loc   = &current->nodes[i].loc;
        double recipMass = 1.0 / current->nodes[i].mass;

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
    return false;
}



/* Different Friction Models &*/
//            (*force).x *= friction;
//            (*force).y *= friction;

//            (*vel).x -= sgn((*vel).x) * friction2;
//            (*vel).y -= sgn((*vel).y) * friction2;



//            double u = 0.0006;
//            double X = (*loc).y / (*loc).x;
//            double cosX = 1.0 / sqrtf(1.0 + (X * X));
//
//            posi fricForce;
//            fricForce.x = u * -sgn((*vel).x) * G;
//            fricForce.y = u * -sgn((*vel).y) * G;
//
//            if (fabs((*vel).x + (fricForce.x * dt)) > 0) {
//                (*force).x += fricForce.x * cosX;
//            }
//            if (fabs((*vel).y + (fricForce.y * dt * recipMass)) > 0) {
//                (*force).y += fricForce.y * X*cosX;
//            }


































































































//
//static void resetForce(creature * current) {
//    int len = current->genome[inf].iData[nod];
//    for (int i = 0; i < len; i++) {
//        current->nodes[i].force = zero();
//    }
//    return;
//}
//
//static void gravity(creature * current) {
//    int len = current->genome[inf].iData[nod];
//    for (int i = 0; i < len; i++) {
//        current->nodes[i].force.z += -G * current->nodes[i].mass;
//    }
//    return;
//}
//
////void friction(posi * force, posi * vel, posi * loc, double height, double mass) {
////    if (height <= 0.05 + 0.75) { // Temp Pseudo-Friction
////        (*vel).x *= 0.99;
////        (*vel).y *= 0.99;
//////        double u = 0.6;
//////        double X = (*loc).y / (*loc).x;
//////        double cosX = 1.0 / sqrtf(1.0 + (X * X));
//////
//////        posi fricForce;
//////        fricForce.x = u * -sgn((*vel).x) * mass * G;
//////        fricForce.y = u * -sgn((*vel).y) * mass * G;
//////
//////        if (fabs((*vel).x + (fricForce.x * dt * (1/mass))) > 0) {
//////            (*force).x += fricForce.x * (1/mass) * cosX;
//////        }
//////        if (fabs((*vel).y + (fricForce.y * dt * (1/mass))) > 0) {
//////            (*force).y += fricForce.y * (1/mass) * X*cosX;
//////        }
////    }
////    return;
////}
//
//void heatLoss(creature * current) {
//    int len = current->genome->iData[nod];
//    for (int i = 0; i < len; i++) {
//        current->nodes[i].vel.x *= 0.99995;
//        current->nodes[i].vel.y *= 0.99995;
//        current->nodes[i].vel.z *= 0.99995;
//    }
//    return;
//}
//
//static void applyForce(creature * current) {
//    int len = current->genome[inf].iData[nod];
//    heatLoss   (current);
//    for (int i = 0; i < len; i++) {
//        posi *force = &current->nodes[i].force;
//        posi *vel   = &current->nodes[i].vel;
//        posi *loc   = &current->nodes[i].loc;
//        double recipMass = 1.0 / current->nodes[i].mass;
//
//        //friction(force, vel, loc, (*loc).z, 1/ recipMass);
//
//        (*vel).x += (*force).x * dt * recipMass;
//        (*vel).y += (*force).y * dt * recipMass;
//        (*vel).z += (*force).z * dt * recipMass;
//
////        (*vel).x += (*force).x * dt;
////        (*vel).y += (*force).y * dt;
////        (*vel).z += (*force).z * dt;
//
//
//        (*loc).x += (*vel).x * dt;
//        (*loc).y += (*vel).y * dt;
//        (*loc).z += (*vel).z * dt;
//    }
//    return;
//}
//
//static void collision(creature * current) {
//    int len = current->genome[inf].iData[nod];
//    for (int i = 0; i < len; i ++) {
//        if (current->nodes[i].loc.z <= 0.75) {
//            current->nodes[i].vel.z *= -0.3;
//            current->nodes[i].loc.z  = 0.75001;
//        }
//    }
//    return;
//}
//
//static void boneRigid(creature * current) {
//    int len = current->genome[inf].iData[bon];
//    for (int i = 0; i < len; i++) {
//        int a = current->bones[i].a;
//        int b = current->bones[i].b;
//        double massA = current->nodes[a].mass;
//        double massB = current->nodes[b].mass;
//
//        double cLength = euc(current->nodes[a].loc, current->nodes[b].loc);
//        double fLength = current->bones[i].length;
//        double deviation = fLength - cLength;
//
//        posi radVector = sub(current->nodes[a].loc, current->nodes[b].loc);
//        if (mag(radVector) < 0.01) continue;
//
//        posi radUnitVector = scale(radVector, 1 / mag(radVector));
//
//        posi deltaPos = scale(radUnitVector, deviation * 0.5);
//        posi deltaVel = scale(deltaPos, 1 / dt);
//        posi deltaAcc = scale(deltaVel, 1 / dt);
//
////        current->nodes[a].loc   = add(current->nodes[a].loc, deltaPos);
////        current->nodes[b].loc   = sub(current->nodes[b].loc, deltaPos);
//
////        current->nodes[a].vel   = add(current->nodes[a].vel, deltaVel);
////        current->nodes[b].vel   = sub(current->nodes[b].vel, deltaVel);
//
//        current->nodes[a].force = add(current->nodes[a].force, scale(deltaAcc, massA));
//        current->nodes[b].force = sub(current->nodes[b].force, scale(deltaAcc, massB));
//    }
//    return;
//}
//
//#include <time.h>
//static void muscleRigid(creature * current, int simTime) {
//    int len = current->genome[inf].iData[mus];
//    for (int i = 0; i < len; i++) {
//        //*currLength *= (sinf((simTime * 0.2 * dt * i)) * 0.2 + 1);
//        int a = current->muscles[i].a;
//        int b = current->muscles[i].b;
//        double * currLength = &current->muscles[i].currLength;
//
//        // Calc Deviation
//        *currLength = euc(current->nodes[a].loc, current->nodes[b].loc);
//        //double lengthToBe = *currLength + 0.0001;
//
//
//        //double deviation = lengthToBe - *currLength;
//        int cap = 30000; // bigger -> quicker
//        double deviation = -0.0001;
//        if ((simTime + cap / 10 * i) % cap > cap * 0.5) {
//            deviation *= -1;
//        }
//        //*currLength += 0.5 * sinf((float) simTime / 30000000000) + 1;
//        *currLength += sinf((float) (simTime + i * 10) / 10000);
//        deviation = current->muscles[i].origLength - *currLength;
//
//
//
//        // Calc unit vector
//        posi radVector = sub(current->nodes[a].loc, current->nodes[b].loc);
//        if (mag(radVector) < 0.01) continue;
//        posi radUnitVector = scale(radVector, 1 / mag(radVector));
//
//        // Calc and apply velocity
//        posi deltaPos = scale(radUnitVector, deviation * 0.5); // idk why 0.05 works...
//        posi deltaVel = scale(deltaPos, 1 / dt);
//        posi deltaFor = scale(deltaVel, 1 / dt);
////
////        current->nodes[a].loc   = add(current->nodes[a].loc,   deltaPos);
////        current->nodes[b].loc   = sub(current->nodes[b].loc,   deltaPos);
//
////        current->nodes[a].vel   = add(current->nodes[a].vel,   deltaVel);
////        current->nodes[b].vel   = sub(current->nodes[b].vel,   deltaVel);
//
//        current->nodes[a].force = add(current->nodes[a].force, scale(deltaFor, current->nodes[a].mass));
//        current->nodes[b].force = sub(current->nodes[b].force, scale(deltaFor, current->nodes[b].mass));
//
////        current->nodes[a].force = add(current->nodes[a].force, deltaFor);
////        current->nodes[b].force = sub(current->nodes[b].force, deltaFor);
//
//
////        current->nodes[a].force = scale(deltaFor,  current->nodes[a].mass);
////        current->nodes[b].force = scale(deltaFor, -current->nodes[b].mass);
//    }
//    return;
//}
//
//static void friction1(creature * current) {
//    int len = current->genome[inf].iData[nod];
//    double u = 0.3;
//
//    for (int i = 0; i < len; i++) {
//        posi *force = &current->nodes[i].force;
//        posi *vel   = &current->nodes[i].vel;
//        posi *loc   = &current->nodes[i].loc;
//
//        (*force).x = 0.0;
//        (*force).y = 0.0;
//        (*force).z = 0.0;
//        /* On Ground */
//        if ((*loc).z <= 0.01 + 0.75) {
//            posi fricForce;
//            double X = (*loc).y / (*loc).x;
//            double cosX = 1.0 / sqrtf(1.0 + (X * X));
//
//            fricForce.x = u * -sgn((*vel).x) * 1.0 * G;
//            fricForce.y = u * -sgn((*vel).y) * 1.0 * G;
//
//            if (fabs((*vel).x + (fricForce.x * dt * 1/1.0)) > 0) {
//                (*force).x = fricForce.x * 1/1.0 * cosX;
//            }
//            if (fabs((*vel).y + (fricForce.y * dt * 1/1.0)) > 0) {
//                (*force).y = fricForce.y * 1/1.0 * X*cosX;
//            }
//
//            (*vel).x += (*force).x * dt;
//            (*vel).y += (*force).y * dt;
//
//            (*loc).x += (*vel).x * dt;
//            (*loc).y += (*vel).y * dt;
//        }
//    }
//}
//
//static void drag(creature * current) {
////    double c = 0.5;
//    int len = current->genome[inf].iData[nod];
//    for (int i = 0; i < len; i++) {
////            /*
////        current->nodes[i].vel.x -= c * current->nodes[i].vel.x * dt; // Linear approximation to drag
////        current->nodes[i].vel.y -= c * current->nodes[i].vel.y * dt;
////        current->nodes[i].vel.z -= c * current->nodes[i].vel.z * dt;
////
////        current->nodes[i].loc.x += current->nodes[i].vel.x * dt;
////        current->nodes[i].loc.y += current->nodes[i].vel.y * dt;
////        current->nodes[i].loc.z += current->nodes[i].vel.z * dt;
////        */
////        current->nodes[i].vel.x *= 0.9999;
////        current->nodes[i].vel.y *= 0.9999;
////        current->nodes[i].vel.z *= 0.9999;
//    }
//    return;
//}
//
