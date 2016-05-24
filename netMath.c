#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "net.h"

void feed(float input[], int numbLayers, int layerIndex[], nn net, float **workingArray, float answer[]) {
	int layer, neuron, j;
	float sum;

	// working matrix defined as [layer number][neuron number]
	// weights defined as [layer number][neuron number][neuron connection to layer behind]
	// where layer 0=input, neuron 0=first neuron

	// load input onto matrix at input layer (0)
	for (neuron = 0; neuron < layerIndex[0]; neuron++)
		workingArray[0][neuron] = input[neuron];
	// insert dummy neuron of 1 for the bias
	workingArray[0][layerIndex[0]] = 1.0;

	// calculate array starting from first layer after input, moving forwards through layers
	for (layer = 1; layer < numbLayers; layer++) {
		// calculate value for each neuron
		for (neuron = 0; neuron < layerIndex[layer]; neuron++) {
			sum = 0;
			for (j = 0; j < (layerIndex[layer-1]); j++)
				sum += (net.weights[layer][neuron][j] * workingArray[layer-1][j]);
			sum += net.bias[layer][neuron];
			workingArray[layer][neuron] = sigmoid(sum);
		}

		// insert dummy neuron of 1 for the bias
		workingArray[layer][layerIndex[layer]] = 1.0;
	}

	// load the last layer into the answer (output) array
	for (neuron = 0; neuron < layerIndex[numbLayers-1]; neuron++)
		answer[neuron] = workingArray[numbLayers-1][neuron];
}

void backProp(float predictedAnswer[], float trueAnswer[], int numbLayers, int layerIndex[], nn net, float **workingArray, const float learnRate, const float momentum) {
	int layer, neuron, j;
	float error, oldWeight, oldBias, sumErrorProp;
	float* layerError = allocateArray1D(arrayMaxInt(layerIndex, numbLayers));
	// float layerError[NET_NPL];

	// working matrix defined as [layer number][neuron number]
	// weights defined as [layer number][neuron number][neuron connection to layer behind]
	// where layer 0=input, neuron 0=first neuron

	// run error correction for last layer
	layer = numbLayers-1;
	for (neuron = 0; neuron < layerIndex[layer]; neuron++) {
		error = predictedAnswer[neuron] * (1 - predictedAnswer[neuron]) * (trueAnswer[neuron] - predictedAnswer[neuron]);
		layerError[neuron] = error;

		// change weights of all connections backwards
		for (j = 0; j < layerIndex[layer - 1]; j++) {
			oldWeight = net.weightsOld[layer][neuron][j];
			net.weightsOld[layer][neuron][j] = net.weights[layer][neuron][j];
			net.weights[layer][neuron][j] += (1.0-momentum)*learnRate*error*workingArray[layer-1][j] + momentum*(net.weights[layer][neuron][j] - oldWeight);
		}

		// change bias (overall weight)
		oldBias = net.biasOld[layer][neuron];
		net.biasOld[layer][neuron] = net.bias[layer][neuron];
		net.bias[layer][neuron] += (1.0-momentum)*learnRate*error*1.0 + momentum*(net.bias[layer][neuron] - oldBias);				// jth input is just 1 for bias
	}

	// backpropagate error to preceeding layers
	for (layer = numbLayers-2; layer > 0; layer--) {
		for (neuron = 0; neuron < layerIndex[layer]; neuron++) {
			// find this neuron's contribution to the errors on the next layer
			sumErrorProp = 0;
			for (j = 0; j < layerIndex[layer+1]; j++)
				sumErrorProp += layerError[j] * net.weightsOld[layer][j][neuron];

			error = workingArray[layer][neuron] * (1 - workingArray[layer][neuron]) * sumErrorProp;
			layerError[neuron] = error;
			for (j = 0; j < layerIndex[layer - 1]; j++) {
				oldWeight = net.weightsOld[layer][neuron][j];
				net.weightsOld[layer][neuron][j] = net.weights[layer][neuron][j];
				net.weights[layer][neuron][j] += (1.0-momentum)*learnRate*error*workingArray[layer-1][j] + momentum*(net.weights[layer][neuron][j] - oldWeight);
			}

			oldBias = net.biasOld[layer][neuron];
			net.biasOld[layer][neuron] = net.bias[layer][neuron];
			net.bias[layer][neuron] += (1.0-momentum)*learnRate*error*1.0 + momentum*(net.bias[layer][neuron] - oldBias);			// jth input is just 1 for bias
		}
	}

	free(layerError);
}

float sigmoid(float argument) {
	return (1.0 / (1.0 + exp(-1.0*argument)));
}
