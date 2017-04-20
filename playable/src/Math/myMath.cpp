#include <math.h> // Sqrtf
#include "Math/myMath.h"

/* Vectors */
    /* Declarations */
posi zero() {
    posi v;
    v.x = 0.0;
    v.y = 0.0;
    v.z = 0.0;
    return v;
}

posi vec(double x, double y, double z) {
    posi temp;
    temp.x = x;
    temp.y = y;
    temp.z = z;
    return temp;
}

    /* Basic Operations */
double euc(posi a, posi b) {
    double x = (a.x - b.x);
    double y = (a.y - b.y);
    double z = (a.z - b.z);
    double square = (x * x) + (y * y) + (z * z);
    return sqrtf(square);
}

double euc2D(posi a, posi b) { // Excludes z
    double x = (a.x - b.x);
    double y = (a.y - b.y);
    double square = (x * x) + (y * y);
    return sqrtf(square);
}

posi sub(posi a, posi b) {
    posi result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

posi add(posi a, posi b) {
    posi result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

posi add(posi vec, double k) {
    vec.x += k;
    vec.y += k;
    vec.z += k;
    return vec;
}

posi scale(posi a, double scale) {
    a.x *= scale;
    a.y *= scale;
    a.z *= scale;
    return a;
}

bool equals(posi a, posi b, double eps) {
    return euc(a, b) < eps;
}

double mag(posi a) {
    double square = (a.x * a.x) + (a.y * a.y) + (a.z * a.z);
    return sqrtf(square);
}

    /* Advanced / Specific Operation */
posi radForce(posi a, posi b, double force) {
    return scale(sub(a, b), force);
}

posi cross(posi a, posi b) { // This has not been verified
    return vec(a.y * b.z - a.z * b.y,
               a.z * b.x - a.x * b.z,
               a.x * b.y - a.y * b.x);
}

double dot(posi a, posi b) { // This has not been verified
    return a.x * b.x + a.y * b.y + a.z * b.z;
}


/* Random Numbers */
bool chance(int x) {
    if (rand() % 100 < x) return true;
    return false;
}

bool chance(double x) {
    if (randf(100) < x) return true;
    return false;
}

double randf2(double a, double b) {
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}

double randf2(int a, int b) {
    double random = ((double) rand()) / (double) RAND_MAX;
    double diff = b - a;
    double r = random * diff;
    return a + r;
}

// Random float [0, number]
double randf(double number) {
    return (double)rand()/(double)(RAND_MAX/number);
}

// Ramdom float [-number, +number]
double pmRandf(double number) {
    return 2 * (randf(number) - (number / 2.0));
}

/* Math Functions */
int fact(int n) {
    int result = 1;
    for (int c = 1; c <= n; c++) {
        result = result * c;
    }
    return result;
}

int comb(int n) {
    return 0.5 * n * (n - 1);
}

int sgn(double x) {
    return (x > 0) - (x < 0);
}

/* Other */
    /* Averages */
posi getCom(creature individual) {
    node * masses = ((stickball*) individual.components)->nodes;
    int numMasses = individual.genome->iData[nod];
    double totMass = 0.0;
    posi com;
    com.x = 0.0;
    com.y = 0.0;
    com.z = 0.0;

    for (int i = 0; i < numMasses; i++) {
        node toAdd = masses[i];
        com.x += toAdd.loc.x * toAdd.mass;
        com.y += toAdd.loc.y * toAdd.mass;
        com.z += toAdd.loc.z * toAdd.mass;
        totMass += toAdd.mass;
    }
    com.x /= totMass;
    com.y /= totMass;
    com.z /= totMass;
    return com;
}

posi getAvgNodeVel(creature current) {
    int num = current.genome->iData[nod];
    node * nodes = ((stickball*) current.components)->nodes;
    posi vel;
    vel.x = 0.0;
    vel.y = 0.0;
    vel.z = 0.0;
    for (int i = 0; i < num; i++) {
        node toAdd = nodes[i];
        vel.x += toAdd.vel.x;
        vel.y += toAdd.vel.y;
        vel.z += toAdd.vel.z;
    }
    return vel;
}





