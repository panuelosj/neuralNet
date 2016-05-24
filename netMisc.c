#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "net.h"

void randomizeNet(nn net, int layerIndex[], const int numbLayers){
	int i, j, k;

	for (i = 1; i < numbLayers; i++) {
		for (j = 0; j < (layerIndex[i]); j++) {
			for (k = 0; k < (layerIndex[i-1]); k++) {
				net.weights[i][j][k] = ((float)rand()*2.0 / (float)RAND_MAX) - 1.0;
				net.weightsOld[i][j][k] = net.weights[i][j][k];
			}
		}
	}
	for (i = 1; i < numbLayers; i++) {
		for (j = 0; j < (layerIndex[i]); j++) {
			net.bias[i][j] = ((float)rand()*2.0 / (float)RAND_MAX) - 1.0;
			net.biasOld[i][j] = net.bias[i][j];
		}
	}
}

void normalize(float **arrayIn, float **arrayOut, const int lengthRow, const int lengthCol) {
	int i, j;
	for (j = 0; j < lengthCol; j++) {	//normalizes inputs based on standard score
		float sum = 0.0;
		float mean = 0.0;
		float variance = 0.0;
		float sd = 0.0;

		for (i = 0; i < lengthRow; i++)
			sum += arrayIn[i][j];
		mean = sum / lengthRow;
		if (mean != 0) {
			for (i = 0; i < lengthRow; i++)
				variance += ((arrayIn[i][j] - mean)*(arrayIn[i][j] - mean));
			sd = sqrt(variance / lengthRow);				//FLAG FOR FIX

			for (i = 0; i < lengthRow; i++)
				arrayOut[i][j] = ((arrayIn[i][j] - mean) / sd);
		}
		else
			for (i = 0; i < lengthRow; i++)
				arrayOut[i][j] = 0;
	}
}

void createRandomSort(int randomSort[], const int lengthRow) {
	int i, inew, iold;
	for (i = 0; i < lengthRow; i++)		//create ordered index list 1->total
		randomSort[i] = i;
	for (i = 0; i < lengthRow; i++) {		//swap each index with a random index
		inew = rand() % lengthRow;
		iold = randomSort[i];
		randomSort[i] = randomSort[inew];
		randomSort[inew] = iold;
	}
}

int arrayMaxIndex(float* arrayIn, int arrayLength) {
	// Finds the index of the maximum value in a 1D array
	int i, imax = 0;
	float max = 0;
	for (i = 0; i < arrayLength; i++) {
		if (arrayIn[i] > max) {
			max = arrayIn[i];
			imax = i;
		}
	}
	return imax;
}

int arrayMaxInt(int* arrayIn, int arrayLength) {
	// Finds the maximum value in a 1D array
	int i, max = arrayIn[0];
	for (i = 0; i < arrayLength; i++) {
		if (arrayIn[i] > max)
			max = arrayIn[i];
	}
	return max;
}

float arrayMaxFloat(float* arrayIn, int arrayLength) {
	// Finds the maximum value in a 1D array
	int i;
	float max = arrayIn[0];
	for (i = 0; i < arrayLength; i++) {
		if (arrayIn[i] > max)
			max = arrayIn[i];
	}
	return max;
}
float arrayMinFloat(float* arrayIn, int arrayLength) {
	// Finds the maximum value in a 1D array
	int i;
	float min = arrayIn[0];
	for (i = 0; i < arrayLength; i++) {
		if (arrayIn[i] < min)
			min = arrayIn[i];
	}
	return min;
}

float arrayAverageRow(float** arrayIn, const int rowNumber, const int lengthCol) {
	// Averages the values of an array over one direction
	int j;
	float sum = 0;
	for (j = 0; j < lengthCol; j++)
		sum += arrayIn[rowNumber][j];
	return (sum / lengthCol);
}

void printWeights(float*** weights, int layerIndex[], const int numbLayers){
	// Prints the current values of all weights
	int i, j, k;

	for (i = 1; i < numbLayers; i++) {
		for (j = 0; j < (layerIndex[i]); j++) {
			for (k = 0; k < (layerIndex[i-1]); k++) {
				printf("%.2f ", weights[i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
}

void displayProgress(const int count, const char name[]){
	// Displays a rotating bar to show training is still in progress
	if (count % 10 == 0)
		printf("\r%s %d completed!\n", name, count);
	if (count % 4 == 0)
		printf("\r|");
	else if (count % 4 == 1)
		printf("\r\\");
	else if (count % 4 == 2)
		printf("\r-");
	else if (count % 4 == 3)
	printf("\r/");
}

void errorCase(const int errorCode){
	system("cat nagato");
	switch (errorCode){
		case ERR_INVALID_INPUT:
			printf("Error: invalid input\n");
			exit(-1);
		case ERR_MALLOC_FAIL:
			printf("Error: out of memory\n");
			exit(-1);
		case ERR_FILE_OPEN:
			printf("Error: file cannot be opened\n");
			exit(-1);
		case ERR_PGPLOT:
			printf("Error: cannot open pgplot window\n");
			exit(-1);
	}
}

