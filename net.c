// stuff goes here
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "cpgplot.h"
#include "net.h"

int main()
{
	//============================================================================================
	//-------------------------------------------INPUT--------------------------------------------
	//============================================================================================
	printf("\n=======================================================================================\n");
	printf("This program uses a three-layer feedforward neural network with backpropagated learning \n");
	printf("to classify numerical datasets\n");
	printf("=======================================================================================\n\n");

	int i, j, k;
	int nread = 0, numbLayers = 3, printNormalized = 0;
	dataset training, testing;
	char filename[100];

	fgets(filename, 100, popen("ls *.config | wc -l", "r"));
	printf("%c .config files available:\n", filename[0]);
	system("ls *.config");
	printf("\nPlease enter config filename:\n");
	scanf("%s", filename);

	printf("Loading config file %s\n\n", filename);
	FILE *stream = fopen(filename, "r");
	if (stream == NULL)
		errorCase(ERR_FILE_OPEN);

	// input size of dataset
	nread = fscanf(stream, "%*s %d", &training.numbInstances);
	if (nread != 1)
		errorCase(ERR_INVALID_INPUT);
	printf("Dataset contains:\n     %d training instances\n", training.numbInstances);
	nread = fscanf(stream, "%*s %d", &training.numbIn);
	if (nread != 1)
		errorCase(ERR_INVALID_INPUT);
	printf("     %d training attributes\n", training.numbIn);
	nread = fscanf(stream, "%*s %d", &training.numbOut);
	if (nread != 1)
		errorCase(ERR_INVALID_INPUT);
	printf("     %d answer attributes\n", training.numbOut);

	// open files and save into allocated array
	fscanf(stream, "%*s %s", filename);
	printf("Training dataset: ");
	training.datasetIn = readCSV(filename, training.numbInstances, training.numbIn);
	fscanf(stream, "%*s %s", filename);
	printf("Training answer key: ");
	training.datasetKey = readCSV(filename, training.numbInstances, training.numbOut);

	// generate index of number of neurons in each layer
	nread = fscanf(stream, "%*s %d", &numbLayers);
	if (nread != 1)
		errorCase(ERR_INVALID_INPUT);
	printf("\n\nNumber of layers in neural net: %d\n", numbLayers);
	int* layerIndex = allocateArray1DInt(numbLayers);
	layerIndex[0] = training.numbIn; layerIndex[numbLayers-1] = training.numbOut;
	fscanf(stream, "%*s");
	for (i=1; i<numbLayers-1; i++) {
		nread = fscanf(stream, " %d", &layerIndex[i]);
		if (nread != 1)
			errorCase(ERR_INVALID_INPUT);
	}
	printf("Number of neurons in hidden layers: ");
	for (i=0; i<numbLayers; i++)
		printf("%d ", layerIndex[i]);
	printf("\n");

	// same for test dataset
	nread = fscanf(stream, "%*s %d", &testing.numbInstances);
	if (nread != 1)
		errorCase(ERR_INVALID_INPUT);
	printf("Dataset contains:\n     %d testing instances\n", testing.numbInstances);
	testing.numbIn = training.numbIn;
	printf("     %d training attributes\n", training.numbIn);
	testing.numbOut = training.numbOut;
	printf("     %d answer attributes\n", training.numbOut);

	fscanf(stream, "%*s %s", filename);
	printf("Testing dataset: ");
	testing.datasetIn = readCSV(filename, testing.numbInstances, testing.numbIn);
	fscanf(stream, "%*s %s", filename);
	printf("Testing answer key: ");
	testing.datasetKey = readCSV(filename, testing.numbInstances, testing.numbOut);

	nread = fscanf(stream, "%*s %d", &printNormalized);
	if (nread != 1)
		errorCase(ERR_INVALID_INPUT);

	fclose(stream);

	// generate neural network arrays
	nn net1;
	net1.bias = makeNetArray(layerIndex, numbLayers);
	net1.biasOld = makeNetArray(layerIndex, numbLayers);
	net1.weights = makeWeightsArray(layerIndex, numbLayers);
	net1.weightsOld = makeWeightsArray(layerIndex, numbLayers);
	float** workingArray = makeNetArray(layerIndex, numbLayers);

	printf("\nNetwork Build OK!\n\n");


	//============================================================================================
	//----------------------------------VARIABLE INITIALIZATION-----------------------------------
	//============================================================================================

	// arrays for reading/holding data set
	int* randomSort = allocateArray1DInt(training.numbInstances);									//index of randomly sorted single-occuring integers

	// arrays for passing in/out between functions
	float* input = allocateArray1D(layerIndex[0]);
	float* plotImg = allocateArray1D(layerIndex[0]);
	float* predictedAnswer = allocateArray1D(layerIndex[numbLayers-1]);
	float* trueAnswer = allocateArray1D(layerIndex[numbLayers-1]);

	// arrays for neural net
	int finalAnswer, finalTrueAnswer;

	// counters
	int tChron, tEpoch, tPeriod, tEon;

	// diagnostic vars
	float meanSqrdError, totalMSE;
	int accuracy;

	// timer
	clock_t timerStart, timerDiff;
	int timerMsec;

	// output file arrays
	float** outputFile = allocateArray2D(MAX_EPOCH, MAX_PERIOD);
	float** outputAccuracy = allocateArray2D(MAX_EPOCH, MAX_ARR_COLS);
	float** outputMSE = allocateArray2D(MAX_EPOCH, MAX_ARR_COLS);
	float** outputMSEDiagnostic = allocateArray2D(MAX_EPOCH, MAX_ARR_COLS);

	// normalize
	training.datasetNorm = allocateArray2D(training.numbInstances, training.numbIn);
	testing.datasetNorm = allocateArray2D(testing.numbInstances, testing.numbIn);
	normalize(training.datasetIn, training.datasetNorm, training.numbInstances, training.numbIn);
	normalize(testing.datasetIn, testing.datasetNorm, testing.numbInstances, testing.numbIn);

	// 
	if (printNormalized == 1) {
		for (i=0; i<layerIndex[0]; i++) {

		}
	}
		// load input numbers and expected answers
		for (j = 0; j < layerIndex[0]; j++)	
			input[j] = testing.datasetNorm[i][j];
		for (j = 0; j < layerIndex[numbLayers-1]; j++)
			trueAnswer[j] = testing.datasetKey[i][j];
	
	// pgplot
	float TR[6] = {0, 1, 0, 0, 0, 1};
	// find appropriate bounds for cpggrey
	float plotMaxBound, plotMinBound;
	if (printNormalized == 1) {
		plotMaxBound = training.datasetNorm[0][0];
		plotMinBound = training.datasetNorm[0][0];
		for (i=0; i<training.numbInstances; i++) {
			if (arrayMaxFloat(training.datasetNorm[i], training.numbInstances) > plotMaxBound)
				plotMaxBound = arrayMaxFloat(training.datasetNorm[i], layerIndex[0]);
			else if (arrayMinFloat(training.datasetNorm[i], training.numbInstances) < plotMinBound)
				plotMinBound = arrayMaxFloat(training.datasetNorm[i], layerIndex[0]);
		}
	}
	else {
		plotMaxBound = training.datasetIn[0][0];
		plotMinBound = training.datasetIn[0][0];
		for (i=0; i<training.numbInstances; i++) {
			if (arrayMaxFloat(training.datasetIn[i], training.numbInstances) > plotMaxBound)
				plotMaxBound = arrayMaxFloat(training.datasetIn[i], layerIndex[0]);
			else if (arrayMinFloat(training.datasetIn[i], training.numbInstances) < plotMinBound)
				plotMinBound = arrayMaxFloat(training.datasetIn[i], layerIndex[0]);
		}
	}

	// parameters
	const float maxMSE = 0.01, targetAccuracy = 0.98, targetMSE = 0.02;
	float learnRate = 0.1, momentum = 0.03;
	int pixelDimension;

	//=========================================================================================
	//----------------------------------OTHER INITIALIZATIONS----------------------------------
	//=========================================================================================
	
	// initialize random seed
	srand(time(NULL));

	// force print all outputs (remove stdout buffer)
	setbuf(stdout, NULL);

	// initialize pgplot window
	if (!cpgopen("/XWINDOW"))
		errorCase(ERR_PGPLOT);
	cpgenv(0.0, MAX_EPOCH, 0.0, 0.2, 0, 1);

	//=========================================================================================
	//---------------------------------------ACTUAL CODE---------------------------------------
	//=========================================================================================
	timerStart = clock();
	tEon = 0;

	// Used to test and compare various neural net configurations (change learn rate, momentum etc);
	while (tEon < MAX_EON) {
		tPeriod = 0;

		// Number of repetitions for a single configuration
		while (tPeriod < MAX_PERIOD) {
			randomizeNet(net1, layerIndex, numbLayers);
			tEpoch = 0; accuracy = 0; totalMSE = 100000;
			
			// TRAIN until net reaches target accuracy
			while (totalMSE > targetMSE && tEpoch < MAX_EPOCH) {
			
				createRandomSort(randomSort, training.numbInstances);
				totalMSE = 0;
				// TRAIN all sets in one Epoch
				for (i = 0; i < training.numbInstances; i++) {
					// load input and expected answers
					for (j = 0; j < layerIndex[0]; j++)
						input[j] = training.datasetNorm[randomSort[i]][j];
					for (j = 0; j < layerIndex[numbLayers-1]; j++)
						trueAnswer[j] = training.datasetKey[randomSort[i]][j];
					meanSqrdError = 1;
					tChron = 0;

					//repeat error correction for one training input
					while (meanSqrdError > maxMSE && tChron < MAX_CHRON) {
						feed(input, numbLayers, layerIndex, net1, workingArray, predictedAnswer);
						backProp(predictedAnswer, trueAnswer, numbLayers, layerIndex, net1, workingArray, learnRate, momentum);
						meanSqrdError = 0;
						for (k = 0; k < layerIndex[numbLayers-1]; k++)
							meanSqrdError += pow((predictedAnswer[k] - trueAnswer[k]), 2.0);
						meanSqrdError /= layerIndex[numbLayers-1];
						tChron++;
					}
					totalMSE += meanSqrdError;
				}

				//TEST the entire set, count how many are correct
				accuracy = 0;
				for (i = 0; i < testing.numbInstances; i++) {
					for (j = 0; j < layerIndex[0]; j++)
						input[j] = testing.datasetNorm[i][j]; 
					for (j = 0; j < layerIndex[numbLayers-1]; j++)
						trueAnswer[j] = testing.datasetKey[i][j];
					feed(input, numbLayers, layerIndex, net1, workingArray, predictedAnswer);

					finalAnswer = arrayMaxIndex(predictedAnswer, layerIndex[numbLayers-1]);
					finalTrueAnswer = arrayMaxIndex(trueAnswer, layerIndex[numbLayers-1]);
					if (finalAnswer == finalTrueAnswer)
						accuracy++;
				}
				totalMSE /= training.numbInstances;
				outputAccuracy[tEpoch][tPeriod] = (float)accuracy;
				outputMSE[tEpoch][tPeriod] = totalMSE;
				tEpoch++;

				cpgdraw(tEpoch, totalMSE);
				displayProgress(tEpoch, "Epoch");
			}
			printf("\rFINAL Accuracy: %i/%i - over %i Epochs, MSE = %f\n", accuracy, testing.numbInstances, tEpoch, totalMSE);

			// fills in gaps (downwards, after program stops using MSE condition) in output 
			for (; tEpoch < MAX_EPOCH; tEpoch++) {
				outputAccuracy[tEpoch][tPeriod] = (float)accuracy;
				outputMSE[tEpoch][tPeriod] = totalMSE;
			}
			tPeriod++;
		}
		printf("\rFINISHED %i Periods on Eon %i, learnRate=%f, momentum=%f\n", tPeriod, tEon, learnRate, momentum);
		// averages and saves value for one network configuration
		for (i = 0; i < MAX_EPOCH; i++) {
			outputMSEDiagnostic[i][tEon] = arrayAverageRow(outputMSE, i, MAX_PERIOD);
		}
		tEon++;
		//learnRate += 0.1;
	}
	timerDiff = clock() - timerStart;
	timerMsec = timerDiff*1000 / CLOCKS_PER_SEC;
	printf("\nTraining completed in %i seconds %i milliseconds\n", timerMsec/1000, timerMsec%1000);


	//final stats and write to file
	writeCSV("net_output_stuffs.csv", outputAccuracy, MAX_EPOCH, MAX_ARR_COLS);
	writeCSV("net_output_MSE.csv", outputMSE, MAX_EPOCH, MAX_ARR_COLS);
	writeCSV("net_output_DiagnosticMSE.csv", outputMSEDiagnostic, MAX_EPOCH, MAX_ARR_COLS);
	printf("File write successful!\n");


	pixelDimension = (int)floor(sqrt(training.numbIn));
	cpgenv(1, pixelDimension, pixelDimension, 1, 0, 1);

	// run values from the dataset and display outputs
	while (42) {
		scanf("%i", &i);
		timerStart = clock();
		// load input numbers and expected answers
		for (j = 0; j < layerIndex[0]; j++)	
			input[j] = testing.datasetNorm[i][j];
		for (j = 0; j < layerIndex[numbLayers-1]; j++)
			trueAnswer[j] = testing.datasetKey[i][j];
		feed(input, numbLayers, layerIndex, net1, workingArray, predictedAnswer);
		timerDiff = clock() - timerStart;
		timerMsec = timerDiff*1000000 / CLOCKS_PER_SEC;
		printf("Answer found in %i ns\n", timerMsec);
		
		// pgplot
		if (printNormalized == 0)
			for (j = 0; j < layerIndex[0]; j++)
				plotImg[j] = (float)(testing.datasetIn[i][j]);	
		else
			for (j = 0; j < layerIndex[0]; j++)
				plotImg[j] = (float)(testing.datasetNorm[i][j]);
		//cpggray(plotImg, pixelDimension, pixelDimension, 1, pixelDimension, 1, pixelDimension, arrayMinFloat(plotImg, layerIndex[0]), arrayMaxFloat(plotImg, layerIndex[0]), TR);
		cpggray(plotImg, pixelDimension, pixelDimension, 1, pixelDimension, 1, pixelDimension, plotMinBound, plotMaxBound, TR);
		// prints calculated and expected answer for comparison
		for (i = 0; i < layerIndex[numbLayers-1]; i++) {
			printf("%.2f, ", predictedAnswer[i]);
		} printf("\n");
		for (i = 0; i < layerIndex[numbLayers-1]; i++) {
			printf("%.2f, ", trueAnswer[i]);
		} printf("\n");
	}
	return 0;
}
