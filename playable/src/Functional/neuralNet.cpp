#include <math.h>
#include <stdlib.h>

#include "Functional/neuralNet.h"
#include "preproccessor.h"
double sigmoid(double input);
double sigmoidDerivative(double input);
double getInputLayerOutput(double * input, double * weight, int numInputs);
double * getLayerOutputs(double * inputs, double ** weights, int numInputs, int numOutputs);



/* Mathimatical Definitions */
double sigmoid(double input) {
    return 1.0 / (1.0 + exp(-input));
}

double sigmoidDerivative(double input) {
    double sigmoidInput = sigmoid(input);
    return sigmoidInput * (1.0 - sigmoidInput);
}

/******************
    Input layer:
    Hidden layer j:  output_j = sigmoid(sum(weights_i_j * inputs_i_j)
    output
*******************/

double getInputLayerOutput(double * input, double * weight, int numInputs) {
    double output = 0.0;
    for (int j = 0; j < numInputs; j++) {
        output += input[j] * weight[j];
    }
    return sigmoid(output);
}

double * getLayerOutputs(double * inputs, double ** weights, int numInputs, int numOutputs) {
    double * layerOutputs = (double*) malloc(sizeof(double) * numOutputs);
    for (int i = 0; i < numOutputs; i++) {
        layerOutputs[i] = 0.0;
    }

    for (int i = 0; i < numOutputs; i++) {
        layerOutputs[i] = getInputLayerOutput(inputs, weights[i], numInputs);
    }
    return layerOutputs;
}

// weights[layer][connA][conn]
double * runNetwork(double * inputs, double *** weights, int numInputs) {
    /* I_0, I_1, I_2 :: W_00, W_01, W_02, W_10, ... W_22 */
    /* 123, 252, 124 :: 0.45, 0.99, -0.1, 0.12, ... 0.23 */
    int numOutputs = HIDDEN_LAYER_SIZE; // 4 hidden layer neurons
    double * hiddenLayerOutput = getLayerOutputs(           inputs, weights[0], numInputs, numOutputs);
    double * finalOutput       = getLayerOutputs(hiddenLayerOutput, weights[1], numOutputs, numInputs);

    return finalOutput;
}
















































