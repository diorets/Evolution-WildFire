#include <stdio.h>
#include <math.h>
#include <stdlib.h>
double power = -1; // is overwritten, shouldnt this be a static variable?

double uniform() { // [0, 1]
    return (double)rand()/(double)(RAND_MAX);
}

double squarify(double (*f) (double), double x, double stepSize) {
    for (double i = stepSize; i < 1.001; i += stepSize) {
        if (i >= x) {
            double xEval = i - stepSize * 0.5;
            return f(xEval); // norm
        }
    }
    return -1;
}

double getArea(double (*f) (double), double stepSize, int square) {
    double area = 0.0;
    double xEval = square * 0;
    for (double i = stepSize; i < 1.001; i += stepSize) {
        xEval = i - stepSize * 0.5;
        area += squarify(f, xEval, stepSize) * stepSize;
    }
    return area;
}

double squareNorm(double (*f) (double), double x, double stepSize) {
    static double area = getArea(f, stepSize, 1);
    static double prevSelectionPower = power;
    if (fabs(prevSelectionPower - power) > 0.00001) { // different
        prevSelectionPower = power;
        area = getArea(f, stepSize, 1); // Only calculates once in program lifetime
    }
    return squarify(f, x, stepSize) / area;
}

double inverseIntegral(double desiredArea, double totSamples, double (*f) (double), double stepSize) {
    double area = 0;
    double dx = 1.0 / totSamples;
    for (double i = 0; i < 1.0; i += dx) { // from 0 to 1
        area += squareNorm(f, i, stepSize) * dx;
        if (area >= desiredArea) {
            return i;
        }
    }
    return 1.0 - 1.0 / totSamples;
}

int getDistribution(double (*f) (double), double popSize, double fPara) {
    power = fPara;
    double divs = 100.0;
    double stepSize = 1.0 / divs;
    return (int) (popSize * inverseIntegral(uniform(), popSize, f, stepSize));
}













