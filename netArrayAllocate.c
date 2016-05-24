#include <stdlib.h>
#include <stdio.h>
#include "net.h"

float* allocateArray1D(const int dim1) {
	int i;
	float* newArray;
	newArray = (float*)malloc(dim1*sizeof(float*));
	if (newArray == NULL)
		printf("Malloc failed!");
	return newArray;
}

int* allocateArray1DInt(const int dim1) {
	int* newArray;
	newArray = (int*)malloc(dim1*sizeof(int*));
	if (newArray == NULL)
		printf("Malloc failed!");
	return newArray;
}

float** allocateArray2D(const int dim1, const int dim2) {
	int i, j;
	float** newArray;
	newArray = (float**)malloc(dim1*sizeof(float*));
	if (newArray == NULL)
		printf("Malloc failed!");
	for (i = 0; i < dim1; i++) {
		newArray[i] = (float*)malloc(dim2*sizeof(float));
		if (newArray[i] == NULL)
			printf("Malloc failed!");
	}
	return newArray;
}
float*** allocateArray3D(const int dim1, const int dim2, const int dim3) {
	int i, j, k;
	float*** newArray;
	newArray = (float***)malloc(dim1*sizeof(float**));
	if (newArray == NULL)
		printf("Malloc failed!");
	for (i = 0; i < dim1; i++) {
		newArray[i] = (float**)malloc(dim2*sizeof(float*));
		if (newArray[i] == NULL)
			printf("Malloc failed!");
		for (j = 0; j < dim2; j++) {
			newArray[i][j] = (float*)malloc(dim3*sizeof(float));
			if (newArray[i][j] == NULL)
				printf("Malloc failed!");
		}
	}
	return newArray;
}

float*** makeWeightsArray(int layerIndex[], const int numbLayers){
	int i, j, k;
	//int numbLayers = (sizeof(layerIndex)/sizeof(layerIndex[0]));
	float*** newArray;

	// create pointer for entire array
	newArray = (float***)malloc(numbLayers*sizeof(float**));
	if (newArray == NULL)
		printf("Malloc failed!");

	// for each layer, create a 2-dimensional array of pointers
	for(i = 0; i < numbLayers; i++) {
		newArray[i] = (float**)malloc((layerIndex[i])*sizeof(float*));
		if (newArray[i] == NULL)
			printf("Malloc failed!");
		if (i != 0) {
			for (j = 0; j < (layerIndex[i]+1); j++) {
				newArray[i][j] = (float*)malloc((layerIndex[i-1])*sizeof(float));
				if (newArray[i][j] == NULL)
					printf("Malloc failed!");
			}
		}
	}
	// returns pointer
	return newArray;
}
float** makeNetArray(int layerIndex[], const int numbLayers){
	int i, j;
	float** newArray;
	newArray = (float **)malloc(numbLayers*sizeof(float*));
	if (newArray == NULL)
		printf("Malloc failed!");
	for (i = 0; i < numbLayers; i++) {
		newArray[i] = (float *)malloc(layerIndex[i]*sizeof(float));
		if (newArray[i] == NULL)
			printf("Malloc failed!");
	}
	return newArray;
}
