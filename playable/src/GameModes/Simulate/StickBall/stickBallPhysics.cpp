#include "GameModes/Simulate/StickBall/stickBallPhysics.h"

#include <math.h> // isinf, sinf
#include "Math/myMath.h"       // Various Vector Functions
#include "ErrorHandle/error.h" // Quit
#include "global.h" // Environment

#define PI 3.141592653589793
#define G  9.8066500
#define dt 1

#define constant   0.4




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

double randomTerrain(posi p) {
//    return 1*r.y * exp(0.002*(-r.x*r.x-r.y*r.y));
//    return -1.3*r.x;
//    return 0;
//    return 3 * cos(0.25 * r.x);
//    return 3 * sin(0.25 * r.x) * cos(0.25 * r.y);

    int numCirles = 1000;

    srand(0);
    double height = 0.0;
    for (int i = 0; i < numCirles; i++) {
        double r = randf(45);
        double x = pmRandf(200);
        double y = pmRandf(200);
        double val = r*r - ((p.x - x)*(p.x - x) + (p.y - y)*(p.y - y));
        height += val > 0 ? val : 0;
    }
    return height;



}

//https://helloacm.com/cc-function-to-compute-the-bilinear-interpolation/     BLESS!!
inline float
BilinearInterpolation(float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2, float x, float y)
{
    float x2x1, y2y1, x2x, y2y, yy1, xx1;
    x2x1 = x2 - x1;
    y2y1 = y2 - y1;
    x2x = x2 - x;
    y2y = y2 - y;
    yy1 = y - y1;
    xx1 = x - x1;
    return 1.0 / (x2x1 * y2y1) * (
        q11 * x2x * y2y +
        q21 * xx1 * y2y +
        q12 * x2x * yy1 +
        q22 * xx1 * yy1
    );
}

static int SEED = 0;

static int hash[] = {208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
                     185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
                     9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
                     70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
                     203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
                     164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
                     228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
                     232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
                     193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
                     101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
                     135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
                     114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219};

int noise2(int x, int y)
{
    int tmp = hash[(y + SEED) % 256];
    return hash[(tmp + x) % 256];
}

float lin_inter(float x, float y, float s)
{
    return x + s * (y-x);
}

float smooth_inter(float x, float y, float s)
{
    return lin_inter(x, y, s * s * (3-2*s));
}

float noise2d(float x, float y)
{
    int x_int = x;
    int y_int = y;
    float x_frac = x - x_int;
    float y_frac = y - y_int;
    int s = noise2(x_int, y_int);
    int t = noise2(x_int+1, y_int);
    int u = noise2(x_int, y_int+1);
    int v = noise2(x_int+1, y_int+1);
    float low = smooth_inter(s, t, x_frac);
    float high = smooth_inter(u, v, x_frac);
    return smooth_inter(low, high, y_frac);
}

float perlin2d(float x, float y, float freq, int depth)
{
    float xa = x*freq;
    float ya = y*freq;
    float amp = 1.0;
    float fin = 0;
    float div = 0.0;

    int i;
    for(i=0; i<depth; i++)
    {
        div += 256 * amp;
        fin += noise2d(xa, ya) * amp;
        amp /= 2;
        xa *= 2;
        ya *= 2;
    }

    return fin/div;
}

double ground(posi r) { // max range is +/- 0.5*h*len. ex: l=500, h=0.1 => x:25
    return 0;
    return 20 * perlin2d(fabs(r.x/2.0), fabs(r.y/2.0), 0.05, 30);
//    return 0;
    double k = 0.05;
    double val = 10*(sin(k*r.x) + sin(2*k*r.y) + 0.1 * k * r.x);
    double x = 0.1 * r.x;
//    val = 5*atan(pow(x, sin(x*sin(x))));
//    return val;
//    double val = 0.4 *(sin(k*r.x)  + 0.1 * r.x);
//    val = val * val * val * val * val;
//    val = val * val;
//    return 2*val ;
//    return 3 * sin(k*r.x) * cos(k*r.y);
    int len = 200;
    double h = 1; // precision
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
                    field[i+len][j+len] = randomTerrain(vec(x, y, 0));
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
                    field[i+len][j+len] *= 30;
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

    if (i_ < 10 || i_ > 2*len-10) return 0;
    if (j_ < 10 || j_ > 2*len-10) return 0;


    if (i_ < 1 || i_ > 2*len) return field[i_][j_];
    if (j_ < 1 || j_ > 2*len) return field[i_][j_];

    /* Bi-Linear Interpolation */
    double Q11 = field[i_][j_];   // r.x
    double Q21 = field[i_+1][j_]; // r.x+h
    double Q12 = field[i_][j_+1];
    double Q22 = field[i_+1][j_+1];

    return BilinearInterpolation(Q11, Q12, Q21, Q22,
                                 h*floor(r.x/h), h*floor(r.x/h)+h,
                                 h*floor(r.y/h), h*floor(r.y/h)+h,
                                 r.x, r.y);
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
    bool lowerCreature = true;
    bool gravity = true;
    double drag = 0.03; // 0.005 < d < 0.05
    double frictionLayer = 0.0;
    double friction = 0.5; // 0.1 < f < 0.5
    double bounce = 0.01; // basically keep this between 0.2-0.4


//  (*time)++;  return false;

    if (errorCheck(current)) return true;//quit(CREATURE_ERROR);
    stickball * components = ((stickball*) current->components);
    int nodeLen   = current->genome->iData[nod];
    int muscleLen = current->genome->iData[mus];
    int boneLen   = current->genome->iData[bon];


    /* Lower Creature To Ground */
    if (lowerCreature) {
        if (*time == 0) {
            double minZ = getLowestNode(components, nodeLen); // find the (distance of the) node that is the lowest below the ground
            for (int i = 0; i < nodeLen; i++) {
                posi * loc = &components->nodes[i].loc;
                (*loc).z -= minZ - 0.1;// - 4 * RADIUS;
            }
        }
    }

    double x_0 = getCom(*current).x;

    /* Node Initializtion */
    for (int i = 0; i < nodeLen; i++) {
        posi * force = &components->nodes[i].force;
        (*force)     = zero();
        (*force).z   = gravity ? -G * components->nodes[i].mass / 1500.0 : (*force).z;
    }

    /* Get Muscle Forces : Find force needed to get to this frames length */
    for (int i = 0; i < muscleLen; i++) {
        int a = components->muscles[i].a;
        int b = components->muscles[i].b;
        double * currLength = &components->muscles[i].currLength;
        double orgiLength = components->muscles[i].origLength;

        double rate = components->muscles[i].rate;
        double shift = components->muscles[i].shift;

        *currLength = euc(components->nodes[a].loc, components->nodes[b].loc);
        double desiredLength = orgiLength * (1 + 0.2 * sin(0.005 * (rate * ((*time) / dt)))); // where is the shift??
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
        if ((*loc).z <= RADIUS + groundHeight + frictionLayer) {
//            if ((*loc).z <=  RADIUS + groundHeight) {
//                posi N = normal(ground, *loc);

//                posi normalForce = scale(N, -bounce * 2 * dot(N, *vel) * mass / dt);
//                (*force) = add(*force, normalForce);

//                (*vel) = sub(*vel, scale(N, bounce * 2 * dot(N, *vel))); // normal force

                (*vel).z *= -bounce;
                (*loc).z = RADIUS + groundHeight;// + 0.0001;
//            }

//            posi norm = normal(ground, *loc);
//            double x = norm.z / norm.x; // this might need sgn(normal) or something?
//            double y = norm.z / norm.y;
            (*vel).x *= (1 - friction * dt) ;// * (isinf(x) || (x>100)? 1 : x / sqrt(1 + x*x));
            (*vel).y *= (1 - friction * dt) ;// * (isinf(y) || (y>100)? 1 : y / sqrt(1 + y*y));
        }
        (*vel).x *= 1 - drag * dt;
        (*vel).y *= 1 - drag * dt;
        (*vel).z *= 1 - drag * dt;

        (*vel).x += (*force).x * dt * recipMass;
        (*vel).y += (*force).y * dt * recipMass;
        (*vel).z += (*force).z * dt * recipMass;


//        (*vel).x *= 1 - mag((*vel))/50.0;
//        (*vel).y *= 1 - mag((*vel))/50.0;
//        (*vel).z *= 1 - mag((*vel))/50.0;

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

//    double t = *time;
//    double s = 1 / 100.0;
////    double val = 0, 0.5 * cos(t*s) + cos(t*s * 3) + 1.5 + 0.5*s*t;
//    double val = s*t;//2*sqrt(s*t);
//    posi path = vec(t*s, 0, 2);
//    path = scale(path, 3);
//    double deviation = euc(path, com);
//    current->fitness -=  deviation / 1000.0;


    current->fitness = euc2D(components->origin, com);// sgn(com.x) * exp(- fabs(com.y) / 100.0);


//    double height = com.z;// + minNode;// * (x_1 - x_0);// > 0.02? 1 : 0;// * exp(-x_1/10.0);
//    double height = (1*(com.z - components->origin.z) + 10*minNode) * (euc2D(components->origin, com) > 1 ? 10 : 1);



//    if (*time > 100) current->fitness += height > current->fitness ? height : current->fitness;
//    if (*time == 999) current->fitness *= euc2D(components->origin, com);
    (*time)++;
    return false;
}







//    for (int i = 0; i < nodeLen; i++) { // to stop rotation
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


























