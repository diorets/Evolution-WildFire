#include "GameModes/Simulate/StickBall/stickBallPhysics.h"

#include <math.h> // isinf, sinf
#include "Math/myMath.h"       // Various Vector Functions
#include "ErrorHandle/error.h" // Quit
#include "global.h" // Environment

#define PI 3.141592653589793
#define G  9.8066500
#define dt 1

#define constant   0.5




double getStickBallFitness(creature individual) {
    enum {getFromPhysics, plusX, planeDistance, upX, downX};
    int measure = getFromPhysics;//environment[1]; // Change this!

    posi com = getCom(individual);
    posi origin = ((stickball*) individual.components)->origin;
    switch (measure) {
        case getFromPhysics : return individual.fitness;
        case plusX          : return com.x - origin.x;
        case planeDistance  : return euc2D(com, origin);
        case upX            : return -sgn(ground(vec(1,0,0)) - ground(vec(0,0,0))) * (com.x - origin.x);
        case downX          : return  sgn(ground(vec(1,0,0)) - ground(vec(0,0,0))) * (com.x - origin.x);
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

double groundFunction(posi r) {
//    return 1*r.y * exp(0.002*(-r.x*r.x-r.y*r.y));
//    return -1.3*r.x;
//    return 0;
//    return 3 * cos(0.25 * r.x);
//    return 3 * sin(0.25 * r.x) * cos(0.25 * r.y);

    int numCirles = 3000;
    double *radii = (double*) malloc(sizeof(double) * numCirles); // random
    double *xs = (double*) malloc(sizeof(double) * numCirles); // random
    double *ys = (double*) malloc(sizeof(double) * numCirles); // random

    srand(0);
    for (int i = 0; i < numCirles; i++) {
        radii[i] = randf(15);
        xs[i] = pmRandf(50);
        ys[i] = pmRandf(50);
    }

    double height = 0.0;
    for (int i = 0; i < numCirles; i++) {
        double val = radii[i]*radii[i] - ((r.x - xs[i])*(r.x - xs[i]) + (r.y - ys[i])*(r.y - ys[i]));
        height += val > 0 ? val : 0;
    }

    free(radii);
    free(xs);
    free(ys);
    return height;



}

double ground(posi r) { // max range is +/- 0.5*h*len. ex: l=500, h=0.1 => x:25
    return 0;
    double k = 0.35;
    double val = (sin(k*r.x) + sin(2*k*r.x) + 0.1 * r.x);
//    double val = 0.4 *(sin(k*r.x)  + 0.1 * r.x);
//    val = val * val * val * val * val;
//    val = val * val;
//    return 2*val ;
//    return 3 * sin(k*r.x) * cos(k*r.y);
    int len = 2000;
    double h = 0.1; // precision
    bool getFromFile = true; // else generate manually

    static bool builtField = false;
    static double **field = NULL;

    /* Build Table */
    if (!builtField) {
        field = (double**) malloc(sizeof(double*) * (2*len+1));
        if (field == NULL) quit(MALLOC_ERROR);

        for (int i = 0; i < (2*len+1); i++) {
            field[i] = (double*) malloc(sizeof(double) * (2*len+1));
            if (field[i] == NULL) quit(MALLOC_ERROR);
        }

        if (getFromFile) {
            FILE* world = fopen("../assets/world.txt", "r");
            for (int i = -len; i < len + 1; i++) {
                for (int j = -len; j < len + 1; j++) {
                    char value[500];
                    fgets(value, 500, world);
                    field[i+len][j+len] = atof(value);
                }
            }
            builtField = true;
        } else {
            double minv = 10000;
            double maxv = 0;
            for (int i = -len; i < len + 1; i++) {
                double x = h*i;
                for (int j = -len; j < len + 1; j++) {
                    double y = h*j;
                    field[i+len][j+len] = groundFunction(vec(x, y, 0));
                    minv = field[i+len][j+len] < minv ? field[i+len][j+len] : minv;
                    maxv = field[i+len][j+len] > maxv ? field[i+len][j+len] : maxv;
                }
            }

            /* If random terrain, normalize */
            FILE* world = fopen("../assets/world.txt", "w");
            for (int i = -len; i < len + 1; i++) {
                for (int j = -len; j < len + 1; j++) {
                    field[i+len][j+len] = (field[i+len][j+len] - minv) / (maxv-minv);
                    field[i+len][j+len] *= field[i+len][j+len];
                    field[i+len][j+len] *= 7;
                    fprintf(world, "%f\n", field[i+len][j+len]);
                }
            }
            fclose(world);
            builtField = true;
        }
    }


    /* Table Lookup */
    if (field == NULL) quit(MALLOC_ERROR);

    if (fabs(r.x) > (len * h)) return 0;
    if (fabs(r.y) > (len * h)) return 0;

    int i_ = floor(r.x/h) + len;
    int j_ = floor(r.y/h) + len;

    if (i_ < 1 || i_ > 2*len) return field[i_][j_];
    if (j_ < 1 || j_ > 2*len) return field[i_][j_];

    /* Bi-Linear Interpolation */
//    double m = (field[i_+1][j_]-field[i_-1][j_]) / 2.0;
//    double x = (r.x/h + len - i_ + 1);
//    double val = (x + i_ - 1) * m;
//    return val / 10.0;
//    field[i_+1] - m(i_+1) *
//    printf("%f, %f, %f, %d :: %f, %f\n", floor((r.x-h)/h) + len, floor((r.x+h)/h) + len, r.x/h + len, i_, field[i_+1],field[i_-1]);
    return field[i_][j_];
}


double getLowestNode(stickball* components, int nodeLen) {
    double minZ = 30.0;
    for (int i = 0; i < nodeLen; i++) {
        posi * loc = &components->nodes[i].loc;
        minZ = (*loc).z - ground(*loc) < minZ ? (*loc).z - ground(*loc) : minZ;
    }
    return minZ;
}

double getHighestNode(stickball* components, int nodeLen) {
    double maxZ = -30.0;
    for (int i = 0; i < nodeLen; i++) {
        posi * loc = &components->nodes[i].loc;
        maxZ = (*loc).z - ground(*loc) > maxZ ? (*loc).z - ground(*loc) : maxZ;
    }
    return maxZ;
}

double getConnectionAngle(stickball * components, int c, int a, int b) {
    node * nodes = components->nodes;
    posi center = nodes[c].loc;
    posi locA = nodes[a].loc;
    posi locB = nodes[b].loc;

    double p12 = euc(center, locA);
    double p13 = euc(center, locB);
    double p23 = euc(locA, locB);

    return acos((p12 * p12 + p13 * p13 - p23 * p23) / (2 * p12 * p13));
}


bool updateStickBall(creature * current, int * time) {
//  (*time)++;  return false;
    if (errorCheck(current)) return true;//quit(CREATURE_ERROR);
    stickball * components = ((stickball*) current->components);
    int nodeLen   = current->genome->iData[nod];
    int muscleLen = current->genome->iData[mus];
    int boneLen   = current->genome->iData[bon];


    /* Lower Creature To Ground */
    if (*time == 0) {
        double minZ = getLowestNode(components, nodeLen); // find the (distance of the) node that is the lowest below the ground
        for (int i = 0; i < nodeLen; i++) {
            posi * loc = &components->nodes[i].loc;
            (*loc).z -= minZ - 0.1;// - 4 * RADIUS;
        }
    }

    double x_0 = getCom(*current).x;

    /* Node Initializtion */
    for (int i = 0; i < nodeLen; i++) {
        posi * force = &components->nodes[i].force;
        (*force)     = zero();
        (*force).z   = -G * components->nodes[i].mass / 1500.0;
    }

//    for (int i = 0; i < nodeLen; i++) {
//        /* All connections attached to a node must have an angle that is a multiple of 45 degrees */
//        // Find all connected nodes
//        int * connectedNodes = (int*) malloc(sizeof(int));
//        int numConnectedNode = 0;
//
//        for (int j = 0; j < muscleLen; j++) {
//            int a = components->muscles[j].a;
//            int b = components->muscles[j].b;
//
//            if (a == i || b == i) { // Connected
//                connectedNodes = (int*) realloc(connectedNodes, sizeof(int) * (numConnectedNode+1));
//                connectedNodes[numConnectedNode] = (a == i) ? b : a; // The other node
//                numConnectedNode++;
//            }
//        }
//        for (int j = 0; j < boneLen; j++) {
//            int a = components->bones[j].a;
//            int b = components->bones[j].b;
//
//            if (a == i || b == i) { // Connected
//                connectedNodes = (int*) realloc(connectedNodes, sizeof(int) * (numConnectedNode+1));
//                connectedNodes[numConnectedNode] = (a == i) ? b : a; // The other node
//                numConnectedNode++;
//            }
//        }
//
//        for (int j = 0; j < numConnectedNode; j++) {
//            for (int k = 0; k < numConnectedNode; k++) {
//                if (connectedNodes[j] == connectedNodes[k]) {
//                    continue;
//                }
//                double angle = getConnectionAngle(components, i, connectedNodes[j], connectedNodes[k]);
////                printf("%f\n",  180/3.1415926*(angle - (3.1415926/4) * round(angle / (3.1415926/4))));
//                // Calculate the deviation
//                double d = angle - (3.1415926/4.0) * round(angle / (3.1415926/4.0));
//                // Find the location the node needs to be at? Bit weird because path is an arc
//                d /= 100000.0;
//                double c = cos(d);
//                double s = sin(d);
//
//                posi * center = &components->nodes[i].loc;
//                posi * locA = &components->nodes[connectedNodes[j]].loc;
//                posi * locB = &components->nodes[connectedNodes[k]].loc;
//
//                posi u = cross(sub(*locA, *center), sub(*locB, *center));
//                u = normalize(u);
//                u = scale(u, -1);
//
//                *locB = sub(*locB, *center);
//                (*locB).x = (c+u.x*u.x*(1-c))     * (*locB).x +
//                            (u.x*u.y*(1-c)-u.z*s) * (*locB).y +
//                            (u.x*u.z*(1-c)+u.y*s) * (*locB).z;
//
//                (*locB).y = (u.y*u.x*(1-c)+u.z*s) * (*locB).x +
//                            (c+u.y*u.y*(1-c))     * (*locB).y +
//                            (u.y*u.z*(1-c)-u.x*s) * (*locB).z;
//
//                (*locB).z = (u.z*u.x*(1-c)-u.y*s) * (*locB).x +
//                            (u.z*u.y*(1-c)+u.x*s) * (*locB).y +
//                            (c+u.z*u.z*(1-c))     * (*locB).z;
//                *locB = add(*locB, *center);
//
//
//                c = cos(-d);
//                s = sin(-d);
//                *locA = sub(*locA, *center);
//                (*locA).x = (c+u.x*u.x*(1-c))     * (*locA).x +
//                            (u.x*u.y*(1-c)-u.z*s) * (*locA).y +
//                            (u.x*u.z*(1-c)+u.y*s) * (*locA).z;
//
//                (*locA).y = (u.y*u.x*(1-c)+u.z*s) * (*locA).x +
//                            (c+u.y*u.y*(1-c))     * (*locA).y +
//                            (u.y*u.z*(1-c)-u.x*s) * (*locA).z;
//
//                (*locA).z = (u.z*u.x*(1-c)-u.y*s) * (*locA).x +
//                            (u.z*u.y*(1-c)+u.x*s) * (*locA).y +
//                            (c+u.z*u.z*(1-c))     * (*locA).z;
//                *locA = add(*locA, *center);
//            }
//        }
//        free(connectedNodes);
//
//    }


    /* Get Muscle Forces : Find force needed to get to this frames length */
    for (int i = 0; i < muscleLen; i++) {
        int a = components->muscles[i].a;
        int b = components->muscles[i].b;
        double * currLength = &components->muscles[i].currLength;
        double orgiLength = components->muscles[i].origLength;

        double rate = components->muscles[i].rate;
        double shift = components->muscles[i].shift;

        *currLength = euc(components->nodes[a].loc, components->nodes[b].loc);
        double desiredLength = orgiLength * (1 + 0.2 * sin(0.005 * (rate * ((*time) / dt))));
        double deviation = desiredLength - *currLength;

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
            double bounce = 0.01;


            posi normalForce = scale(N, -bounce * 2 * dot(N, *vel) * mass / dt);
//            printf("%f\n", mag(*vel));
//            if (mag((*vel)) > 0.1) {
                (*force) = add(*force, normalForce);
//            }

            (*vel) = sub(*vel, scale(N, bounce * 2 * dot(N, *vel))); // normal force
            (*loc).z = RADIUS + groundHeight + 0.0001;


            posi norm = normal(ground, *loc);
            double x = norm.z / norm.x; // this might need sgn(normal) or something?
            double y = norm.z / norm.y;
            double friction = 0.5;
            (*vel).x *= (1 - friction * dt)  * (isinf(x) || (x>100)? 1 : x / sqrt(1 + x*x));
            (*vel).y *= (1 - friction * dt)  * (isinf(y) || (y>100)? 1 : y / sqrt(1 + y*y));
        }
        (*vel).x += (*force).x * dt * recipMass;
        (*vel).y += (*force).y * dt * recipMass;
        (*vel).z += (*force).z * dt * recipMass;

        double drag = 0.03;
        (*vel).x *= 1 - drag * dt;
        (*vel).y *= 1 - drag * dt;
        (*vel).z *= 1 - drag * dt;

//        (*vel).x *= 1 - mag((*vel))/100.0;
//        (*vel).y *= 1 - mag((*vel))/100.0;
//        (*vel).z *= 1 - mag((*vel))/100.0;

        (*loc).x += (*vel).x * dt;
        (*loc).y += (*vel).y * dt;
        (*loc).z += (*vel).z * dt;



        if ((*loc).z >= 300) { // If they go over this, it is very likely an error.
            return true;
        }
        if (fabs((*loc).x) >= 5000 || fabs((*loc).y) >= 5000) { // If they go over this, it is very likely an error.
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
        globalData[creatureFitnessE].g.points[numEntries] = current->fitness;// euc2D(zero(), getCom(*current));
        globalData[creatureFitnessE].g.numEntries++;
    }

    if (*time == 100) { // give time to fall over
        components->origin = getCom(*current);
    }

    components->trail[*time] = getCom(*current);

    posi com = getCom(*current);
    double x_1 = com.x;
    // Bonus for moving distance, scaled by your heigh, but deviations from x axis is heavy penalty
//    if (*time > 500) {
//        current->fitness += (x_1 - x_0) * com.z * com.z * exp(-com.y * com.y);
//    }
    // Normal 2D distance

//    current->fitness = euc2D(components->origin, com);
//    (*time)++;
//    return false;
//    if (*time == 100) {
//            current->fitness = 0;
//            components->origin = getCom(*current);
//    }
//    double minNode = getLowestNode(components, nodeLen);
//    double maxNode = getHighestNode(components, nodeLen);

    double t = *time;
    double s = 1 / 100.0;
//    double val = 0, 0.5 * cos(t*s) + cos(t*s * 3) + 1.5 + 0.5*s*t;
    double val = s*t;//2*sqrt(s*t);
    posi path = vec(t*s, 0, 2 * sqrt(s*t));
    path = scale(path, 3);
    double deviation = euc(path, com);
    current->fitness -=  deviation * deviation * deviation / 100000.0;
//    double height = com.z;// + minNode;// * (x_1 - x_0);// > 0.02? 1 : 0;// * exp(-x_1/10.0);
//    double height = (1*(com.z - components->origin.z) + 10*minNode) * (euc2D(components->origin, com) > 1 ? 10 : 1);



//    if (*time > 100) current->fitness += height > current->fitness ? height : current->fitness;
//    if (*time == 999) current->fitness *= euc2D(components->origin, com);
    (*time)++;
    return false;
}































