#define MAX_LINE 30000					//max chars in a line
#define MAX_CHRON 1
#define MAX_EPOCH 200
#define MAX_PERIOD 1		//50
#define MAX_EON 1
#define MAX_ARR_ROWS 500
#define MAX_ARR_COLS 50

#define NUMB_ROWS 100		//150
#define NUMB_TRAIN 100		//150
#define NUMB_COLS 794		//7
//#define NET_LAYERS 3
//#define NET_NPL	784			//7			//neurons per layer
//#define NET_NUR_IN 784		//4
//#define NET_NUR_OUT 10		//3

#define ERR_INVALID_INPUT 1
#define ERR_MALLOC_FAIL 2
#define ERR_FILE_OPEN 3
#define ERR_PGPLOT 4

#define X_MIN 0.0
#define X_MAX 28.0
#define Y_MIN 0.0
#define Y_MAX 28.0

// net type
typedef struct {float*** weights; float*** weightsOld; float** bias; float** biasOld; } nn;
typedef struct {float** datasetIn; float** datasetNorm; float** datasetKey; int numbInstances; int numbIn; int numbOut; } dataset;

// making arrays and mem management
float* allocateArray1D(const int dim1);
int* allocateArray1DInt(const int dim1);
float** allocateArray2D(const int dim1, const int dim2);							// generic array allocation
float*** allocateArray3D(const int dim1, const int dim2, const int dim3);
float** makeNetArray(int layerIndex[], const int numbLayers);						// specific array allocation
float*** makeWeightsArray(int layerIndex[], const int numbLayers);
void randomizeNet(nn net, int layerIndex[], const int numbLayers);					// inits weights as random between 0 and 1

// importing data
float** readCSV(const char filename[], const int lengthRow, const int lengthCol);
int writeCSV(const char filename[], float** arrayIn, const int lengthRow, const int lengthCol);

// outputting and displaying data
void displayProgress(const int count, const char name[]);
void printWeights(float*** weights, int layerIndex[], const int numbLayers);

// data processing
void normalize(float** arrayIn, float** arrayOut, const int lengthRow, const int lengthCol);
void createRandomSort(int randomSort[], const int lengthRow);
float arrayAverageRow(float** arr8ayIn, const int rowNumber, const int lengthCol);
int arrayMaxIndex(float* arrayIn, int arrayLength);
int arrayMaxInt(int* arrayIn, int arrayLength);
float arrayMaxFloat(float* arrayIn, int arrayLength);
float arrayMinFloat(float* arrayIn, int arrayLength);

// neural net stuffs
void feed(float* input, int numbLayers, int layerIndex[], nn net, float** workingArray, float* answer);
void backProp(float* predictedAnswer, float* trueAnswer, int numbLayers, int layerIndex[], nn net, float** workingArray, const float learnRate, const float momentum);
float sigmoid(float argument);

// errors
void errorCase(const int errorCode);

